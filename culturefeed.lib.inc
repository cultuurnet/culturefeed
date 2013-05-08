<?php

class DrupalCultureFeed_Cache implements ICultureFeed {

  protected $loggedInUserId;

  protected $realCultureFeed;

  public function __construct(CultureFeed $realCultureFeed, $loggedInUserId) {
    $this->loggedInUserId = $loggedInUserId;
    $this->realCultureFeed = $realCultureFeed;
  }

  public function getRealCultureFeed() {
    return $this->realCultureFeed();
  }

  protected function getCachePrefix() {
    return 'culturefeed:results:';
  }

  protected function getCacheSuffix() {
    $consumer = $this->getConsumer();
    return sprintf(':%s:%s', $consumer->key, $consumer->secret);
  }

  protected function getCacheCidBase($cid_base) {
    return self::getCachePrefix() . $cid_base;
  }

  protected function getCacheCid($cid) {
    return self::getCachePrefix() . $cid . self::getCacheSuffix();
  }

  protected function cacheSet($cid, $data, $expires = CACHE_PERMANENT) {
    $cid = $this->getCacheCid($cid);
    cache_set($cid, $data, 'cache_culturefeed', $expires);
  }

  protected function cacheGet($cid) {
    $cid = $this->getCacheCid($cid);
    return cache_get($cid, 'cache_culturefeed');
  }

  protected function cacheClear($cid = NULL, $wildcard = FALSE) {
    cache_clear_all($cid, 'cache_culturefeed', $wildcard);
  }

  protected function cacheClearActivities() {
    $cid_base = 'activity:';
    $cid_base = $this->getCacheCidBase($cid_base);
    $this->cacheClear($cid_base, TRUE);
  }

  protected function cacheClearMailings() {
    $cid_base = 'mailing:';
    $cid_base = $this->getCacheCidBase($cid_base);
    $this->cacheClear($cid_base, TRUE);
  }

  protected function cacheClearRecommendations() {
    $cid_base = sprintf('recommendation:user:%s', $this->loggedInUserId);
    $cid_base = $this->getCacheCidBase($cid_base);
    $this->cacheClear($cid_base, TRUE);
  }

  public function getConsumer() {
    return $this->realCultureFeed->getConsumer();
  }

  public function getToken() {
    return $this->realCultureFeed->getToken();
  }

  public function getRequestToken($callback = '') {
    return $this->realCultureFeed->getRequestToken($callback);
  }

  public function getUrlAuthorize($token, $callback = '', $type = CultureFeed::AUTHORIZE_TYPE_REGULAR, $skip_confirmation = FALSE, $skip_authorization = FALSE, $via = '') {
    return $this->realCultureFeed->getUrlAuthorize($token, $callback, $type, $skip_confirmation, $skip_authorization, $via);
  }

  public function getAccessToken($oauth_verifier) {
    return $this->realCultureFeed->getAccessToken($oauth_verifier);
  }

  public function createUser(CultureFeed_User $user) {
    $this->realCultureFeed->createUser($user);
  }

  public function getUserPreferences($uid) {
    return $this->realCultureFeed->getUserPreferences($uid);
  }

  public function setUserPreferences($uid, CultureFeed_Preferences $preferences) {
    return $this->realCultureFeed->setUserPreferences($uid, $preferences);
  }

  public function updateUser(CultureFeed_User $user, $fields = array()) {
    $this->realCultureFeed->updateUser($user, $fields);
  }

  public function deleteUser($id) {
    $this->realCultureFeed->deleteUser($id);
  }

  public function getUser($id, $private = FALSE, $use_auth = TRUE) {
    return $this->realCultureFeed->getUser($id, $private, $use_auth);
  }

  public function searchUsers(CultureFeed_SearchUsersQuery $query) {
    return $this->realCultureFeed->searchUsers($query);
  }

  public function getSimilarUsers($id) {
    return $this->realCultureFeed->getSimilarUsers($id);
  }

  public function uploadUserDepiction($id, $file_data) {
    $this->realCultureFeed->uploadUserDepiction($id, $file_data);
  }

  public function resendMboxConfirmationForUser($id) {
    $this->realCultureFeed->resendMboxConfirmationForUser($id);
  }

  public function updateUserPrivacy($id, CultureFeed_UserPrivacyConfig $privacy_config) {
    $this->realCultureFeed->updateUserPrivacy($id, $privacy_config);
  }

  public function getUserServiceConsumers($id) {
    return $this->realCultureFeed->getUserServiceConsumers($id);
  }

  public function revokeUserServiceConsumer($user_id, $consumer_id) {
    $this->realCultureFeed->revokeUserServiceConsumer($user_id, $consumer_id);
  }

  public function updateUserOnlineAccount($id, CultureFeed_OnlineAccount $account) {
    $this->realCultureFeed->updateUserOnlineAccount($id, $account);
  }

  public function deleteUserOnlineAccount($id, $account_type, $account_name) {
    $this->realCultureFeed->deleteUserOnlineAccount($id, $account_type, $account_name);
  }

  public function createActivity(CultureFeed_Activity $activity) {
    $id = $this->realCultureFeed->createActivity($activity);

    $this->cacheClearActivities();

    return $id;
  }

  public function updateActivity($id, $private) {
    $this->realCultureFeed->updateActivity($id, $private);

    $this->cacheClearActivities();
  }

  public function deleteActivity($id) {
    $this->realCultureFeed->deleteActivity($id);

    $this->cacheClearActivities();
  }

  public function searchActivities(CultureFeed_SearchActivitiesQuery $query) {
    $cid = sprintf('activity:activities:%s', md5(serialize($query->toPostData())));

    if ($cache = $this->cacheGet($cid)) {
      return $cache->data;
    }

    $data = $this->realCultureFeed->searchActivities($query);

    $this->cacheSet($cid, $data, REQUEST_TIME + CULTUREFEED_CACHE_EXPIRES);

    return $data;
  }

  public function postToSocial($id, $account_name, $account_type, $message, $image = NULL, $link = NULL) {
    $this->realCultureFeed->postToSocial($id, $account_name, $account_type, $message, $image, $link);
  }

  public function searchActivityUsers($nodeId, $type, $contentType, $start = NULL, $max = NULL) {
    $cid = sprintf('activity:users:%s:%s:%s:%s:%s', $nodeId, $type, $contentType, $start, $max);

    if ($cache = $this->cacheGet($cid)) {
      return $cache->data;
    }

    $data = $this->realCultureFeed->searchActivityUsers($nodeId, $type, $contentType, $start, $max);

    $this->cacheSet($cid, $data, REQUEST_TIME + CULTUREFEED_CACHE_EXPIRES);

    return $data;
  }

  public function getMailing($id) {
    return $this->realCultureFeed->getMailing($id);
  }

  public function createMailing(CultureFeed_Mailing $mailing) {
    return $this->realCultureFeed->createMailing($mailing);

    $this->cacheClearMailings();
  }

  public function updateMailing(CultureFeed_Mailing $mailing, $fields = array()) {
    $this->realCultureFeed->updateMailing($mailing, $fields);

    $this->cacheClearMailings();
  }

  public function disableMailing($id) {
    $this->realCultureFeed->disableMailing($id);

    $this->cacheClearMailings();
  }

  public function deleteMailing($id) {
    $this->realCultureFeed->deleteMailing($id);

    $this->cacheClearMailings();
  }

  public function getMailingList(CultureFeed_SearchMailingsQuery $query) {
    return $this->realCultureFeed->getMailingList($query);
  }

  public function sendTestMailing($user_id, $mailing_id) {
    $this->realCultureFeed->sendTestMailing($user_id, $mailing_id);
  }

  public function sendMailing($id) {
    $this->realCultureFeed->sendMailing($id);
  }

  public function searchMailings(CultureFeed_SearchMailingsQuery $query) {
    $this->realCultureFeed->searchMailings($query);
  }

  public function subscribeToMailing($user_id, $mailing_id) {
    return $this->realCultureFeed->subscribeToMailing($user_id, $mailing_id);
  }

  public function unsubscribeFromMailing($user_id, $mailing_id) {
    $this->realCultureFeed->unsubscribeFromMailing($user_id, $mailing_id);
  }

  public function getMailingSubscriptions($user_id) {
    return $this->realCultureFeed->getMailingSubscriptions($user_id);
  }

  public function getTopEvents($type, $max = 5) {
    $cid = sprintf('topevents:%s:%s', $type, $max);

    if ($cache = $this->cacheGet($cid)) {
      return $cache->data;
    }

    $data = $this->realCultureFeed->getTopEvents($type, $max);

    $this->cacheSet($cid, $data, REQUEST_TIME + CULTUREFEED_CACHE_EXPIRES);

    return $data;
  }

  public function getRecommendationsForUser($id, CultureFeed_RecommendationsQuery $query = NULL, $skip_cache = FALSE) {
    $cid = 'recommendation:user:' . $id . ':' . md5(serialize($query->toPostData()));

    if (!$skip_cache && $cache = $this->cacheGet($cid)) {
      return $cache->data;
    }

    $data = $this->realCultureFeed->getRecommendationsForUser($id, $query);

    $this->cacheSet($cid, $data, REQUEST_TIME + CULTUREFEED_CACHE_EXPIRES);

    return $data;
  }

  public function getRecommendationsForEvent($id, CultureFeed_RecommendationsQuery $query = NULL) {
    $cid = 'recommendation:event:' . $id . ':' . md5(serialize($query->toPostData()));

    if ($cache = $this->cacheGet($cid)) {
      return $cache->data;
    }

    $data = $this->realCultureFeed->getRecommendationsForEvent($id, $query);

    $this->cacheSet($cid, $data, REQUEST_TIME + CULTUREFEED_CACHE_EXPIRES);

    return $data;  }

  public function evaluateRecommendation($id, $evaluation) {
    $this->realCultureFeed->evaluateRecommendation($id, $evaluation);

    $this->cacheClearRecommendations();

    if ($evaluation == CultureFeed::RECOMMENDATION_EVALUATION_POSITIVE) {
      $this->cacheClearActivities();
    }
  }

  /**
   * @see ICultureFeed::getNotificationsCount()
   */
  public function getNotificationsCount($userId, $dateFrom = NULL) {
    return $this->realCultureFeed->getNotificationsCount($userId, $dateFrom);
  }

  /**
   * @see ICultureFeed::getNotifications()
   */
  public function getNotifications($userId, $params = array()) {
    return $this->realCultureFeed->getNotifications($userId, $params);
  }
  
  public function getNodeStatus($contentType, $nodeId, $userId) {
    return $this->realCultureFeed->getNodeStatus($contentType, $nodeId, $userId);
  }

  public function followNode($contentType, $nodeId, $userId) {
    return $this->realCultureFeed->followNode($contentType, $nodeId, $userId);
  }
  
  public function unFollowNode($contentType, $nodeId, $userId) {
    return $this->realCultureFeed->unFollowNode($contentType, $nodeId, $userId);
  }

  public function getUrlAddSocialNetwork($network, $destination = '') {
    return $this->realCultureFeed->getUrlAddSocialNetwork($network, $destination);
  }

  public function getUrlChangePassword($id, $destination = '') {
    return $this->realCultureFeed->getUrlChangePassword($id, $destination);
  }

  public function getUrlLogout($destination = '') {
    return $this->realCultureFeed->getUrlLogout($destination);
  }

  public function getServiceConsumers($start = 0, $max = NULL) {
    return $this->realCultureFeed->getServiceConsumers($start, $max);
  }

  public function createServiceConsumer(CultureFeed_Consumer $consumer) {
    $this->realCultureFeed->createServiceConsumer($consumer);
  }

  public function updateServiceConsumer(CultureFeed_Consumer $consumer) {
    $this->realCultureFeed->updateServiceConsumer($consumer);
  }

  public function uitpas() {
    return $this->realCultureFeed->uitpas();
  }

  public function pages() {
    return $this->realCultureFeed->pages();
  }

  public function getClient() {
    return $this->realCultureFeed->getClient();
  }

}

abstract class DrupalCultureFeedBase {

  protected static $consumer_instance;

  protected static $user_instance;

  protected static $logged_in_account;

  protected static $logged_in_user;


  public static function isCacheEnabled() {

    if (! self::isCultureFeedUser()) {
      return variable_get('culturefeed_cache_status', CULTUREFEED_CACHE_DISABLED) == CULTUREFEED_CACHE_ENABLED;
    } else {
      return FALSE;
    }

  }

  public static function isCultureFeedUser($uid = NULL) {
    if (!$uid) {
      $account = self::getLoggedInAccount();
    }
    else {
      $account = user_load($uid);
    }

    return isset($account->culturefeed_uid) && !empty($account->culturefeed_uid);
  }

  public static function getLoggedInUserId() {
    if (self::getLoggedInAccount() && self::isCultureFeedUser()) {
      return self::getLoggedInAccount()->culturefeed_uid;
    }

    return NULL;
  }

  public static function getLoggedInAccount() {
    if (isset(self::$logged_in_account)) {
      return self::$logged_in_account;
    }

    if (user_is_anonymous()) {
      return NULL;
    }

    global $user;

    self::$logged_in_account = user_load($user->uid);

    return self::$logged_in_account;
  }

  public static function getLoggedInUser($application_key = NULL, $shared_secret = NULL, $reset = FALSE) {
    if (!$application_key) {
      $application_key = variable_get('culturefeed_api_application_key', '');
      $shared_secret = variable_get('culturefeed_api_shared_secret', '');
    }

    if (!$reset && isset(static::$logged_in_user[$application_key])) {
      return static::$logged_in_user[$application_key];
    }

    if (!self::isCultureFeedUser()) {
      return NULL;
    }

    static::$logged_in_user[$application_key] = static::getLoggedInUserInstance($application_key, $shared_secret)->getUser(self::getLoggedInUserId(), TRUE, TRUE);

    return static::$logged_in_user[$application_key];
  }

  /**
   * @param string|null $application_key
   * @param string|null $shared_secret
   * @return ICultureFeed
   * @throws Exception
   */
  public static function getLoggedInUserInstance($application_key = NULL, $shared_secret = NULL) {
    if (!$application_key) {
      $application_key = variable_get('culturefeed_api_application_key', '');
      $shared_secret = variable_get('culturefeed_api_shared_secret', '');
    }

    if (isset(self::$user_instance[$application_key])) {
      return self::$user_instance[$application_key];
    }

    $account = self::getLoggedInAccount();

    if (!isset($account->tokens) ||
        !isset($account->tokens[$application_key]) ||
        !isset($account->tokens[$application_key]->token) ||
        !isset($account->tokens[$application_key]->secret)) {
      throw new Exception('Not a valid token set.');
    }

    $token = $account->tokens[$application_key];

    static::$user_instance[$application_key] = static::getInstance($token->token, $token->secret, $application_key, $shared_secret);

    return static::$user_instance[$application_key];
  }

  public static function getConsumerInstance($application_key = NULL, $shared_secret = NULL) {
    if (!$application_key) {
      $application_key = variable_get('culturefeed_api_application_key', '');
      $shared_secret = variable_get('culturefeed_api_shared_secret', '');
    }

    if (isset(static::$consumer_instance[$application_key])) {
      return static::$consumer_instance[$application_key];
    }

    static::$consumer_instance[$application_key] = static::getInstance(NULL, NULL, $application_key, $shared_secret);

    return static::$consumer_instance[$application_key];
  }

  /**
   * Get an OAuthClient instance for CultureFeed.
   * @param string $endpoint
   *   Endpoint for the client.
   */
  public static function getOAuthClient($endpoint, $token, $secret, $application_key = NULL, $shared_secret = NULL) {
    if (!$application_key) {
      $application_key = variable_get('culturefeed_api_application_key', '');
      $shared_secret = variable_get('culturefeed_api_shared_secret', '');
    }

    $oauth_client = new CultureFeed_DefaultOAuthClient($application_key, $shared_secret, $token, $secret);

    $oauth_client->setEndpoint($endpoint);

    $uri = @parse_url($endpoint);
    $proxy_server = variable_get('proxy_server', '');
    if ($proxy_server && _drupal_http_use_proxy($uri['host'])) {
      $http_client = new CultureFeed_DefaultHttpClient();

      $http_client->setProxyServer($proxy_server);
      $http_client->setProxyPort(variable_get('proxy_port', 8080));
      $http_client->setProxyUsername(variable_get('proxy_username', ''));
      $http_client->setProxyPassword(variable_get('proxy_password', ''));

      $oauth_client->setHttpClient($http_client);
    }

    return $oauth_client;
  }

}

class DrupalCultureFeed extends DrupalCultureFeedBase {

  protected static $consumer_instance;
  protected static $user_instance;
  protected static $logged_in_user;

  public static function getInstance($token, $secret, $application_key = NULL, $shared_secret = NULL) {
    $endpoint = variable_get('culturefeed_api_location', CULTUREFEED_API_LOCATION);
    $oauth_client = self::getOAuthClient($endpoint, $token, $secret, $application_key, $shared_secret);

    $cf = new CultureFeed($oauth_client);

    if (self::isCacheEnabled()) {
      $cf = new DrupalCultureFeed_Cache($cf, self::getLoggedInUserId());
    }

    return $cf;
  }

  public static function createUser(CultureFeed_User $user) {
    self::getLoggedInUserInstance()->createUser($user);
  }

  public static function updateUser(CultureFeed_User $user, $fields = array()) {
    self::getLoggedInUserInstance()->updateUser($user, $fields);
  }

  public static function deleteUser($id) {
    self::getLoggedInUserInstance()->deleteUser($id);
  }

  public static function getUser($id, $private = FALSE) {
    if ($private) {
      return self::getLoggedInUserInstance()->getUser($id, TRUE, TRUE);
    }
    else {
      return self::getConsumerInstance()->getUser($id, FALSE, FALSE);
    }
  }

  public static function searchUsers(CultureFeed_SearchUsersQuery $query) {
    return self::getConsumerInstance()->searchUsers($query);
  }

  public static function getSimilarUsers($id) {
    return self::getConsumerInstance()->getSimilarUsers($id);
  }

  public static function uploadUserDepiction($id, $file_data) {
    self::getLoggedInUserInstance()->uploadUserDepiction($id, $file_data);
  }

  public static function resendMboxConfirmationForUser($id) {
    self::getLoggedInUserInstance()->resendMboxConfirmationForUser($id);
  }

  public static function updateUserPrivacy($id, CultureFeed_UserPrivacyConfig $privacy_config) {
    self::getLoggedInUserInstance()->updateUserPrivacy($id, $privacy_config);
  }

  public static function getUserServiceConsumers($id) {
    return self::getLoggedInUserInstance()->getUserServiceConsumers($id);
  }

  public static function revokeUserServiceConsumer($user_id, $consumer_id) {
    self::getLoggedInUserInstance()->revokeUserServiceConsumer($user_id, $consumer_id);
  }

  public static function updateUserOnlineAccount($id, CultureFeed_OnlineAccount $account) {
    self::getLoggedInUserInstance()->updateUserOnlineAccount($id, $account);
  }

  public static function getUserOnlineAccounts($application_key = NULL, $shared_secret = NULL) {
    $cf_account = self::getLoggedInUser($application_key, $shared_secret);

    $online_accounts = array();

    if ($cf_account->holdsAccount) {
      foreach ($cf_account->holdsAccount as $online_account) {
        $online_accounts[$online_account->accountType] = $online_account;
      }
    }

    return $online_accounts;
  }

  public static function getUserOnlineAccount($account_type) {
    $online_accounts = self::getUserOnlineAccounts();

    if (isset($online_accounts[$account_type])) {
      return $online_accounts[$account_type];
    }

    return FALSE;
  }

  public static function deleteUserOnlineAccount($id, $account_type, $account_name) {
    self::getLoggedInUserInstance()->deleteUserOnlineAccount($id, $account_type, $account_name);
  }

  public static function createActivity(CultureFeed_Activity $activity) {
    return self::getLoggedInUserInstance()->createActivity($activity);
  }

  public static function postToSocial($id, $account_name, $account_type, $message, $image = NULL, $link = NULL) {
    return self::getLoggedInUserInstance()->postToSocial($id, $account_name, $account_type, $message, $image, $link);
  }

  public static function updateActivity($id, $private) {
    return self::getLoggedInUserInstance()->updateActivity($id, $private);
  }

  public static function deleteActivity($id) {
    return self::getLoggedInUserInstance()->deleteActivity($id);
  }

  public static function deleteActivities($user_id, $node_id, $content_type, $activity_type) {
    $query = new CultureFeed_SearchActivitiesQuery();
    $query->type = $activity_type;
    $query->contentType = $content_type;
    $query->nodeId = $node_id;
    $query->userId = $user_id;
    $query->private = TRUE;

    $activities = self::searchActivities($query);

    if (empty($activities->objects)) {
      return;
    }

    foreach ($activities->objects as $activity) {
      self::deleteActivity($activity->id);
    }
  }

  public static function searchActivityUsers($nodeId, $type, $contentType, $start = NULL, $max = NULL) {
    return self::getConsumerInstance()->searchActivityUsers($nodeId, $type, $contentType, $start, $max);
  }

  public static function searchActivities(CultureFeed_SearchActivitiesQuery $query) {
    if ($query->private) {
      $data = self::getLoggedInUserInstance()->searchActivities($query);
    }
    else {
      $data = self::getConsumerInstance()->searchActivities($query);
    }

    return $data;
  }

  public static function getMailing($id) {
    return self::getLoggedInUserInstance()->getMailing($id);
  }

  public static function createMailing(CultureFeed_Mailing $mailing) {
    return self::getLoggedInUserInstance()->createMailing($mailing);
  }

  public static function updateMailing(CultureFeed_Mailing $mailing, $fields = array()) {
    self::getLoggedInUserInstance()->updateMailing($mailing, $fields);
  }

  public static function disableMailing($id) {
    self::getLoggedInUserInstance()->disableMailing($id);
  }

  public static function deleteMailing($id) {
    self::getLoggedInUserInstance()->deleteMailing($id);
  }

  public static function getMailingList(CultureFeed_SearchMailingsQuery $query) {
    return self::getLoggedInUserInstance()->getMailingList($query);
  }

  public static function sendTestMailing($user_id, $mailing_id) {
    self::getLoggedInUserInstance()->sendTestMailing($user_id, $mailing_id);
  }

  public static function sendMailing($id) {
    self::getLoggedInUserInstance()->sendMailing($id);
  }

  public static function searchMailings(CultureFeed_SearchMailingsQuery $query) {
    return self::getLoggedInUserInstance()->searchMailings($query);
  }

  public static function subscribeToMailing($user_id, $mailing_id) {
    self::getLoggedInUserInstance()->subscribeToMailing($user_id, $mailing_id);
  }

  public static function unsubscribeFromMailing($user_id, $mailing_id) {
    self::getLoggedInUserInstance()->unsubscribeFromMailing($user_id, $mailing_id);
  }

  public static function getMailingSubscriptions($user_id) {
    return self::getLoggedInUserInstance()->getMailingSubscriptions($user_id);
  }

  public static function getTopEvents($type, $max = 5) {
    return self::getConsumerInstance()->getTopEvents($type, $max);
  }

  public static function getRecommendationsForUser($id, CultureFeed_RecommendationsQuery $query = NULL, $skip_cache = FALSE) {
    return self::getLoggedInUserInstance()->getRecommendationsForUser($id, $query, $skip_cache);
  }

  public static function getRecommendationsForEvent($id, CultureFeed_RecommendationsQuery $query = NULL) {
    return self::getConsumerInstance()->getRecommendationsForEvent($id, $query);
  }

  public static function evaluateRecommendation($id, $evaluation) {
    self::getLoggedInUserInstance()->evaluateRecommendation($id, $evaluation);
  }

  /**
   * @see ICultureFeed::getNotificationsCount()
   */
  public static function getNotificationsCount($userId, $dateFrom = NULL) {
    return self::getLoggedInUserInstance()->getNotificationsCount($userId, $dateFrom);
  }

  /**
   * @see ICultureFeed::getNotifications()
   */
  public static function getNotifications($userId, $params = array()) {
    return self::getLoggedInUserInstance()->getNotifications($userId, $params);
  }
  
  public static function getNodeStatus($contentType, $nodeId, $userId) {
    return self::getLoggedInUserInstance()->getNodeStatus($contentType, $nodeId, $userId);
  }

  public static function followNode($contentType, $nodeId, $userId) {
    return self::getLoggedInUserInstance()->followNode($contentType, $nodeId, $userId);
  }
  
  public static function unFollowNode($contentType, $nodeId, $userId) {
    return self::getLoggedInUserInstance()->unFollowNode($contentType, $nodeId, $userId);
  }

  public static function getUrlAddSocialNetwork($network, $destination = '') {
    return self::getConsumerInstance()->getUrlAddSocialNetwork($network, $destination);
  }

  public static function getUrlChangePassword($id, $destination = '') {
    return self::getConsumerInstance()->getUrlChangePassword($id, $destination);
  }

  public static function getUrlLogout($destination = '') {
    return self::getConsumerInstance()->getUrlLogout($destination);
  }

  public static function userDidActivity($type, $nodeId, $contentType) {
    $userDidActivity = &drupal_static(__FUNCTION__, array());

    $user = self::getLoggedInUserId();

    if (!isset($userDidActivity[$nodeId][$contentType][$user])) {
      $query = new CultureFeed_SearchActivitiesQuery();
      $query->nodeId = $nodeId;
      $query->contentType = $contentType;
      $query->userId = $user;
      $query->private = TRUE;

      $activities = self::searchActivities($query);

      $userDidActivity[$nodeId][$contentType][$user] = $activities;
    }

    $activities = $userDidActivity[$nodeId][$contentType][$user];

    if (!empty($activities->objects)) {
      foreach ($activities->objects as $activity) {
        if ($activity->type == $type) {
          return TRUE;
        }
      }
    }

    return FALSE;
  }
}

/**
 * @class
 * Drupal layer for the CultureFeed_EntryApi class.
 */
class DrupalCultureFeed_EntryApi extends DrupalCultureFeedBase {

  protected static $consumer_instance;
  protected static $user_instance;
  protected static $logged_in_user;

  public static function getInstance($token, $secret, $application_key = NULL, $shared_secret = NULL) {

    $endpoint = variable_get('culturefeed_api_location', CULTUREFEED_API_LOCATION);
    $endpoint .= variable_get('culturefeed_entry_api_path', CULTUREFEED_ENTRY_API_PATH);

    $oauth_client = self::getOAuthClient($endpoint, $token, $secret, $application_key, $shared_secret);

    return new CultureFeed_EntryApi($oauth_client);

  }

  /**
   * @see CultureFeed_EntryApi::getEvent()
   */
  public static function getEvent($id) {
    return self::getLoggedInUserInstance()->getEvent($id);
  }

  /**
   * @see CultureFeed_EntryApi::getActor()
   */
  public static function getActor($id) {
    return self::getLoggedInUserInstance()->getActor($id);
  }

  /**
   * @see CultureFeed_EntryApi::getProduction()
   */
  public static function getProduction($id) {
    return self::getLoggedInUserInstance()->getProduction($id);
  }

  /**
   * @see CultureFeed_EntryApi::createEvent()
   */
  public static function createEvent(CultureFeed_Cdb_Item_Event $event) {
    return self::getLoggedInUserInstance()->createEvent($event);
  }

  /**
   * @see CultureFeed_EntryApi::createActor()
   */
  public static function createActor(CultureFeed_Cdb_Item_Actor $actor) {
    return self::getLoggedInUserInstance()->createActor($event);
  }

  /**
   * @see CultureFeed_EntryApi::createProduction()
   */
  public static function createProduction(CultureFeed_Cdb_Item_Production $production) {
    return self::getLoggedInUserInstance()->createProduction($production);
  }

  /**
   * @see CultureFeed_EntryApi::updateEvent()
   */
  public static function updateEvent(CultureFeed_Cdb_Item_Event $event) {
    self::getLoggedInUserInstance()->updateEvent($event);
  }

  /**
   * @see CultureFeed_EntryApi::updateActor()
   */
  public static function updateActor(CultureFeed_Cdb_Item_Actor $actor) {
    self::getLoggedInUserInstance()->updateActor($actor);
  }

  /**
   * @see CultureFeed_EntryApi::updateProduction()
   */
  public static function updateProduction(CultureFeed_Cdb_Item_Production $production) {
    self::getLoggedInUserInstance()->updateProduction($production);
  }

  /**
   * @see CultureFeed_EntryApi::deleteEvent()
   */
  public static function deleteEvent($id) {
    self::getLoggedInUserInstance()->deleteEvent($id);
  }

  /**
   * @see CultureFeed_EntryApi::deleteActor()
   */
  public static function deleteActor($id) {
    self::getLoggedInUserInstance()->deleteActor($id);
  }

  /**
   * @see CultureFeed_EntryApi::deleteProduction()
   */
  public static function deleteProduction($id) {
    self::getLoggedInUserInstance()->deleteProduction($id);
  }

  /**
   * @see CultureFeed_EntryApi::addTagToEvent()
   */
  public static function addTagToEvent(CultureFeed_Cdb_Item_Event $event, $keywords) {
    self::getLoggedInUserInstance()->addTagToEvent($event, $keywords);
  }

  /**
   * @see CultureFeed_EntryApi::removeTagFromEvent()
   */
  public static function removeTagFromEvent(CultureFeed_Cdb_Item_Event $event, $keyword) {
    self::getLoggedInUserInstance()->removeTagFromEvent($event, $keyword);
  }

  /**
   * @see CultureFeed_EntryApi::addLinkToEvent()
   */
  public static function addLinkToEvent(CultureFeed_Cdb_Item_Event $event, $link, $linktype = '', $lang = '') {
    self::getLoggedInUserInstance()->addLinkToEvent($event, $link, $linktype, $lang);
  }

  /**
   * @see CultureFeed_EntryApi::removeLinkFromEvent()
   */
  public static function removeLinkFromEvent(CultureFeed_Cdb_Item_Event $event, $link) {
    self::getLoggedInUserInstance()->removeLinkFromEvent($event, $link);
  }

  /**
   * @see CultureFeed_EntryApi::addTranslationToEvent()
   */
  public static function addTranslationToEvent(CultureFeed_Cdb_Item_Event $event, $lang, $title = '', $shortDescription = '', $longDescription = '') {
    self::getLoggedInUserInstance()->addTranslationToEvent($event, $lang, $title, $shortDescription, $longDescription);
  }

  /**
   * @see CultureFeed_EntryApi::removeTranslationFromEvent()
   */
  public static function removeTranslationFromEvent(CultureFeed_Cdb_Item_Event $event, $lang) {
    self::getLoggedInUserInstance()->removeTranslationFromEvent($event, $lang);
  }

  /**
   * @see CultureFeed_EntryApi::addTagToProduction()
   */
  public static function addTagToProduction(CultureFeed_Cdb_Item_Production $production, $keywords) {
    self::getLoggedInUserInstance()->addTagToProduction($production, $keywords);
  }

  /**
   * @see CultureFeed_EntryApi::removeTagFromProduction()
   */
  public static function removeTagFromProduction(CultureFeed_Cdb_Item_Production $production, $keyword) {
    self::getLoggedInUserInstance()->removeTagFromProduction($production, $keyword);
  }

  /**
   * @see CultureFeed_EntryApi::addLinkToProduction()
   */
  public static function addLinkToProduction(CultureFeed_Cdb_Item_Production $production, $link, $linktype = '', $lang = '') {
    self::getLoggedInUserInstance()->addLinkToProduction($production, $link, $linktype, $lang);
  }

  /**
   * @see CultureFeed_EntryApi::removeLinkFromProduction()
   */
  public static function removeLinkFromProduction(CultureFeed_Cdb_Item_Production $production, $link) {
    self::getLoggedInUserInstance()->removeLinkFromProduction($production, $link);
  }

  /**
   * @see CultureFeed_EntryApi::addTranslationToProduction()
   */
  public static function addTranslationToProduction(CultureFeed_Cdb_Item_Production $production, $lang, $title = '', $shortDescription = '', $longDescription = '') {
    self::getLoggedInUserInstance()->addTranslationToProduction($production, $lang, $title, $shortDescription, $longDescription);
  }

  /**
   * @see CultureFeed_EntryApi::removeTranslationFromProduction()
   */
  public static function removeTranslationFromProduction(CultureFeed_Cdb_Item_Production $production, $lang) {
    self::getLoggedInUserInstance()->removeTranslationFromProduction($production, $lang);
  }

  /**
   * @see CultureFeed_EntryApi::addTagToActor()
   */
  public static function addTagToActor(CultureFeed_Cdb_Item_Actor $actor, $keywords) {
    self::getLoggedInUserInstance()->addTagToActor($actor, $keywords);
  }

  /**
   * @see CultureFeed_EntryApi::removeTagFromActor()
   */
  public static function removeTagFromActor(CultureFeed_Cdb_Item_Actor $actor, $keyword) {
    self::getLoggedInUserInstance()->removeTagFromActor($actor, $keyword);
  }

  /**
   * @see CultureFeed_EntryApi::addLinkToActor()
   */
  public static function addLinkToActor(CultureFeed_Cdb_Item_Actor $actor, $link, $linktype = '', $lang = '') {
    self::getLoggedInUserInstance()->addLinkToActor($actor, $link, $linktype, $lang);
  }

  /**
   * @see CultureFeed_EntryApi::removeLinkFromActor()
   */
  public static function removeLinkFromActor(CultureFeed_Cdb_Item_Actor $actor, $link) {
    self::getLoggedInUserInstance()->removeLinkFromActor($actor, $link);
  }

  /**
   * @see CultureFeed_EntryApi::addTranslationToActor()
   */
  public static function addTranslationToActor(CultureFeed_Cdb_Item_Actor $actor, $lang, $title = '', $shortDescription = '', $longDescription = '') {
    self::getLoggedInUserInstance()->addTranslationToActor($actor, $lang, $title, $shortDescription, $longDescription);
  }

  /**
   * @see CultureFeed_EntryApi::removeTranslationFromActor()
   */
  public static function removeTranslationFromActor(CultureFeed_Cdb_Item_Actor $actor, $lang) {
    self::getLoggedInUserInstance()->removeTranslationFromActor($actor, $lang);
  }

}
