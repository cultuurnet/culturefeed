<?php

require_once(realpath(dirname(__FILE__)) . '/OAuth.php');

/**
 * @file
 * Classes to work with CultuurNet's Culture Feed API.
 */

/**
 * Class to represent a basic HTTP request.
 */
class CultureFeedHTTPRequest {

  /**
   * Proxy server URI.
   *
   * @var string
   */
  protected $proxy_server;

  /**
   * Proxy server port.
   *
   * @var integer
   */
  protected $proxy_port;

  /**
   * Proxy server username.
   *
   * @var string
   */
  protected $proxy_username;

  /**
   * Proxy server password.
   *
   * @var string
   */
  protected $proxy_password;

  /**
   * Connection timeout (defaults to 15 seconds).
   *
   * @var integer
   */
  protected $timeout = 15;

  /**
   * Set the proxy server URI.
   *
   * @param string $proxy_server
   */
  public function setProxyServer($proxy_server) {
    $this->proxy_server = $proxy_server;
  }

  /**
   * Set the proxy server port.
   *
   * @param integer $proxy_port
   */
  public function setProxyPort($proxy_port) {
    $this->proxy_port = $proxy_port;
  }

  /**
   * Set the proxy server username.
   *
   * @param string $proxy_username
   */
  public function setProxyUsername($proxy_username) {
    $this->proxy_username = $proxy_username;
  }

  /**
   * Set the proxy server password
   *
   * @param string $proxy_password
   */
  public function setProxyPassword($proxy_password) {
    $this->proxy_password = $proxy_password;
  }

  /**
   * Set the connection timeout.
   *
   * @param integer $timeout
   */
  public function setTimeout($timeout) {
    $this->timeout = $timeout;
  }

  /**
   * Make an HTTP request
   *
   * @param string $url
   *   The URL for the request.
   * @param array $http_headers
   *   HTTP headers to set on the request.
   *   Represented as an array of header strings.
   * @param string $method
   *   The HTTP method.
   * @param string $post_data
   *   In case of a POST request, specify the post data a string.
   * @return string
   *   The response body.
   *
   * @throws CultureFeedException
   *   If an error message and code could be parsed from the response.
   * @throws CultureFeedHTTPException
   *   If no error message and code could be parsed from the response.
   */
  public function httpRequest($url, $http_headers = array(), $method = 'GET', $post_data = '') {
    // Initialising some general CURL options (url, timeout, ...).
    $curl_options = array(
      CURLOPT_URL => $url,
      CURLOPT_TIMEOUT => $this->timeout,
      CURLOPT_RETURNTRANSFER => TRUE,
      CURLOPT_HEADER => FALSE,
    );

    // If a proxy server is set, configure CURL to use it.
    if (!empty($this->proxy_server)) {
      $curl_options[CURLOPT_PROXY] = $this->proxy_server;
      $curl_options[CURLOPT_PROXYPORT] = $this->proxy_port;
      if ($this->proxy_username) {
        $curl_options[CURLOPT_PROXYUSERPWD] = sprintf('%s:%s', $this->proxy_username, $this->proxy_password);
      }
    }

    // Set HTTP headers.
    if (!empty($http_headers)) {
      $curl_options[CURLOPT_HTTPHEADER] = $http_headers;
    }

    // If the method is POST, configure CURL for it and set the post data.
    if ($method == 'POST') {
      $curl_options[CURLOPT_POST] = TRUE;
      $curl_options[CURLOPT_POSTFIELDS] = $post_data;
    }

    // Do the CURL request.
    $ch = curl_init();
    curl_setopt_array($ch, $curl_options);

    $response  = curl_exec($ch);
    $curl_info = curl_getinfo($ch);
    $curl_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);

    curl_close($ch);

    // In case the HTTP response status is not 200, we consider this an error.
    // In case we can parse a code and message from the response, we throw a CultureFeedException.
    // In case we can't parse a code and message, we throw a CultureFeedHTTPException.
    if ($curl_code != 200) {
      try {
        $xml = new CultureFeedSimpleXMLElement($response);
      }
      catch (Exception $e) {
        throw new CultureFeedHTTPException($curl_code, $response, $curl_info);
      }

      if ($code = $xml->xpath_str('/response/code')) {
        $message = $xml->xpath_str('/response/message');
        throw new CultureFeedException($code, $message);
      }

      throw new CultureFeedHTTPException($curl_code, $response, $curl_info);
    }

    // In case the HTTP response status is 200, we return the response.
    return $response;
  }

}

/**
 * Class to represent a OAuth request.
 */
class CultureFeedOAuthRequest extends CultureFeedHTTPRequest {

  /**
   * Endpoint (full url) where the CultureFeed API resides.
   *
   * @var string
   */
  protected $endpoint = 'http://test.uitid.be/culturefeed/rest/';

  /**
   * Signature method for signing OAuth requests.
   *
   * @var OAuthSignatureMethod
   */
  protected $signature_method;

  /**
   * OAuth consumer token.
   *
   * @var OAuthConsumer
   */
  protected $consumer;

  /**
   * OAuth token (request, access, ...).
   *
   * @var OAuthConsumer
   */
  protected $token;

  /**
   * Constructor for a new CultureFeedOAuthRequest instance.
   *
   * @param string $consumer_key
   *   Consumer key.
   * @param string $consumer_secret
   *   Consumer secret.
   * @param string $oauth_token
   *   (optional) Token.
   * @param string $oauth_token_secret
   *   (optional) Token secret.
   */
  public function __construct($consumer_key, $consumer_secret, $oauth_token = NULL, $oauth_token_secret = NULL) {
    $this->signature_method = new OAuthSignatureMethod_HMAC_SHA1();
    $this->consumer = new OAuthConsumer($consumer_key, $consumer_secret);
    if (!empty($oauth_token) && !empty($oauth_token_secret)) {
      $this->token = new OAuthConsumer($oauth_token, $oauth_token_secret);
    }
  }

  /**
   * Set the endpoint.
   *
   * @param string $endpoint
   */
  public function setEndpoint($endpoint) {
    $this->endpoint = $endpoint;
  }

  /**
   * Do a GET request with only a consumer token.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function consumerGet($path, $params = array(), $format = '') {
    return $this->request($path, $params, 'GET', FALSE, $format);
  }

  /**
   * Do a GET request with only a consumer token and return the response as XML.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function consumerGetAsXml($path, $params = array()) {
    return $this->consumerGet($path, $params, 'xml');
  }

  /**
   * Do a GET request with only a consumer token and return the response as JSON.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function consumerGetAsJson($path, $params = array()) {
    return $this->consumerGet($path, $params, 'json');
  }

  /**
   * Do a POST request with only a consumer token.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function consumerPost($path, $params = array(), $raw_post = TRUE, $has_file_upload = FALSE, $format = '') {
    return $this->request($path, $params, 'POST', FALSE, $format, $raw_post, $has_file_upload = FALSE);
  }

  /**
   * Do a POST request with only a consumer token and return the response as XML.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function consumerPostAsXml($path, $params = array(), $raw_post = TRUE, $has_file_upload = FALSE) {
    return $this->consumerPost($path, $params, $raw_post, $has_file_upload, 'xml');
  }

  /**
   * Do a POST request with only a consumer token and return the response as JSON.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function consumerPostAsJson($path, $params = array(), $raw_post = TRUE, $has_file_upload = FALSE) {
    return $this->consumerPost($path, $params, $raw_post, $has_file_upload, 'json');
  }

  /**
   * Do a GET request with a consumer token and user token.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function authenticatedGet($path, $params = array(), $format = '') {
    return $this->request($path, $params, 'GET', TRUE, $format);
  }

  /**
   * Do a GET request with a consumer token and user token and return the response as XML.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function authenticatedGetAsXml($path, $params = array()) {
    return $this->authenticatedGet($path, $params, 'xml');
  }

  /**
   * Do a GET request with a consumer token and user token and return the response as JSON.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function authenticatedGetAsJson($path, $params = array()) {
    return $this->authenticatedGet($path, $params, 'json');
  }

  /**
   * Do a POST request with a consumer token and user token.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function authenticatedPost($path, $params = array(), $raw_post = TRUE, $has_file_upload = FALSE, $format = '') {
    return $this->request($path, $params, 'POST', TRUE, $format, $raw_post, $has_file_upload);
  }

  /**
   * Do a POST request with a consumer token and user token and return the response as XML.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function authenticatedPostAsXml($path, $params = array(), $raw_post = TRUE, $has_file_upload = FALSE) {
    return $this->authenticatedPost($path, $params, $raw_post, 'xml');
  }

  /**
   * Do a POST request with a consumer token and user token and return the response as JSON.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function authenticatedPostAsJson($path, $params = array(), $raw_post = TRUE, $has_file_upload = FALSE) {
    return $this->authenticatedPost($path, $params, $raw_post, 'json');
  }

  /**
   * Do a OAuth signed request.
   *
   * @param string $path
   *   The path for the request relative to the endpoint.
   * @param string $params
   *   Post data for a POST request, query string for a GET request.
   * @param string $method
   *   HTTP method.
   * @param string $use_auth
   *   Should the request be signed with the user token and token secret or just the consumer token and secret?
   *   If $use_auth is TRUE, sign with user token, secret as well as consumer token and secret.
   *   If $use_auth is FALSE, sign only with consumer token and secret.
   * @param string $format
   *   The response format.
   *   Possible values are 'xml', 'json' and '' for default response (depending on request).
   * @param string $raw_post
   *   Should the post data (passed via $params) be passed as is ($raw_post TRUE) or should the OAuth params be added?
   * @return string
   *   The response body.
   */
  public function request($path, $params = array(), $method = 'GET', $use_auth = TRUE, $format = 'xml', $raw_post = TRUE, $has_file_upload = FALSE) {
    // Getting full URL.
    $url = $this->getUrl($path);

    // Getting the request token for the request based on $use_auth.
    $request_token = $use_auth ? $this->token : NULL;
    
    // Since the OAuth library doesn't support multipart, we don't encode params that have a file.
    $params_to_encode = $has_file_upload ? array() : $params;

    // Constructing the request...
    $request = OAuthRequest::from_consumer_and_token($this->consumer, $request_token, $method, $url, $params_to_encode);

    // ... and signing it.
    $request->sign_request($this->signature_method, $this->consumer, $request_token);

    // Getting the URL for the request.
    $url = $request->to_url();

    if ($method == 'POST') {
      $url = $request->get_normalized_http_url();
    }

    $http_headers = array();

    // Setting the OAuth headers.
    $http_headers[] = $request->to_header();

    // Setting the 'Accept' header.
    switch ($format) {
      case 'json':
        $http_headers[] = 'Accept: application/json';
        break;
      case 'xml':
        $http_headers[] = 'Accept: application/xml';
        break;
    }

    // If we have a file upload, we pass $params as an array to trigger CURL multipart.
    $post_data = $has_file_upload ? $params : http_build_query($params, '', '&');

    // Necessary to support token setup calls.
    if (!$raw_post) {
      $post_data = $request->to_postdata();
    }

    return $this->httpRequest($url, $http_headers, $method, $post_data);
  }

  /**
   * Getting the full URL based on a path and query.
   *
   * @param string $path
   *   The path relative to the endpoint.
   * @param array $query
   *   (optional) The query string represented as an array.
   * @return string
   *   The full URL.
   */
  protected function getUrl($path, $query = array()) {
    $url =  rtrim($this->endpoint, '/') . '/' . trim($path, '/');

    if (!empty($query)) {
      $url .= '?' . http_build_query($query, '', '&');
    }

    return $url;
  }

}

/**
 * The main class to communicate with the Culture Feed API.
 *
 * Provides calls and helper functions to set up OAuth authorization.
 * Provides calls to fetch objects (users, activities, ...) from the API.
 */
class CultureFeed extends CultureFeedOAuthRequest {

  /**
   * Privacy status 'public'.
   */
  const PRIVACY_PUBLIC = 'public';

  /**
   * Privacy status 'private'.
   */
  const PRIVACY_PRIVATE = 'private';

  /**
   * Recommendation evaluation 'unspecified'.
   */
  const RECOMMENDATION_EVALUATION_UNSPECIFIED = 0;

  /**
   * Recommendation evaluation 'positive'.
   */
  const RECOMMENDATION_EVALUATION_POSITIVE = 1;

  /**
   * Recommendation evaluation 'negative'.
   */
  const RECOMMENDATION_EVALUATION_NEGATIVE = 2;

  /**
   * Fetch a request token. Sets the token for this object with the fetched token.
   *
   * The object should be initialized with the consumer token.
   *
   * @return array or FALSE
   *   In case of success an associative array containing the token, secret and callback confirmed status.
   *   Array keys are 'oauth_callback_confirmed', 'oauth_token' and 'oauth_token_secret'.
   *   In case of failure FALSE.
   */
  public function getRequestToken() {
    try {
      $response = $this->consumerPost('requestToken', array(), FALSE);
    }
    catch (Exception $e) {
      return FALSE;
    }

    $token = OAuthUtil::parse_parameters($response);

    if (!isset($token['oauth_token']) || !isset($token['oauth_token'])) {
      return FALSE;
    }

    $this->token = new OAuthConsumer($token['oauth_token'], $token['oauth_token_secret']);

    return $token;
  }

  /**
   * Get the URL of the authorization page on the provider to redirect the user to.
   *
   * The object should be initialized with the consumer token.
   *
   * @param string $token
   *   A request token fetched with getRequestToken.
   * @param string $callback
   *   (optional) The URL of the page to redirect the user back to after authorization has been handled on provider.
   * @return string
   *   The URL of the authorization page.
   */
  public function getUrlAuthorize($token, $callback = '') {
    $query = array('oauth_token' => $token['oauth_token']);

    if (!empty($callback)) {
      $query['oauth_callback'] = $callback;
    }

    return $this->getUrl('auth/authorize', $query);
  }

  /**
   * Fetch an access token. Sets the token for this object with the fetched token.
   *
   * The object should be initialized with the consumer token and user access token.
   *
   * @param string $oauth_verifier
   *   The 'oauth_verifier' that was sent back in the callback URL after the authorization step.
   * @return array or FALSE
   *   In case of success an associative array containing the token, secret and callback confirmed status.
   *   Array keys are 'oauth_callback_confirmed', 'oauth_token' and 'oauth_token_secret'.
   *   In case of failure FALSE.
   */
  public function getAccessToken($oauth_verifier) {
    try {
      $response = $this->authenticatedPost('accessToken', array('oauth_verifier' => $oauth_verifier));
    }
    catch (Exception $e) {
      return FALSE;
    }

    $token = OAuthUtil::parse_parameters($response);

    if (!isset($token['oauth_token']) || !isset($token['oauth_token'])) {
      return FALSE;
    }

    $this->token = new OAuthConsumer($token['oauth_token'], $token['oauth_token_secret']);

    return $token;
  }

  /**
   * Create a new user.
   *
   * The object should be initialized with the consumer token and the user access token of a user who has permission to create users.
   *
   * @param CultureFeedUser $user
   *   The user to create.
   * @return string or FALSE
   *   In case of success a string representing the user id of the newly created user.
   *   In case of failure FALSE.
   *
   * @throws CultureFeedException
   *   If an application error occurred.
   */
  public function createUser($user) {
    $data = $user->toPostData();

    try {
      $result = $this->consumerPostAsXml('user', $data);

      $xml = new CultureFeedSimpleXMLElement($result);
    }
    catch (CultureFeedException $e) {
      throw $e;
    }
    catch (Exception $e) {
      return FALSE;
    }

    if ($uid = $xml->xpath_str('/response/uid')) {
      return $uid;
    }

    return FALSE;
  }

  /**
   * Update an existing user.
   *
   * The object should be initialized with the consumer token and user access token of the user who is acted upon.
   *
   * @param CultureFeedUser $user
   *   The user to update. The user is identified by ID. Only fields that are set will be updated.
   * @return bool
   *   In case of success TRUE.
   *   In case of failure FALSE.
   *
   * @throws CultureFeedException
   *   If an application error occurred.
   */
  public function updateUser($user) {
    $data = $user->toPostData();

    $id = $data['id'];

    unset($data['id']);

    try {
      $this->authenticatedPostAsXml('user/' . $id, $data);

      return TRUE;
    }
    catch (CultureFeedException $e) {
      throw $e;
    }
    catch (Exception $e) {
      return FALSE;
    }
  }

  /**
   * Delete a user.
   *
   * The object should be initialized with the consumer token and user access token of the user who is acted upon.
   *
   * @param string $id
   *   ID of the user who is deleted.
   * @return bool
   *   In case of success TRUE.
   *   In case of failure FALSE.
   */
  public function deleteUser($id) {
    try {
      $this->authenticatedGetAsXml('user/' . $id . '/delete');

      return TRUE;
    }
    catch (Exception $e) {
      return FALSE;
    }
  }

  /**
   * Fetch a user.
   *
   * The user can be fetched on behalf of a user. In this case, the object should be initialized with the consumer token and user access token of the user who is acted upon.
   *
   * @param string $id
   *   ID of the user to fetch.
   * @param bool $private
   *   Boolean indicating wether private fields should be included.
   *   For fetching private fields $use_auth should be TRUE.
   *   Defaults to FALSE.
   * @param bool $use_auth
   *   Boolean indicating wether the request should be done on behalf of a certain user.
   *   In case $use_auth is TRUE, the object should be initialized with an access token.
   *   Defaults to TRUE.
   * @return CultureFeedUser or FALSE
   *   In case of success a CultureFeedUser object representing the requested user.
   *   In case of failure FALSE.
   */
  public function getUser($id, $private = FALSE, $use_auth = TRUE) {
    $query = array('private' => $private ? 'true' : 'false');

    try {
      if ($use_auth) {
        $result = $this->authenticatedGetAsXml('user/' . $id, $query);
      }
      else {
        $result = $this->consumerGetAsXml('user/' . $id, $query);
      }

      $xml = new CultureFeedSimpleXMLElement($result);
    }
    catch (Exception $e) {
      return FALSE;
    }

    return $xml->parseUser($xml);
  }

  /**
   * Search for users.
   *
   * The object should be initialized with the consumer token.
   *
   * @param CultureFeedSearchUsersQuery $query
   *   The search query.
   * @return array or FALSE
   *   In case of success an array of CultureFeedUser objects.
   *   In case of failure FALSE.
   */
  public function searchUsers($query) {
    $data = $query->toPostData();

    try {
      $result = $this->consumerGetAsXml('user/search', $data);

      $xml = new CultureFeedSimpleXMLElement($result);
    }
    catch (Exception $e) {
      return FALSE;
    }

    return $xml->parseUsers($xml);
  }

  /**
   * Get a list of users similar to a given user.
   *
   * The object should be initialized with the consumer token and user access token of the user who is acted upon.
   *
   * @param string $id
   *   ID of the user to fetch similar users for.
   * @return array or FALSE
   *   In case of success an array of CultureFeedUser objects.
   *   In case of failure FALSE.
   */
  public function getSimilarUsers($id) {
    try {
      $result = $this->consumerGetAsXml('user/' . $id . '/similar', array());

      $xml = new CultureFeedSimpleXMLElement($result);
    }
    catch (Exception $e) {
      return FALSE;
    }

    return $xml->parseUsers($xml);
  }

  /**
   * Update a depiction for a user.
   *
   * The object should be initialized with the consumer token and user access token of the user who is acted upon.
   *
   * @param string $id
   *   ID of the user to upload depiction for.
   * @param string $file_data
   *   Binary data of the file to upload.
   * @return array or FALSE
   *   In case of success TRUE.
   *   In case of failure FALSE.
   */  
  public function uploadUserDepiction($id, $file_data) {
    try {
      $this->authenticatedPostAsXml('user/' . $id . '/upload_depiction', array('depiction' => $file_data), TRUE, TRUE);
      
      return TRUE;
    }
    catch (Exception $e) {
      return FALSE;
    }
  }

  /**
   * Resend the e-mail confirmation mail for a user.
   *
   * The object should be initialized with the consumer token and user access token of the user who is acted upon.
   *
   * @param string $id
   *   ID of the user who requests the confirmation e-mail to be resent.
   * @return bool
   *   In case of success TRUE.
   *   In case of failure FALSE.
   */
  public function resendMboxConfirmationForUser($id) {
    try {
      $this->authenticatedPostAsXml('user/' . $id . '/resend_mbox_confirmation');

      return TRUE;
    }
    catch (Exception $e) {
      return FALSE;
    }
  }

  /**
   * Update a user's field privacy settings.
   *
   * The object should be initialized with the consumer token and user access token of the user who is acted upon.
   *
   * @param string $id
   *   ID of the user whose privacy settings to update.
   * @param CultureFeedUserPrivacyConfig $privacy_config
   *   An associative array representing the privacy status for each field.
   *   The array is indexed by the field name. Values are the privacy status.
   *   Possible values for the privacy status are represented in the PRIVACY_* constants.
   * @return bool
   *   In case of success TRUE.
   *   In case of failure FALSE.
   */
  public function updateUserPrivacy($id, $privacy_config) {
    $data = $privacy_config->toPostData();

    try {
      $this->authenticatedPostAsXml('user/' . $id . '/privacy', $data);

      return TRUE;
    }
    catch (Exception $e) {
      return FALSE;
    }
  }

  /**
   * Fetch a list of service consumers the user is connected with.
   *
   * The object should be initialized with the consumer token and user access token of the user who is acted upon.
   *
   * @param string $id
   *   ID of the user to fetch service consumers for.
   * @return array or FALSE
   *   In case of success an array of CultureFeedConsumer objects.
   *   In case of failure FALSE.
   */
  public function getUserServiceConsumers($id) {
    try {
      $result = $this->authenticatedGetAsXml('user/' . $id . '/serviceconsumers');

      $xml = new CultureFeedSimpleXMLElement($result);
    }
    catch (Exception $e) {
      return FALSE;
    }

    return $xml->parseServiceConsumers($xml);
  }

  /**
   * Revoke a service consumer from a user.
   *
   * The object should be initialized with the consumer token and user access token of the user who is acted upon.
   *
   * @param string $user_id
   *   ID of the user who requests the revoke.
   * @param integer id
   *   ID of the service consumer that needs to be revoked.
   * @return bool
   *   In case of success TRUE.
   *   In case of failure FALSE.
   */
  public function revokeUserServiceConsumer($user_id, $consumer_id) {
    try {
      $this->authenticatedPostAsXml('user/' . $user_id . '/serviceconsumers/' . $consumer_id . '/revoke');

      return TRUE;
    }
    catch (Exception $e) {
      return FALSE;
    }
  }

  /**
   * Update a user's online account settings.
   *
   * The object should be initialized with the consumer token and user access token of the user who is acted upon.
   *
   * @param string $id
   *   ID of the user whose online account settings will be updated.
   * @param CultureFeedOnlineAccount $account
   *   The account settings to update.
   * @return bool
   *   In case of success TRUE.
   *   In case of failure FALSE.
   */
  public function updateUserOnlineAccount($id, $account) {
    $data = $account->toPostData();

    try {
      $this->authenticatedPostAsXml('user/' . $id . '/onlineaccount/update', $data);

      return TRUE;
    }
    catch (Exception $e) {
      return FALSE;
    }
  }

  /**
   * Delete an online account for a user.
   *
   * The object should be initialized with the consumer token and user access token of the user who is acted upon.
   *
   * @param string $id
   *   ID of the user who requests the delete.
   * @param string $account_type
   *   Type of online account to delete.
   * @param string $account_name
   *   Account name of the account to delete.
   * @return bool
   *   In case of success TRUE.
   *   In case of failure FALSE.
   */
  public function deleteUserOnlineAccount($id, $account_type, $account_name) {
    $data = array(
      'accountName' => $account_name,
      'accountType' => $account_type,
    );

    try {
      $this->authenticatedPostAsXml('user/' . $id . '/onlineaccount/delete', $data);

      return TRUE;
    }
    catch (Exception $e) {
      return FALSE;
    }
  }

  /**
   * Create a new activity.
   *
   * The object should be initialized with the consumer token and user access token of the user who is acted upon.
   *
   * @param CultureFeedActivity $activity
   *   The activity to create.
   * @return string or FALSE
   *   In case of success a string representing the user id of the newly created user.
   *   In case of failure FALSE.
   *
   * @throws CultureFeedException
   *   If an application error occurred.
   */
  public function createActivity($activity) {
    $data = $activity->toPostData();

    try {
      $result = $this->authenticatedPostAsXml('activity', $data);

      $xml = new CultureFeedSimpleXMLElement($result);
    }
    catch (CultureFeedException $e) {
      throw $e;
    }
    catch (Exception $e) {
      return FALSE;
    }

    if ($id = $xml->xpath_str('/response/activityId')) {
      return $id;
    }

    return FALSE;
  }

  /**
   * Update an existing user.
   *
   * The object should be initialized with the consumer token and user access token of the user who is acted upon.
   *
   * @param string $id
   *   ID of the activity to update.
   * @return bool
   *   In case of success TRUE.
   *   In case of failure FALSE.
   */
  public function updateActivity($id, $private) {
    $data = array('private' => $private ? 'true' : 'false');

    try {
      $this->authenticatedPostAsXml('activity/' . $id, $data);

      return TRUE;
    }
    catch (Exception $e) {
      return FALSE;
    }
  }

  /**
   * Delete an activity.
   *
   * The object should be initialized with the consumer token and user access token of the user who is acted upon.
   *
   * @param string $id
   *   ID of the activity that is deleted.
   * @return bool
   *   In case of success TRUE.
   *   In case of failure FALSE.
   */
  public function deleteActivity($id) {
    try {
      $this->authenticatedGetAsXml('activity/' . $id . '/delete');

      return TRUE;
    }
    catch (Exception $e) {
      return FALSE;
    }
  }

  /**
   * Search for activities.
   *
   * In case the query contains 'private=true', the object should be initialized with the consumer token and user access token of the user who is acted upon.
   * Else the object should be initialized with the consumer token.
   *
   * @param CultureFeedSearchActivitiesQuery $query
   *   The query.
   * @return array or FALSE
   *   In case of success an array of CultureFeedActivity objects.
   *   In case of failure FALSE.
   */
  public function searchActivities($query) {
    $use_auth = FALSE;

    if ($query->private) {
      $use_auth = TRUE;
    }

    $data = $query->toPostData();

    try {
      if ($use_auth) {
        $result = $this->authenticatedGetAsXml('activity', $data);
      }
      else {
        $result = $this->consumerGetAsXml('activity', $data);
      }

      $xml = new CultureFeedSimpleXMLElement($result);
    }
    catch (Exception $e) {
      return FALSE;
    }

    return $xml->parseActivities($xml);
  }

  /**
   * Fetch a list of events that have the most activity.
   *
   * The object should be initialized with the consumer token and user access token of the user who is acted upon.
   *
   * @param string $type
   *   The type of activity that is used to measure 'most activity'.
   *   Possible values are 'like', 'facebook', 'twitter', 'attend' and 'active'.
   *   The type 'active' is the aggregate of all other activities.
   * @return array or FALSE
   *   In case of success an array of event CDBIDs (string).
   *   In case of failure FALSE.
   */
  public function getTopEvents($type) {
    try {
      $result = $this->consumerGetAsXml('activity/topevents/' . $type);

      $xml = new CultureFeedSimpleXMLElement($result);
    }
    catch (Exception $e) {
      return FALSE;
    }

    return $xml->xpath_str('/response/events/event/cdbid', TRUE);
  }

  /**
   * Fetch a list of recommendations for a user.
   *
   * The object should be initialized with the consumer token and user access token of the user who is acted upon.
   *
   * @param string $id
   *   ID of the user recommendations should be based on.
   * @param array $query
   *   An associative array representing the query to refine the recommendations.
   *   The array is indexed by the query key.
   * @return array or FALSE
   *   In case of success an array of CultureFeedRecommendation objects.
   *   In case of failure FALSE.
   */
  public function getRecommendationsForUser($id, $query = array()) {
    try {
      $result = $this->authenticatedGetAsXml('recommendation/user/' . $id, $query);

      $xml = new CultureFeedSimpleXMLElement($result);
    }
    catch (Exception $e) {
      return FALSE;
    }

    return $xml->parseRecommendations($xml);
  }

  /**
   * Fetch a list of recommendations based on a given event.
   *
   * The object should be initialized with the consumer token and user access token of the user who is doing the action.
   *
   * @param string $id
   *   CDBID of the event recommendations should be based on.
   * @param array $query
   *   An associative array representing the query to refine the recommendations.
   *   The array is indexed by the query key.
   * @return array or FALSE
   *   In case of success an array of CultureFeedRecommendation objects.
   *   In case of failure FALSE.
   */
  public function getRecommendationsForEvent($id, $query = array()) {
    $query['eventId'] = $id;

    try {
      $result = $this->authenticatedGetAsXml('recommendation/event', $query);

      $xml = new CultureFeedSimpleXMLElement($result);
    }
    catch (Exception $e) {
      return FALSE;
    }

    return $xml->parseRecommendations($xml);
  }

  /**
   * Evaluate a recommendation as postive, negative (or unspecified).
   *
   * The object should be initialized with the consumer token and user access token of the user who is acted upon.
   *
   * @param string $id
   *   ID of the recommendation that is evaluated.
   * @param integer $evaluation
   *   Evaluation for the recommendation.
   *   Possible values are represented in the RECOMMENDATION_EVALUATION_* constants.
   * @return bool
   *   In case of success TRUE.
   *   In case of failure FALSE.
   */
  public function evaluateRecommendation($id, $evaluation) {
    $data = array('evaluation' => $evaluation);

    try {
      $this->authenticatedPostAsXml('recommendation/evaluate/' . $id, $data);

      return TRUE;
    }
    catch (Exception $e) {
      return FALSE;
    }
  }

  /**
   * Get the URL of the page to allow a user to connect his account with a social service (Twitter, Facebook, Google).
   *
   * @param string $network
   *   The network to connect to. Possible values are 'facebook', 'twitter' and 'google'.
   * @param string $destination
   *   (optional) The URL of the page to redirect the user back to after the password change has been handled on the provider.
   * @return string
   *   The URL of the 'connect social network' page.
   */
  public function getUrlAddSocialNetwork($network, $destination = '') {
    $query = array();

    if (!empty($destination)) {
      $query = array('destination' => $destination);
    }

    return $this->getUrl('auth/network/extra', array('network' => $network, $query));
  }

  /**
   * Get the URL of the page to change a user's password.
   *
   * @param string $id
   *   ID of the user who wants to change his password.
   * @param string $destination
   *   (optional) The URL of the page to redirect the user back to after the password change has been handled on the provider.
   * @return string
   *   The URL of the 'change password' page.
   */
  public function getUrlChangePassword($id, $destination = '') {
    $query = array();

    if (!empty($destination)) {
      $query = array('destination' => $destination);
    }

    return $this->getUrl('auth/changepassword/' . $id, $query);
  }

  /**
   * Get the URL to force a logout of the user on the Culture Feed provider.
   *
   * @param string $destination
   *   (optional) The URL of the page to redirect the user back to after the logout has been handled on the provider.
   * @return string
   *   The URL of the 'logout' page.
   */
  public function getUrlLogout($destination = '') {
    $query = array();

    if (!empty($destination)) {
      $query = array('destination' => $destination);
    }

    return $this->getUrl('auth/logout', $query);
  }

}

/**
 * Class that extends SimpleXMLElement to add some parsing helpers.
 */
class CultureFeedSimpleXMLElement extends SimpleXMLElement {

  /**
   * Runs XPath query on XML data and casts it to a string or an array of string values.
   * @see xpath_cast for more documentation on the arguments.
   */
  public function xpath_str($path, $multiple = FALSE, $trim = TRUE) {
    $tmp = $this->xpath_cast('strval', $path, $multiple);
    if ($tmp && !$multiple && $trim) {
      return trim($tmp);
    }
    return $tmp;
  }

  /**
   * Runs XPath query on XML data and casts it to an integer or an array of integer values.
   * @see xpath_cast for more documentation on the arguments.
   */
  public function xpath_int($path, $multiple = FALSE) {
    return $this->xpath_cast('intval', $path, $multiple);
  }

  /**
   * Runs XPath query on XML data and casts it to a float or an array of float values.
   * @see xpath_cast for more documentation on the arguments.
   */
  public function xpath_float($path, $multiple = FALSE) {
    return $this->xpath_cast('floatval', $path, $multiple);
  }

  /**
   * Runs XPath query on XML data and casts it to a UNIX timestamp or an array of timestamps.
   * @see xpath_cast for more documentation on the arguments.
   */
  public function xpath_time($path, $multiple = FALSE) {
    $val = $this->xpath_cast('strval', $path, $multiple);
    if (!$val) {
      return NULL;
    }

    if ($multiple) {
      foreach ($val as $key => $value) {
        $val[$key] = strtotime($value);
      }
    }

    return strtotime($val);
  }

  /**
   * Runs XPath query on XML data and casts it to a bool or an array of bool values.
   * @see xpath_cast for more documentation on the arguments.
   */
  public function xpath_bool($path, $multiple = FALSE) {
    $val = $this->xpath_cast('strval', $path, $multiple);
    if (!$val) {
      return NULL;
    }

    if ($multiple) {
      foreach ($val as $key => $value) {
        $val[$key] = strtolower($value) == 'true' ? TRUE : FALSE;
      }
    }

    return strtolower($val) == 'true' ? TRUE : FALSE;
  }

  /**
   * Runs XPath query on XML data and casts it using a type casting function.
   *
   * @param string $cast_function
   * @param string $path
   *   The XPath query.
   * @param string $multiple
   *   Does the query direct to a path where multiple values are possible?
   * @return array or undefined
   *   In case $multiple is TRUE, an array is returned with the type casted elements.
   *   In case $multiple is FALSE, the result of the XPath query is casted using the $cast_function and type depends on type of that function.
   *   In case no nodes were found with the query, NULL is returned.
   */
  private function xpath_cast($cast_function, $path, $multiple = FALSE) {
    $objects = $this->xpath($path);

    if (!is_array($objects)) return $objects;

    if ($multiple) {
      $result = array();
      foreach ($objects as $object) {
        $result[] = is_null($object) || ($cast_function != 'strval' && empty($object)) ? NULL : call_user_func($cast_function, $object);
      }
      return array_filter($result);
    }
    else {
      return empty($objects) || is_null($objects[0]) || ($cast_function != 'strval' && empty($objects[0])) ? NULL : call_user_func($cast_function, $objects[0]);
    }
  }

  /**
   * Parse the SimpleXML element as a CultureFeedUser.
   *
   * @return CultureFeedUser
   */
  public function parseUser() {
    $user['id']           = $this->xpath_str('/foaf:person/rdf:id');
    $user['nick']         = $this->xpath_str('/foaf:person/foaf:nick');
    $user['givenName']    = $this->xpath_str('/foaf:person/foaf:givenName');
    $user['familyName']   = $this->xpath_str('/foaf:person/foaf:familyName');
    $user['mbox']         = $this->xpath_str('/foaf:person/mbox');
    $user['mboxVerified'] = $this->xpath_bool('/foaf:person/mboxVerified');
    $user['gender']       = $this->xpath_str('/foaf:person/foaf:gender');
    $user['dob']          = $this->xpath_time('/foaf:person/foaf:dob');
    $user['depiction']    = $this->xpath_str('/foaf:person/foaf:depiction');
    $user['bio']          = $this->xpath_str('/foaf:person/bio');
    $user['homeAddress']  = $this->xpath_str('/foaf:person/foaf:homeAddress');
    $user['status']       = $this->xpath_str('/foaf:person/status');
    if ($user['status']) {
      $user['status'] = strtolower($user['status']);
    }
    $user['openid']       = $this->xpath_str('/foaf:person/foaf:openid');

    $lat = $this->xpath_float('/foaf:person/homeLocation/geo:lat');
    $lng = $this->xpath_float('/foaf:person/homeLocation/geo:long');

    if ($lat && $lng) {
      $user['homeLocation'] = new CultureFeedLocation($lat, $lng);
    }

    $lat = $this->xpath_float('/foaf:person/currentLocation/geo:lat');
    $lng = $this->xpath_float('/foaf:person/currentLocation/geo:long');

    if ($lat && $lng) {
      $user['currentLocation'] = new CultureFeedLocation($lat, $lng);
    }

    $accounts = array();

    $objects = $this->xpath('/foaf:person/foaf:holdsAccount/foaf:onlineAccount');

    foreach ($objects as $object) {
      $account = array();

      $account['accountType']            = $object->xpath_str('accountType');
      $account['accountServiceHomepage'] = $object->xpath_str('foaf:accountServiceHomepage');
      $account['accountName']            = $object->xpath_str('foaf:accountName');
      $account['private']                = $object->xpath_bool('private');
      $account['publishActivities']      = $object->xpath_bool('publishActivities');

      $accounts[] = new CultureFeedOnlineAccount($account);
    }

    if (!empty($accounts)) {
      $user['holdsAccount'] = $accounts;
    }

    return new CultureFeedUser($user);
  }

  /**
   * Parse the SimpleXML element as a CultureFeedResultSet.
   *
   * @return CultureFeedResultSet
   *   CultureFeedResultSet where the objects are of the CultureFeedUser type.
   */
  public function parseUsers() {
    $total = $this->xpath_int('/response/total');

    $users = array();

    $objects = $this->xpath('/response/users/user');

    foreach ($objects as $object) {
      $user = array();

      $user['id']        = $object->xpath_str('rdf:id');
      $user['nick']      = $object->xpath_str('foaf:nick');
      $user['depiction'] = $object->xpath_str('foaf:depiction');

      $users[] = new CultureFeedSearchUser($user);
    }

    return new CultureFeedResultSet($total, $users);
  }

  /**
   * Parse the SimpleXML element as an array of CultureFeedServiceConsumer objects.
   *
   * @return array
   *   Array of CultureFeedServiceConsumer objcts.
   */
  public function parseServiceConsumers() {
    $consumers = array();

    $objects = $this->xpath('/response/serviceconsumers/serviceconsumer');

    foreach ($objects as $object) {
      $consumer = array();

      $consumer['consumerKey']  = $object->xpath_str('consumerKey');
      $consumer['creationDate'] = $object->xpath_time('creationDate');
      $consumer['id']           = $object->xpath_int('id');
      $consumer['name']         = $object->xpath_str('name');
      $consumer['organization'] = $object->xpath_str('organization');

      $consumers[] = new CultureFeedConsumer($consumer);
    }

    return $consumers;
  }

  /**
   * Parse the SimpleXML element as a CultureFeedResultSet.
   *
   * @return CultureFeedResultSet
   *   CultureFeedResultSet where the objects are of the CultureFeedActivity type.
   */
  public function parseActivities() {
    $total = $this->xpath_int('/response/total');

    $activities = array();

    $objects = $this->xpath('/response/activities/activity');

    foreach ($objects as $object) {
      $activity = array();

      $activity['nodeId']       = $object->xpath_str('nodeID');
      $activity['private']      = $object->xpath_str('private');
      $activity['createdVia']   = $object->xpath_str('createdVia');
      $activity['points']       = $object->xpath_str('points');
      $activity['contentType']  = $object->xpath_str('contentType');
      $activity['type']         = $object->xpath_str('type');
      $activity['value']        = $object->xpath_str('value');
      $activity['userId']       = $object->xpath_str('userId');
      $activity['depiction']    = $object->xpath_str('depiction');
      $activity['nick']         = $object->xpath_str('nick');
      $activity['creationDate'] = $object->xpath_time('creationDate');

      $activities[] = new CultureFeedActivity($activity);
    }

    return new CultureFeedResultSet($total, $activities);
  }

  /**
   * Parse the SimpleXML element as an array of CultureFeedServiceConsumer objects.
   *
   * @return array
   *   Array of CultureFeedRecommendation objcts.
   */
  public function parseRecommendations() {
    $recommendations = array();

    $objects = $this->xpath('/response/recommendations/recommendation');

    foreach ($objects as $object) {
      $recommendation = array();

      $recommendation['id']           = $object->xpath_str('id');
      $recommendation['itemid']       = $object->xpath_str('itemid');
      $recommendation['score']        = $object->xpath_float('score');
      $recommendation['algorithm']    = $object->xpath_str('algorithm');
      $recommendation['creationDate'] = $object->xpath_time('creationDate');

      $recommendation_item['id']                = $object->xpath_str('item/id');
      $recommendation_item['permalink']         = $object->xpath_str('item/permalink');
      $recommendation_item['title']             = $object->xpath_str('item/title');
      $recommendation_item['description_short'] = $object->xpath_str('item/description_short');
      $recommendation_item['from']              = $object->xpath_time('item/from');
      $recommendation_item['to']                = $object->xpath_time('item/to');
      $recommendation_item['location_simple']   = $object->xpath_str('item/location_simple');

      $coord = $object->xpath_str('item/location_latlong');
      if ($coord) {
        list($lat, $lng) = explode(',', $coord);
        $recommendation_item['location_latlong'] = new CultureFeedLocation((float)$lat, (float)$lng);
      }

      $recommendation['recommendationItem'] = new CultureFeedRecommendationItem($recommendation_item);

      $recommendations[] = new CultureFeedRecommendation($recommendation);
    }

    return $recommendations;
  }

}

class CultureFeedHTTPException extends Exception {

  public $code;

  public $message;

  public function __construct($code, $message) {
    $this->code = $code;
    $this->message = $message;
  }

}

class CultureFeedException extends Exception {

  public $code;

  public $message;

  public function __construct($code, $message) {
    $this->code = $code;
    $this->message = $message;
  }

}

/**
 * Class to represent a result set (results + total number of results).
 */
class CultureFeedResultSet {

  /**
   * The total number of objects in the complete set.
   *
   * The result set holds a slice of the complete set.
   *
   * @var int
   */
  public $total;

  /**
   * The objects in the slice.
   *
   * @var array
   */
  public $objects;

  /**
   * Constructor for a new CultureFeedResultSet instance.
   *
   * @param integer $total
   *   The total number of objects in the complete set.
   * @param array $objects
   *   The objects in the slice.
   */
  public function __construct($total, $objects) {
    $this->total = $total;
    $this->objects = $objects;
  }

}

/**
 * Class to represent a user as returned in a search operation.
 */
class CultureFeedSearchUser {

  /**
   * ID of the user.
   *
   * @var string
   */
  public $id;

  /**
   * Nick of the user.
   *
   * @var string
   */
  public $nick;

  /**
   * Depiction of the user.
   *
   * @var string
   */
  public $depiction;

  /**
   * Soft value.
   *
   * @var integer
   */
  public $sortValue;

  /**
   * Constructor for a new CultureFeedSearchUser instance.
   *
   * @param array $values
   *   Associative array holding the values for each property.
   *   The array is indexed with the property name.
   */
  public function __construct($values = array()) {
    foreach ($values as $property => $value) {
      if (property_exists('CultureFeedSearchUser', $property)) {
        $this->{$property} = $value;
      }
    }
  }

}

/**
 * Class to represent an online account.
 */
class CultureFeedOnlineAccount {

  /**
   * Type of the online service.
   */
  public $accountType;

  /**
   * Homepage of the online service.
   *
   * @var string
   */
  public $accountServiceHomepage;

  /**
   * Account name on the service.
   *
   * @var string
   */
  public $accountName;

  /**
   * Privacy status of this account. If FALSE a user's activities on this service are in the public data for this user.
   *
   * @var bool
   */
  public $private;

  /**
   * Should activities be pushed through to this service.
   *
   * @var bool
   */
  public $publishActivities;

  /**
   * Constructor for a new CultureFeedOnlineAccount instance.
   *
   * @param array $values
   *   Associative array holding the values for each property.
   *   The array is indexed with the property name.
   */
  public function __construct($values = array()) {
    foreach ($values as $property => $value) {
      if (property_exists('CultureFeedOnlineAccount', $property)) {
        $this->{$property} = $value;
      }
    }
  }

  /**
   * Convert a CultureFeedOnlineAccount object to an array that can be used as data in POST requests that expect online account info.
   *
   * @return array
   *   Associative array representing the object. For documentation of the structure, check the Culture Feed API documentation.
   */
  public function toPostData() {
    // For most properties we can rely on get_object_vars.
    $data = array_filter(get_object_vars($this));

    // Represent private as a string (true/false);
    if (isset($data['private'])) {
      $data['private'] = $data['private'] ? 'true' : 'false';
    }

    // Represent publishActivities as a string (true/false);
    if (isset($data['publishActivities'])) {
      $data['publishActivities'] = $data['publishActivities'] ? 'true' : 'false';
    }

    return $data;
  }

}

/**
 * Class to represent a service consumer.
 */
class CultureFeedConsumer {

  /**
   * Consumer key of this consumer.
   *
   * @var string
   */
  public $consumerKey;

  /**
   * Creation date of this consumer represented as a UNIX timestamp.
   *
   * @var integer
   */
  public $creationDate;

  /**
   * ID of the consumer.
   *
   * @var integer
   */
  public $id;

  /**
   * Name of the consumer.
   *
   * @var string
   */
  public $name;

  /**
   * Organization that owns the consumer.
   *
   * @var string
   */
  public $organization;

  /**
   * Constructor for a new CultureFeedConsumer instance.
   *
   * @param array $values
   *   Associative array holding the values for each property.
   *   The array is indexed with the property name.
   */
  public function __construct($values = array()) {
    foreach ($values as $property => $value) {
      if (property_exists('CultureFeedConsumer', $property)) {
        $this->{$property} = $value;
      }
    }
  }

}

/**
 * Class to represent a recommendation.
 */
class CultureFeedRecommendation {

  /**
   * ID of the recommendation.
   *
   * @var string
   */
  public $id;

  /**
   * ID of the recommendation item (event).
   *
   * @var string
   */
  public $itemid;

  /**
   * Recommendation score.
   *
   * @var string
   */
  public $score;

  /**
   * Algorithm used to generate the recommendation.
   *
   * @var string
   */
  public $algorithm;

  /**
   * The information about the item this recommendation is about.
   *
   * @var CultureFeedRecommendationItem
   */
  public $recommendationItem;

  /**
   * Creation date of this recommendation represented as a UNIX timestamp.
   *
   * @var integer
   */
  public $creationDate;

  /**
   * Constructor for a new CultureFeedRecommendation instance.
   *
   * @param array $values
   *   Associative array holding the values for each property.
   *   The array is indexed with the property name.
   */
  public function __construct($values = array()) {
    foreach ($values as $property => $value) {
      if (property_exists('CultureFeedRecommendation', $property)) {
        $this->{$property} = $value;
      }
    }
  }

}

/**
 * Class to represent a recommendation item (event).
 */
class CultureFeedRecommendationItem {

  /**
   * ID of the recommendation item (CDBID of the event).
   *
   * @var string
   */
  public $id;

  /**
   * Permalink of the recommendation item (event).
   *
   * @var string
   */
  public $permalink;

  /**
   * Title of the recommendation item (event).
   *
   * @var string
   */
  public $title;

  /**
   * Short description of the recommendation item (event).
   *
   * @var string
   */
  public $description_short;

  /**
   * Start date of the recommendation item (event) as a UNIX timestamp.
   *
   * @var integer
   */
  public $from;

  /**
   * End dat of the recommendation item (event) as a UNIX timestamp.
   *
   * @var integer
   */
  public $to;

  /**
   * Address of the recommendation item (event).
   *
   * @var string
   */
  public $location_simple;

  /**
   * Coordinates of the recommendation item (event).
   *
   * @var CultureFeedLocation
   */
  public $location_latlong;

  /**
   * Constructor for a new CultureFeedRecommendationItem instance.
   *
   * @param array $values
   *   Associative array holding the values for each property.
   *   The array is indexed with the property name.
   */
  public function __construct($values = array()) {
    foreach ($values as $property => $value) {
      if (property_exists('CultureFeedRecommendationItem', $property)) {
        $this->{$property} = $value;
      }
    }
  }

}

/**
 * Class to represent a user.
 */
class CultureFeedUser {

  /**
   * Gender 'male'.
   */
  const GENDER_MALE = 'male';

  /**
   * Gender 'female'.
   */
  const GENDER_FEMALE = 'female';

  /**
   * User status 'public'.
   */
  const STATUS_PUBLIC = 'public';

  /**
   * User status 'private'.
   */
  const STATUS_PRIVATE = 'private';

  /**
   * User status 'blocked'.
   */
  const STATUS_BLOCKED = 'blocked';

  /**
   * User status 'deleted'.
   */
  const STATUS_DELETED = 'deleted';

  /**
   * ID of the user.
   *
   * @var string
   */
  public $id;

  /**
   * Nick of the user.
   *
   * @var string
   */
  public $nick;

  /**
   * Password of the user.
   *
   * @var string
   */
  public $password;

  /**
   * First name of the user.
   *
   * @var string
   */
  public $givenName;

  /**
   * Family name of the user.
   *
   * @var string
   */
  public $familyName;

  /**
   * E-mail of the user.
   *
   * @var string
   */
  public $mbox;

  /**
   * E-mail verification status.
   *
   * @var bool
   */
  public $mboxVerified;

  /**
   * Gender of the user.
   * Possible values are represented in the GENDER_* constants.
   *
   * @var string
   */
  public $gender;

  /**
   * Date of birth of the user represented as a UNIX timestamp.
   *
   * @var integer
   */
  public $dob;

  /**
   * Depiction of the user.
   *
   * @var string
   */
  public $depiction;

  /**
   * Biography of the user.
   *
   * @var string
   */
  public $bio;

  /**
   * Home address of the user.
   *
   * @var string
   */
  public $homeAddress;

  /**
   * Coordinates of the user's home address.
   *
   * @var CultureFeedLocation
   */
  public $homeLocation;

  /**
   * Coordinates of the user's current address.
   *
   * @var CultureFeedLocation
   */
  public $currentLocation;

  /**
   * Status of the user.
   * Possible values are represented in the STATUS_* constants.
   *
   * @var string
   */
  public $status;

  /**
   * OpenID handle of the user.
   *
   * @var string
   */
  public $openid;

  /**
   * Online accounts (social services) the user is connected with.
   * Represented as an array of CultureFeedOnlineAccount objects.
   *
   * @var array
   */
  public $holdsAccount;

  /**
   * Constructor for a new CultureFeedUser instance.
   *
   * @param array $values
   *   Associative array holding the values for each property.
   *   The array is indexed with the property name.
   */
  public function __construct($values = array()) {
    foreach ($values as $property => $value) {
      if (property_exists('CultureFeedUser', $property)) {
        $this->{$property} = $value;
      }
    }
  }

  /**
   * Convert a CultureFeedUser object to an array that can be used as data in POST requests that expect user info.
   *
   * @return array
   *   Associative array representing the object. For documentation of the structure, check the Culture Feed API documentation.
   */
  public function toPostData() {
    // For most properties we can rely on get_object_vars.
    $data = array_filter(get_object_vars($this));

    // Represent mboxVerified as a string (true/false);
    if (isset($data['mboxVerified'])) {
      $data['mboxVerified'] = $data['mboxVerified'] ? 'true' : 'false';
    }

    // Represent homeLocation as a string.
    if (isset($data['homeLocation'])) {
      $data['homeLocation'] = (string)$data['homeLocation'];
    }

    // Represent currentLocation as a string.
    if (isset($data['currentLocation'])) {
      $data['currentLocation'] = (string)$data['homeLocation'];
    }

    // Represent dob as a W3C date.
    if (isset($data['dob'])) {
      $data['dob'] = date('c', $data['dob']);
    }

    return $data;
  }

}

/**
 * Class to represent an activity.
 */
class CultureFeedActivity {

  /**
   * Content type 'page'.
   */
  const CONTENT_TYPE_PAGE = 'page';

  /**
   * Content type 'event'.
   */
  const CONTENT_TYPE_EVENT = 'event';

  /**
   * Content type 'actor'.
   */
  const CONTENT_TYPE_ACTOR = 'actor';

  /**
   * Consumer type that indicates the action "Viewed".
   */
  const TYPE_VIEW = 1;

  /**
   * Consumer type that indicates the action "Detail viewed".
   */
  const TYPE_DETAIL = 2;

  /**
   * Consumer type that indicates the action "I like this".
   */
  const TYPE_LIKE = 3;

  /**
   * Consumer type that indicates the action "I forwarded this via mail".
   */
  const TYPE_MAIL = 4;

  /**
   * Consumer type that indicates the action "Printed".
   */
  const TYPE_PRINT = 5;

  /**
   * Consumer type that indicates the action "Shared on Facebook".
   */
  const TYPE_FACEBOOK = 6;

  /**
   * Consumer type that indicates the action "Shared on Twitter".
   */
  const TYPE_TWITTER = 7;

  /**
   * Consumer type that indicates the action "I went to this event".
   */
  const TYPE_IK_GA = 8;

  /**
   * NodeId of the activity object.
   *
   * @var string
   */
  public $nodeId;

  /**
   * Privacy status of the activity.
   *
   * @var bool
   */
  public $private;

  /**
   * The service consumer id of the consumer where the activity was generated.
   *
   * @var string
   */
  public $createdVia;

  /**
   * The points for this activity.
   *
   * @var int
   */
  public $points;

  /**
   * The type of content this activity handles.
   * Possible values are represented in the CONTENT_TYPE_* constants.
   *
   * @var string
   */
  public $contentType;

  /**
   * The consumption type of this activity.
   * Possible values are represented in the TYPE_* constants.
   *
   * @var string
   */
  public $type;

  /**
   * Value for this activity.
   *
   * @var string
   */
  public $value;

  /**
   * ID of the user who generated this activity.
   *
   * @var string
   */
  public $userId;

  /**
   * Depiction of the user who generated this activity.
   *
   * @var string
   */
  public $depiction;

  /**
   * Nick of the user who generated this activity.
   *
   * @var string
   */
  public $nick;

  /**
   * Creation date of this activity represented as a UNIX timestamp.
   *
   * @var integer
   */
  public $creationDate;

  /**
   * Constructor for a new CultureFeedActivity instance.
   *
   * @param array $values
   *   Associative array holding the values for each property.
   *   The array is indexed with the property name.
   */
  public function __construct($values = array()) {
    foreach ($values as $property => $value) {
      if (property_exists('CultureFeedActivity', $property)) {
        $this->{$property} = $value;
      }
    }
  }

  /**
   * Convert a CultureFeedActivity object to an array that can be used as data in POST requests that expect user info.
   *
   * @return array
   *   Associative array representing the object. For documentation of the structure, check the Culture Feed API documentation.
   */
  public function toPostData() {
    // For most properties we can rely on get_object_vars.
    $data = array_filter(get_object_vars($this));

    // Represent private as a string (true/false);
    if (isset($data['private'])) {
      $data['private'] = $data['private'] ? 'true' : 'false';
    }

    // Represent creationDate as a W3C date.
    if (isset($data['creationDate'])) {
      $data['creationDate'] = date('c', $data['creationDate']);
    }

    return $data;
  }

}

/**
 * Class to represent a location's coordinates (latitude and longitude).
 */
class CultureFeedLocation {

  /**
   * Latitude.
   *
   * @var float
   */
  public $lat;

  /**
   * Longitude
   *
   * @var float
   */
  public $lng;

  /**
   * Constructor for a new CultureFeedLocation instance.
   *
   * @param float $lat
   *   Latitude.
   * @param float $lng
   *   Longitude.
   */
  public function __construct($lat, $lng) {
    $this->lat = $lat;
    $this->lng = $lng;
  }

  /**
   * Implements PHP magic __toString method to convert the CultureFeedLocation to a string.
   *
   * @return string
   *   The string representation of the location in lat;lng format.
   */
  public function __toString() {
    return sprintf('%f;%f', $this->lat, $this->lng);
  }

}

/**
 * Class to represent a user search query.
 */
class CultureFeedSearchUsersQuery {

  /**
   * Sort option 'creationDate'.
   */
  const SORT_CREATIONDATE = 'creationDate';

  /**
   * Sort option 'userId'.
   */
  const SORT_USERID = 'userId';

  /**
   * Sort option 'firstName'.
   */
  const SORT_FIRSTNAME = 'firstName';

  /**
   * Sort option 'lastName'.
   */
  const SORT_LASTNAME = 'lastName';

  /**
   * Sort option 'lastLoginDate'.
   */
  const SORT_LASTLOGINDATE = 'lastLoginDate';

  /**
   * Sort option 'lastActivityDate'.
   */
  const SORT_LASTACTIVITYDATE = 'lastActivityDate';

  /**
   * Sort option 'numberOfActivities'.
   */
  const SORT_NUMBEROFACTIVITIES = 'numberOfActivities';

  /**
   * Sort option 'numberOfLikes'.
   */
  const SORT_NUMBEROFLIKES = 'numberOfLikes';

  /**
   * Sort option 'numberOfSharesFacebook'.
   */
  const SORT_NUMBEROFSHARESFACEBOOK = 'numberOfSharesFacebook';

  /**
   * Sort option 'numberOfSharesTwitter'.
   */
  const SORT_NUMBEROFSHARESTWITTER = 'numberOfSharesTwitter';

  /**
   * Sort option 'numberOfAttends'.
   */
  const SORT_NUMBEROFATTENDS = 'numberOfAttends';

  /**
   * Sort option 'numberOfActiveActivities'.
   */
  const SORT_NUMBEROFACTIVEACTIVITIES = 'numberOfActiveActivities';

  /**
   * Sort option 'numberOfActiveActivities'.
   */
  const SORT_ORDER_ASCENDING= 'ascending';

  /**
   * Sort option 'descending'.
   */
  const SORT_ORDER_DESCENDING = 'descending';

  /**
   * ID of the user.
   *
   * @var string
   */
  public $userId;

  /**
   * Nick of the user.
   *
   * @var string
   */
  public $nick;

  /**
   * First name (givenName), last name and/or nick.
   *
   * @var string
   */
  public $name;

  /**
   * E-mail address.
   *
   * @var string
   */
  public $mbox;

  /**
   * Gender.
   * Possible values are represented in the CultureFeedUser::GENDER_* constants.
   *
   * @var string
   */
  public $gender;

  /**
   * Date of birth represented as a UNIX timestamp.
   *
   * @var integer
   */
  public $dob;

  /**
   * Minimum date of birth represented as a UNIX timestamp.
   *
   * @var integer
   */
  public $dobMin;

  /**
   * Maximum date of birth represented as a UNIX timestamp.
   *
   * @var integer
   */
  public $dobMax;

  /**
   * Street of the home address.
   *
   * @var string
   */
  public $homeStreet;

  /**
   * ZIP of the home address.
   *
   * @var string
   */
  public $homeZip;

  /**
   * City of the home address.
   *
   * @var string
   */
  public $homeCity;

  /**
   * Country of the home address.
   *
   * @var string
   */
  public $homeCountry;

  /**
   * Coordinates of the home address.
   *
   * @var CultureFeedLocation
   */
  public $homeLocation;

  /**
   * Coordinates of the current location.
   *
   * @var CultureFeedLocation
   */
  public $currentLocation;

  /**
   * User status.
   * Possible values are represented in the CultureFeedUser::STATUS_* constants.
   *
   * @var string
   */
  public $status;

  /**
   * User points.
   *
   * @var integer
   */
  public $points;

  /**
   * Minimum user points.
   *
   * @var integer
   */
  public $pointsMin;

  /**
   * Maximum user points.
   *
   * @var integer
   */
  public $pointsMax;

  /**
   * User likes.
   *
   * @var integer
   */
  public $likes;

  /**
   * Minimum user likes.
   *
   * @var integer
   */
  public $likesMin;

  /**
   * Maximum user likes.
   *
   * @var integer
   */
  public $likesMax;

  /**
   * The service consumer id of the consumer where the activity was generated.
   *
   * @var string
   */
  public $createdBy;

  /**
   * Last login time represented as a UNIX timestamp.
   *
   * @var integer
   */
  public $lastLogin;

  /**
   * Minimum last login time represented as a UNIX timestamp.
   *
   * @var integer
   */
  public $lastLoginMin;

  /**
   * Maximum last login time represented as a UNIX timestamp.
   *
   * @var integer
   */
  public $lastLoginMax;

  /**
   * Start position.
   *
   * @var integer
   */
  public $start;

  /**
   * Maximum number of items to return.
   *
   * @var integer
   */
  public $max;

  /**
   * Sort field.
   * Possible values are represented in the SORT_* constants.
   *
   * @var string
   */
  public $sort;

  /**
   * Sort order.
   * Possible values are represented in the SORT_ORDER_* constants.
   *
   * @var string
   */
  public $order;

  /**
   * Constructor for a new CultureFeedSearchUsersQuery instance.
   *
   * @param array $values
   *   Associative array holding the values for each property.
   *   The array is indexed with the property name.
   */
  public function __construct($values = array()) {
    foreach ($values as $property => $value) {
      if (property_exists('CultureFeedSearchUsersQuery', $property)) {
        $this->{$property} = $value;
      }
    }
  }

  /**
   * Convert a CultureFeedSearchUsersQuery object to an array that can be used as data in POST requests that expect search user query info.
   *
   * @return array
   *   Associative array representing the object. For documentation of the structure, check the Culture Feed API documentation.
   */
  public function toPostData() {
    // For most properties we can rely on get_object_vars.
    $data = array_filter(get_object_vars($this));

    // Represent homeLocation as a string.
    if (isset($data['homeLocation'])) {
      $data['homeLocation'] = (string)$data['homeLocation'];
    }

    // Represent currentLocation as a string.
    if (isset($data['currentLocation'])) {
      $data['currentLocation'] = (string)$data['currentLocation'];
    }

    // Represent dob as a W3C date.
    if (isset($data['dob'])) {
      $data['dob'] = date('c', $data['dob']);
    }

    // Represent dobMin as a W3C date.
    if (isset($data['dobMin'])) {
      $data['dobMin'] = date('c', $data['dobMin']);
    }

    // Represent dobMax as a W3C date.
    if (isset($data['dobMax'])) {
      $data['dobMax'] = date('c', $data['dobMax']);
    }

    // Represent lastLogin as a W3C date.
    if (isset($data['lastLogin'])) {
      $data['lastLogin'] = date('c', $data['lastLogin']);
    }

    // Represent lastLoginMin as a W3C date.
    if (isset($data['lastLoginMin'])) {
      $data['lastLoginMin'] = date('c', $data['lastLoginMin']);
    }

    // Represent lastLoginMax as a W3C date.
    if (isset($data['lastLoginMax'])) {
      $data['lastLoginMax'] = date('c', $data['lastLoginMax']);
    }

    return $data;
  }

}

/**
 * Class to represent a user privacy config.
 */
class CultureFeedUserPrivacyConfig {

  /**
   * Privacy status 'public'.
   */
  const PRIVACY_PUBLIC = 'public';

  /**
   * Privacy status 'private'.
   */
  const PRIVACY_PRIVATE = 'private';

  /**
   * Privacy status of the user's nick.
   * Possible values are represented in the PUBLIC_* constants.
   *
   * @var string
   */
  public $nick;

  /**
   * Privacy status of the user's givenName.
   * Possible values are represented in the PUBLIC_* constants.
   *
   * @var string
   */
  public $givenName;

  /**
   * Privacy status of the user's familyName.
   * Possible values are represented in the PUBLIC_* constants.
   *
   * @var string
   */
  public $familyName;

  /**
   * Privacy status of the user's mbox.
   * Possible values are represented in the PUBLIC_* constants.
   *
   * @var string
   */
  public $mbox;

  /**
   * gender.
   * Possible values are represented in the PUBLIC_* constants.
   *
   * @var string
   */
  public $gender;

  /**
   * Privacy status of the user's dob.
   * Possible values are represented in the PUBLIC_* constants.
   *
   * @var string
   */
  public $dob;

  /**
   * Privacy status of the user's depiction.
   * Possible values are represented in the PUBLIC_* constants.
   *
   * @var string
   */
  public $depiction;

  /**
   * Privacy status of the user's bio.
   * Possible values are represented in the PUBLIC_* constants.
   *
   * @var string
   */
  public $bio;

  /**
   * Privacy status of the user's homeAddress.
   * Possible values are represented in the PUBLIC_* constants.
   *
   * @var string
   */
  public $homeAddress;

  /**
   * Privacy status of the user's homeLocation.
   * Possible values are represented in the PUBLIC_* constants.
   *
   * @var string
   */
  public $homeLocation;

  /**
   * Privacy status of the user's openId.
   * Possible values are represented in the PUBLIC_* constants.
   *
   * @var string
   */
  public $openId;

  /**
   * Constructor for a new CultureFeedUserPrivacyConfig instance.
   *
   * @param array $values
   *   Associative array holding the values for each property.
   *   The array is indexed with the property name.
   */
  public function __construct($values = array()) {
    foreach ($values as $property => $value) {
      if (property_exists('CultureFeedUserPrivacyConfig', $property)) {
        $this->{$property} = $value;
      }
    }
  }

  /**
   * Convert a CultureFeedUserPrivacyConfig object to an array that can be used as data in POST requests that expect search user query info.
   *
   * @return array
   *   Associative array representing the object. For documentation of the structure, check the Culture Feed API documentation.
   */
  public function toPostData() {
    // For most properties we can rely on get_object_vars.
    $data = array_filter(get_object_vars($this));

    return $data;
  }

}

/**
 * Class to represent an activities search query.
 */
class CultureFeedSearchActivitiesQuery {

  /**
   * The type of content the activity handles.
   * Possible values are represented in the CultureFeedActivity::CONTENT_TYPE_* constants.
   *
   * @var string
   */
  public $contentType;

  /**
   * The consumption type of the activity.
   * Possible values are represented in the CultureFeedActivity::TYPE_* constants.
   *
   * @var integer
   */
  public $type;

  /**
   * NodeId.
   *
   * @var string
   */
  public $nodeId;

  /**
   * UserId.
   *
   * @var string
   */
  public $userId;

  /**
   * Consumer.
   *
   * @var string
   */
  public $consumer;

  /**
   * ActivityId.
   *
   * @var string
   */
  public $activityId;

  /**
   * Include private activities?
   *
   * @var bool
   */
  public $private;

  /**
   * Start position.
   *
   * @var integer
   */
  public $start;

  /**
   * Maximum number of results to return.
   *
   * @var integer
   */
  public $max;

  /**
   * Constructor for a new CultureFeedSearchActivitiesQuery instance.
   *
   * @param array $values
   *   Associative array holding the values for each property.
   *   The array is indexed with the property name.
   */
  public function __construct($values = array()) {
    foreach ($values as $property => $value) {
      if (property_exists('CultureFeedSearchActivitiesQuery', $property)) {
        $this->{$property} = $value;
      }
    }
  }

  /**
   * Convert a CultureFeedSearchActivitiesQuery object to an array that can be used as data in POST requests that expect search user query info.
   *
   * @return array
   *   Associative array representing the object. For documentation of the structure, check the Culture Feed API documentation.
   */
  public function toPostData() {
    // For most properties we can rely on get_object_vars.
    $data = array_filter(get_object_vars($this));

    return $data;
  }

}