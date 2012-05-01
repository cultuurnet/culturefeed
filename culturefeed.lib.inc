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

  public function getUrlAuthorize($token, $callback = '', $type = CultureFeed::AUTHORIZE_TYPE_REGULAR) {
    return $this->realCultureFeed->getUrlAuthorize($token, $callback, $type);
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
    $this->realCultureFeed->createActivity($activity);

    $this->cacheClearActivities();
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

  public function searchActivityUsers($nodeId, $type, $contentType, $start = NULL, $max = NULL) {
    $cid = sprintf('activity:users:%s:%s:%s:%s:%s', $nodeId, $type, $contentType, $start, $max);

    if ($cache = $this->cacheGet($cid)) {
      return $cache->data;
    }

    $data = $this->realCultureFeed->searchActivityUsers($nodeId, $type, $contentType, $start, $max);

    $this->cacheSet($cid, $data, REQUEST_TIME + CULTUREFEED_CACHE_EXPIRES);

    return $data;
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

  public function getRecommendationsForUser($id, CultureFeed_RecommendationsQuery $query = NULL) {
    $cid = 'recommendation:user:' . $id . ':' . md5(serialize($query->toPostData()));

    if ($cache = $this->cacheGet($cid)) {
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

  public function getClient() {
    return $this->realCultureFeed->getClient();
  }

}

class DrupalCultureFeed {

  protected static $consumer_instance;

  protected static $user_instance;

  protected static $logged_in_account;

  protected static $logged_in_user;

  public static function isCacheEnabled() {
    return variable_get('culturefeed_cache_status', CULTUREFEED_CACHE_DISABLED) == CULTUREFEED_CACHE_ENABLED;
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

    if (!$reset && isset(self::$logged_in_user[$application_key])) {
      return self::$logged_in_user[$application_key];
    }

    if (!self::isCultureFeedUser()) {
      return NULL;
    }

    self::$logged_in_user[$application_key] = self::getLoggedInUserInstance($application_key, $shared_secret)->getUser(self::getLoggedInUserId(), TRUE, TRUE);

    return self::$logged_in_user[$application_key];
  }

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

    self::$user_instance[$application_key] = self::getInstance($token->token, $token->secret, $application_key, $shared_secret);

    return self::$user_instance[$application_key];
  }

  public static function getConsumerInstance($application_key = NULL, $shared_secret = NULL) {
    if (!$application_key) {
      $application_key = variable_get('culturefeed_api_application_key', '');
      $shared_secret = variable_get('culturefeed_api_shared_secret', '');
    }

    if (isset(self::$consumer_instance[$application_key])) {
      return self::$consumer_instance[$application_key];
    }

    self::$consumer_instance[$application_key] = self::getInstance(NULL, NULL, $application_key, $shared_secret);

    return self::$consumer_instance[$application_key];
  }

  public static function getInstance($token, $secret, $application_key = NULL, $shared_secret = NULL) {
    if (!$application_key) {
      $application_key = variable_get('culturefeed_api_application_key', '');
      $shared_secret = variable_get('culturefeed_api_shared_secret', '');
    }

    if (!variable_get('culturefeed_proxy_enabled', FALSE)) {
      $oauth_client = new CultureFeed_DefaultOAuthClient($application_key, $shared_secret, $token, $secret);

    }
    else {
      $http_client = new CultureFeed_DefaultHttpClient();

      $http_client->proxy_server   = variable_get('culturefeed_proxy_server', '');
      $http_client->proxy_port     = variable_get('culturefeed_proxy_port', '');
      $http_client->proxy_username = variable_get('culturefeed_proxy_username', '');
      $http_client->proxy_password = variable_get('culturefeed_proxy_password', '');

      $oauth_client = new CultureFeed_DefaultOAuthClient($application_key, $shared_secret, $token, $secret);

      $oauth_client->setHttpClient($http_client);
    }

    $oauth_client->setEndpoint(variable_get('culturefeed_api_location', CULTUREFEED_API_LOCATION));

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
    self::getLoggedInUserInstance()->createActivity($activity);
  }

  public static function updateActivity($id, $private) {
    self::getLoggedInUserInstance()->updateActivity($id, $private);
  }

  public static function deleteActivity($id) {
    self::getLoggedInUserInstance()->deleteActivity($id);
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

  public static function getTopEvents($type, $max = 5) {
    return self::getConsumerInstance()->getTopEvents($type, $max);
  }

  public static function getRecommendationsForUser($id, CultureFeed_RecommendationsQuery $query = NULL) {
    return self::getLoggedInUserInstance()->getRecommendationsForUser($id, $query);
  }

  public static function getRecommendationsForEvent($id, CultureFeed_RecommendationsQuery $query = NULL) {
    return self::getConsumerInstance()->getRecommendationsForEvent($id, $query);
  }

  public static function evaluateRecommendation($id, $evaluation) {
    self::getLoggedInUserInstance()->evaluateRecommendation($id, $evaluation);
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
      $query->type = array(
        CultureFeed_Activity::TYPE_VIEW,
        CultureFeed_Activity::TYPE_DETAIL,
        CultureFeed_Activity::TYPE_LIKE,
        CultureFeed_Activity::TYPE_MAIL,
        CultureFeed_Activity::TYPE_PRINT,
        CultureFeed_Activity::TYPE_FACEBOOK,
        CultureFeed_Activity::TYPE_TWITTER,
        CultureFeed_Activity::TYPE_IK_GA,
      );
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