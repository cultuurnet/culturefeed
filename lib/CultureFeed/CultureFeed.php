<?php

/**
 * @file
 * Classes to work with CultuurNet's Culture Feed API.
 */

/**
 * The main class to communicate with the Culture Feed API.
 *
 * Provides calls and helper functions to set up OAuth authorization.
 * Provides calls to fetch objects (users, activities, ...) from the API.
 */
class CultureFeed {

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
   * Top events sort option 'by number of likes'.
   */
  const TOP_EVENTS_SORT_LIKE = 'like';

  /**
   * Top events sort option 'by number of Facebook shares'.
   */
  const TOP_EVENTS_SORT_FACEBOOK = 'facebook';

  /**
   * Top events sort option 'by number of Twitter shares (tweets)'.
   */
  const TOP_EVENTS_SORT_TWITTER = 'twitter';

  /**
   * Top events sort option 'by number of attends'.
   */
  const TOP_EVENTS_SORT_ATTEND = 'attend';

  /**
   * Top events sort option 'by aggregate of all other options'.
   */
  const TOP_EVENTS_SORT_ACTIVE = 'active';

  /**
   * Authorization screen option 'regular'.
   */
  const AUTHORIZE_TYPE_REGULAR = 'regular';

  /**
   * Authorization screen option 'register'.
   */
  const AUTHORIZE_TYPE_REGISTER = 'register';

  /**
   * Authorization screen option 'force login'.
   */
  const AUTHORIZE_TYPE_FORCELOGIN = 'forcelogin';

  /**
   * OAuth request object to do the request.
   *
   * @var CultureFeed_OAuthClient
   */
  protected $oauth_client;

  /**
   * Constructor for a new CultureFeed instance.
   *
   * @param CultureFeed_OAuthClient $oauth_client
   *   A OAuth client to make requests.
   */
  public function __construct(CultureFeed_OAuthClient $oauth_client) {
    $this->oauth_client = $oauth_client;
  }

  /**
   * Fetch a request token. Sets the token for this object with the fetched token.
   *
   * The object should be initialized with the consumer token.
   *
   * @return array
   *   An associative array containing the token, secret and callback confirmed status.
   *   Array keys are 'oauth_callback_confirmed', 'oauth_token' and 'oauth_token_secret'.
   *
   * @throws CultureFeed_ParseException
   *   If the result could not be parsed.
   */
  public function getRequestToken() {
    $response = $this->oauth_client->consumerPost('requestToken', array(), FALSE);

    $token = OAuthUtil::parse_parameters($response);

    if (!isset($token['oauth_token']) || !isset($token['oauth_token'])) {
      throw new CultureFeed_ParseException($response, 'token');
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
   * @param string $type
   *   (optional) Type of CultureFeed screen: regular, register or force login (even when user was logged in.
   *   Possible values for the privacy status are represented in the AUTHORIZE_TYPE_* constants.
   * @return string
   *   The URL of the authorization page.
   */
  public function getUrlAuthorize($token, $callback = '', $type = CultureFeed::AUTHORIZE_TYPE_REGULAR) {
    $query = array('oauth_token' => $token['oauth_token']);

    if (!empty($callback)) {
      $query['oauth_callback'] = $callback;
    }

    if (!empty($type)) {
      $query['type'] = $type;
    }

    return $this->oauth_client->getUrl('auth/authorize', $query);
  }

  /**
   * Fetch an access token. Sets the token for this object with the fetched token.
   *
   * The object should be initialized with the consumer token and user access token.
   *
   * @param string $oauth_verifier
   *   The 'oauth_verifier' that was sent back in the callback URL after the authorization step.
   * @return array
   *   An associative array containing the token, secret and callback confirmed status.
   *   Array keys are 'oauth_callback_confirmed', 'oauth_token' and 'oauth_token_secret'.
   *
   * @throws CultureFeed_ParseException
   *   If the result could not be parsed.
   */
  public function getAccessToken($oauth_verifier) {
    $response = $this->oauth_client->authenticatedPost('accessToken', array('oauth_verifier' => $oauth_verifier));

    $token = OAuthUtil::parse_parameters($response);

    if (!isset($token['oauth_token']) || !isset($token['oauth_token'])) {
      throw new CultureFeed_ParseException($response, 'token');
    }

    $this->token = new OAuthConsumer($token['oauth_token'], $token['oauth_token_secret']);

    return $token;
  }

  /**
   * Create a new user.
   *
   * The object should be initialized with the consumer token and the user access token of a user who has permission to create users.
   *
   * @param CultureFeed_User $user
   *   The user to create.
   * @return string
   *   The id of the newly created user.
   *
   * @throws CultureFeed_ParseException
   *   If the result could not be parsed.
   */
  public function createUser(CultureFeed_User $user) {
    $data = $user->toPostData();

    $result = $this->oauth_client->consumerPostAsXml('user', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    if ($uid = $xml->xpath_str('/response/uid')) {
      return $uid;
    }

    throw new CultureFeed_ParseException($result);
  }

  /**
   * Update an existing user.
   *
   * The object should be initialized with the consumer token and user access token of the user who is acted upon.
   *
   * @param CultureFeed_User $user
   *   The user to update. The user is identified by ID. Only fields that are set will be updated.
   */
  public function updateUser(CultureFeed_User $user) {
    $data = $user->toPostData();

    $id = $data['id'];

    unset($data['id']);

    $this->oauth_client->authenticatedPostAsXml('user/' . $id, $data);
  }

  /**
   * Delete a user.
   *
   * The object should be initialized with the consumer token and user access token of the user who is acted upon.
   *
   * @param string $id
   *   ID of the user who is deleted.
   */
  public function deleteUser($id) {
    $this->oauth_client->authenticatedGetAsXml('user/' . $id . '/delete');
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
   * @return CultureFeed_User
   *   The requested user.
   *
   * @throws CultureFeed_ParseException
   *   If the result could not be parsed.
   */
  public function getUser($id, $private = FALSE, $use_auth = TRUE) {
    $query = array('private' => $private ? 'true' : 'false');

    if ($use_auth) {
      $result = $this->oauth_client->authenticatedGetAsXml('user/' . $id, $query);
    }
    else {
      $result = $this->oauth_client->consumerGetAsXml('user/' . $id, $query);
    }

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    return $xml->parseUser($xml);
  }

  /**
   * Search for users.
   *
   * The object should be initialized with the consumer token.
   *
   * @param CultureFeed_SearchUsersQuery $query
   *   The search query.
   * @return CultureFeed_ResultSet
   *   The users.
   *
   * @throws CultureFeed_ParseException
   *   If the result could not be parsed.
   */
  public function searchUsers(CultureFeed_SearchUsersQuery $query) {
    $data = $query->toPostData();

    $result = $this->oauth_client->consumerGetAsXml('user/search', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
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
   * @return CultureFeed_ResultSet
   *   The users.
   *
   * @throws CultureFeed_ParseException
   *   If the result could not be parsed.
   */
  public function getSimilarUsers($id) {
    $result = $this->oauth_client->consumerGetAsXml('user/' . $id . '/similar', array());

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
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
   */
  public function uploadUserDepiction($id, $file_data) {
    $this->oauth_client->authenticatedPostAsXml('user/' . $id . '/upload_depiction', array('depiction' => $file_data), TRUE, TRUE);
  }

  /**
   * Resend the e-mail confirmation mail for a user.
   *
   * The object should be initialized with the consumer token and user access token of the user who is acted upon.
   *
   * @param string $id
   *   ID of the user who requests the confirmation e-mail to be resent.
   */
  public function resendMboxConfirmationForUser($id) {
    $this->oauth_client->authenticatedPostAsXml('user/' . $id . '/resend_mbox_confirmation');
  }

  /**
   * Update a user's field privacy settings.
   *
   * The object should be initialized with the consumer token and user access token of the user who is acted upon.
   *
   * @param string $id
   *   ID of the user whose privacy settings to update.
   * @param CultureFeed_UserPrivacyConfig $privacy_config
   *   An associative array representing the privacy status for each field.
   *   The array is indexed by the field name. Values are the privacy status.
   *   Possible values for the privacy status are represented in the PRIVACY_* constants.
   */
  public function updateUserPrivacy($id, CultureFeed_UserPrivacyConfig $privacy_config) {
    $data = $privacy_config->toPostData();

    $this->oauth_client->authenticatedPostAsXml('user/' . $id . '/privacy', $data);
  }

  /**
   * Fetch a list of service consumers the user is connected with.
   *
   * The object should be initialized with the consumer token and user access token of the user who is acted upon.
   *
   * @param string $id
   *   ID of the user to fetch service consumers for.
   * @return CultureFeed_Consumer[]
   *   The service consumers
   *
   * @throws CultureFeed_ParseException
   *   If the result could not be parsed.
   */
  public function getUserServiceConsumers($id) {
    $result = $this->oauth_client->authenticatedGetAsXml('user/' . $id . '/serviceconsumers');

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
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
   */
  public function revokeUserServiceConsumer($user_id, $consumer_id) {
    $this->oauth_client->authenticatedPostAsXml('user/' . $user_id . '/serviceconsumers/' . $consumer_id . '/revoke');
  }

  /**
   * Update a user's online account settings.
   *
   * The object should be initialized with the consumer token and user access token of the user who is acted upon.
   *
   * @param string $id
   *   ID of the user whose online account settings will be updated.
   * @param CultureFeed_OnlineAccount $account
   *   The account settings to update.
   */
  public function updateUserOnlineAccount($id, CultureFeed_OnlineAccount $account) {
    $data = $account->toPostData();

    $this->oauth_client->authenticatedPostAsXml('user/' . $id . '/onlineaccount/update', $data);
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
   */
  public function deleteUserOnlineAccount($id, $account_type, $account_name) {
    $data = array(
      'accountName' => $account_name,
      'accountType' => $account_type,
    );

    $this->oauth_client->authenticatedPostAsXml('user/' . $id . '/onlineaccount/delete', $data);
  }

  /**
   * Create a new activity.
   *
   * The object should be initialized with the consumer token and user access token of the user who is acted upon.
   *
   * @param CultureFeed_Activity $activity
   *   The activity to create.
   *
   * @throws CultureFeed_ParseException
   *   If the result could not be parsed.
   */
  public function createActivity(CultureFeed_Activity $activity) {
    $data = $activity->toPostData();

    $result = $this->oauth_client->authenticatedPostAsXml('activity', $data);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
    }

    if ($id = $xml->xpath_str('/response/activityId')) {
      return $id;
    }

    throw new CultureFeed_ParseException($result);
  }

  /**
   * Update an existing user.
   *
   * The object should be initialized with the consumer token and user access token of the user who is acted upon.
   *
   * @param string $id
   *   ID of the activity to update.
   */
  public function updateActivity($id, $private) {
    $data = array('private' => $private ? 'true' : 'false');

    $this->oauth_client->authenticatedPostAsXml('activity/' . $id, $data);
  }

  /**
   * Delete an activity.
   *
   * The object should be initialized with the consumer token and user access token of the user who is acted upon.
   *
   * @param string $id
   *   ID of the activity that is deleted.
   */
  public function deleteActivity($id) {
    $this->oauth_client->authenticatedGetAsXml('activity/' . $id . '/delete');
  }

  /**
   * Search for activities.
   *
   * In case the query contains 'private=true', the object should be initialized with the consumer token and user access token of the user who is acted upon.
   * Else the object should be initialized with the consumer token.
   *
   * @param CultureFeed_SearchActivitiesQuery $query
   *   The query.
   * @return CultureFeed_Activity[]
   *   The activities.
   *
   * @throws CultureFeed_ParseException
   *   If the result could not be parsed.
   */
  public function searchActivities(CultureFeed_SearchActivitiesQuery $query) {
    $use_auth = FALSE;

    if ($query->private) {
      $use_auth = TRUE;
    }

    $data = $query->toPostData();

    if ($use_auth) {
      $result = $this->oauth_client->authenticatedGetAsXml('activity', $data);
    }
    else {
      $result = $this->oauth_client->consumerGetAsXml('activity', $data);
    }

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
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
   *   Possible values are represented in the TOP_EVENTS_SORT_* constants.
   * @param integer $max
   *   Maximum number of events to return.
   * @return string[]
   *   The events.
   *
   * @throws CultureFeed_ParseException
   *   If the result could not be parsed.
   */
  public function getTopEvents($type, $max = 5) {
    $query = array('max' => $max);

    $result = $this->oauth_client->consumerGetAsXml('activity/topevents/' . $type, $query);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
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
   * @return CultureFeed_Recommendation[]
   *   The recommendations.
   *
   * @throws CultureFeed_ParseException
   *   If the result could not be parsed.
   */
  public function getRecommendationsForUser($id, $query = array()) { // @todo make $query a class
    $result = $this->oauth_client->authenticatedGetAsXml('recommendation/user/' . $id, $query);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
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
   * @return CultureFeed_Recommendation[]
   *   The recommendations.
   *
   * @throws CultureFeed_ParseException
   *   If the result could not be parsed.
   */
  public function getRecommendationsForEvent($id, array $query = array()) { // @todo make $query a class
    $query['eventId'] = $id;

    $result = $this->oauth_client->authenticatedGetAsXml('recommendation/event', $query);

    try {
      $xml = new CultureFeed_SimpleXMLElement($result);
    }
    catch (Exception $e) {
      throw new CultureFeed_ParseException($result);
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
   */
  public function evaluateRecommendation($id, $evaluation) {
    $data = array('evaluation' => $evaluation);

    $this->oauth_client->authenticatedPostAsXml('recommendation/evaluate/' . $id, $data);
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

    $query['network'] = $network;

    if (!empty($destination)) {
      $query['destination'] = $destination;
    }

    return $this->oauth_client->getUrl('auth/network/extra', $query);
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
      $query['destination'] = $destination;
    }

    return $this->oauth_client->getUrl('auth/changepassword/' . $id, $query);
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
      $query['destination'] = $destination;
    }

    return $this->oauth_client->getUrl('auth/logout', $query);
  }

}