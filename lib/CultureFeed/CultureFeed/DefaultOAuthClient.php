<?php

/**
 * Class to represent a OAuth request.
 */
class CultureFeed_DefaultOAuthClient implements CultureFeed_OAuthClient {

  /**
   * HTTP Request object to make the requests.
   *
   * @var CultureFeed_HttpClient
   */
  protected $http_client;

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
   * Constructor for a new CultureFeed_OAuthClient instance.
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
   * Set the HTTP request object.
   *
   * @param CultureFeed_HttpClient $http_client
   */
  public function setHttpClient(CultureFeed_HttpClient $http_client) {
    $this->http_client = $http_client;
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
  public function consumerGet($path, array $params = array(), $format = '') {
    return $this->request($path, $params, 'GET', FALSE, $format);
  }

  /**
   * Do a GET request with only a consumer token and return the response as XML.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function consumerGetAsXml($path, array $params = array()) {
    return $this->consumerGet($path, $params, 'xml');
  }

  /**
   * Do a GET request with only a consumer token and return the response as JSON.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function consumerGetAsJson($path, array $params = array()) {
    return $this->consumerGet($path, $params, 'json');
  }

  /**
   * Do a POST request with only a consumer token.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function consumerPost($path, array $params = array(), $raw_post = TRUE, $has_file_upload = FALSE, $format = '') {
    return $this->request($path, $params, 'POST', FALSE, $format, $raw_post, $has_file_upload = FALSE);
  }

  /**
   * Do a POST request with only a consumer token and return the response as XML.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function consumerPostAsXml($path, array $params = array(), $raw_post = TRUE, $has_file_upload = FALSE) {
    return $this->consumerPost($path, $params, $raw_post, $has_file_upload, 'xml');
  }

  /**
   * Do a POST request with only a consumer token and return the response as JSON.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function consumerPostAsJson($path, array $params = array(), $raw_post = TRUE, $has_file_upload = FALSE) {
    return $this->consumerPost($path, $params, $raw_post, $has_file_upload, 'json');
  }

  /**
   * Do a GET request with a consumer token and user token.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function authenticatedGet($path, array $params = array(), $format = '') {
    return $this->request($path, $params, 'GET', TRUE, $format);
  }

  /**
   * Do a GET request with a consumer token and user token and return the response as XML.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function authenticatedGetAsXml($path, array $params = array()) {
    return $this->authenticatedGet($path, $params, 'xml');
  }

  /**
   * Do a GET request with a consumer token and user token and return the response as JSON.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function authenticatedGetAsJson($path, array $params = array()) {
    return $this->authenticatedGet($path, $params, 'json');
  }

  /**
   * Do a POST request with a consumer token and user token.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function authenticatedPost($path, array $params = array(), $raw_post = TRUE, $has_file_upload = FALSE, $format = '') {
    return $this->request($path, $params, 'POST', TRUE, $format, $raw_post, $has_file_upload);
  }

  /**
   * Do a POST request with a consumer token and user token and return the response as XML.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function authenticatedPostAsXml($path, array $params = array(), $raw_post = TRUE, $has_file_upload = FALSE) {
    return $this->authenticatedPost($path, $params, $raw_post, $has_file_upload, 'xml');
  }

  /**
   * Do a POST request with a consumer token and user token and return the response as JSON.
   *
   * Wrapper function around request. @see request for documentation of remaining parameters.
   */
  public function authenticatedPostAsJson($path, array $params = array(), $raw_post = TRUE, $has_file_upload = FALSE) {
    return $this->authenticatedPost($path, $params, $raw_post, $has_file_upload, 'json');
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
   * @return CultureFeed_HTTPResponse
   *   The response.
   *
   * @throws Exception
   *   If an $use_auth is TRUE and no token was set.
   * @throws CultureFeed_Exception
   *   If an error message and code could be parsed from the response.
   * @throws CultureFeed_HTTPException
   *   If no error message and code could be parsed from the response.
   */
  public function request($path, array $params = array(), $method = 'GET', $use_auth = TRUE, $format = 'xml', $raw_post = TRUE, $has_file_upload = FALSE) {
    if ($use_auth && !isset($this->token->key)) {
      throw new Exception('Trying to do an authorized request without an access token set.');
    }

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
    $post_data = $has_file_upload ? $params : self::build_query($params);
print $post_data;
    // Necessary to support token setup calls.
    if (!$raw_post) {
      $post_data = $request->to_postdata();
    }

    // If no HTTP client was set, create one.
    if (!isset($this->http_client)) {
      $this->http_client = new CultureFeed_DefaultHttpClient();
    }

    // Do the request.
    $response = $this->http_client->request($url, $http_headers, $method, $post_data);

    // In case the HTTP response status is not 200, we consider this an error.
    // In case we can parse a code and message from the response, we throw a CultureFeed_Exception.
    // In case we can't parse a code and message, we throw a CultureFeed_HTTPException.
    if ($response->code != 200) {
      try {
        $xml = new CultureFeed_SimpleXMLElement($response);
      }
      catch (Exception $e) {
        throw new CultureFeed_HttpException($response->response, $response->code);
      }

      if ($code = $xml->xpath_str('/response/code')) {
        $message = $xml->xpath_str('/response/message');
        throw new CultureFeed_Exception($message, $code);
      }

      throw new CultureFeed_HttpException($response->response, $response->code);
    }

    // In case the HTTP response status is 200, we return the response.
    return $response->response;
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
  public function getUrl($path, array $query = array()) {
    $url =  rtrim($this->endpoint, '/') . '/' . trim($path, '/');

    if (!empty($query)) {
      $url .= '?' . self::build_query($query);
    }

    return $url;
  }
  
  /**
   * Build a querystring.
   *
   * @param string $params
   *   Array representation of the querystring.
   * @return string
   *   The querystring.
   */  
  private static function build_query($params) {
    $parts = array();

    foreach ($params as $key => $value) {
      if (is_array($value)) {
        foreach ($value as $value_part) {
          $parts[] = urlencode($key) . '=' . urlencode($value_part);
        }
      }
      else {
        $parts[] = urlencode($key) . '=' . urlencode($value);
      }
    }
    
    return implode('&', $parts);
  }

}