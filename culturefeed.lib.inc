<?php

class DrupalCultureFeed {

  protected static $consumer_instance;

  protected static $user_instance;

  protected static $logged_in_account;

  protected static $logged_in_user;
  
  public static function isCacheEnabled() {
    return variable_get('culturefeed_cache_status', CULTUREFEED_CACHE_DISABLED) == CULTUREFEED_CACHE_ENABLED;
  }
  
  public static function getCachePrefix($instance) {
    $consumer = $instance->getConsumer();
    return sprintf('culturefeed:results:%s:%s:', $consumer->key, $consumer->secret);
  }
  
  public static function getConsumerCachePrefix() {
    $consumer = self::getConsumerInstance()->getConsumer();
    return sprintf('culturefeed:results:%s:%s:', $consumer->key, $consumer->secret);
  }
  
  public static function getLoggedInUserCachePrefix() {
    $consumer = self::getLoggedInUserInstance()->getConsumer();
    return sprintf('culturefeed:results:%s:%s:', $consumer->key, $consumer->secret);
  }
  
  public static function getConsumerCacheCid($cid) {
    return self::getConsumerCachePrefix() . $cid;
  }
  
  public static function getLoggedInUserCacheCid($cid) {
    return self::getLoggedInUserCachePrefix() . $cid;
  }
  
  public static function cacheSet($cid, $data) {
    if (self::isCacheEnabled()) {
      cache_set($cid, $data, 'cache_culturefeed');
    }
  }
  
  public static function cacheGet($cid, $data) {
    if (self::isCacheEnabled()) {
      return cache_get($cid, $data, 'cache_culturefeed');
    }
  }
  
  public static function cacheClear($cid = NULL, $wildcard = FALSE) {
    if (self::isCacheEnabled()) {
      cache_clear_all($cid, 'cache_culturefeed', $wildcard);
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
    return self::getLoggedInAccount()->culturefeed_uid;
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

  /**
   * @todo document this
   *
   * @return CultureFeed_User
   *   @todo document this
   */
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

  /**
   * @todo document this
   *
   * @return CultureFeed
   *   @todo document this
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

    $token = $account->tokens[$application_key];

    if (!isset($token->token) || !isset($token->secret)) {
      throw new Exception('Not a valid token set.');
    }

    self::$user_instance[$application_key] = self::getInstance($token->token, $token->secret, $application_key, $shared_secret);

    return self::$user_instance[$application_key];
  }

  /**
   * @todo document this
   *
   * @return CultureFeed
   *   @todo document this
   */
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

  /**
   * @todo document this
   *
   * @return CultureFeed
   *   @todo document this
   */
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

    return new CultureFeed($oauth_client);
  }

  public static function createUser(CultureFeed_User $user) {
    self::getLoggedInUserInstance()->createUser($user);
  }

  public static function updateUser(CultureFeed_User $user) {
    self::getLoggedInUserInstance()->updateUser($user);
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

  public static function getUserOnlineAccounts() {
    $cf_account = self::getLoggedInUser();
    
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
    
    $cid_base = 'activity:';
    $cid_base = self::getLoggedInUserCacheCid($cid_base);
    self::cacheClear($cid_base, TRUE);
  }

  public static function updateActivity($id, $private) {
    self::getLoggedInUserInstance()->updateActivity($id, $private);
    
    $cid_base = 'activity:';
    $cid_base = self::getLoggedInUserCacheCid($cid_base);
    self::cacheClear($cid_base, TRUE);
  }

  public static function deleteActivity($id) {
    self::getLoggedInUserInstance()->deleteActivity($id);
    
    $cid_base = 'activity:';
    $cid_base = self::getLoggedInUserCacheCid($cid_base);
    self::cacheClear($cid_base, TRUE);
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
    $cid = sprintf('activity:users:%s:%s:%s:%s:%s', $nodeId, $type, $contentType, $start, $max);
    $cid = self::getConsumerCacheCid($cid);
    
    if (self::cacheGet($cid)) {
      return $cache->data;
    }
    
    $data = self::getConsumerInstance()->searchActivityUsers($nodeId, $type, $contentType, $start, $max);
    
    self::cacheSet($cid, $data);
    
    return $data;
  }

  public static function searchActivities(CultureFeed_SearchActivitiesQuery $query) {
    if ($query->private) {
      $cid = sprintf('activity:activities:%s', md5(serialize($query->toPostData())));
      $cid = self::getLoggedInUserCacheCid($cid);
    }
    else {
      $cid = sprintf('activity:activities:%s', md5(serialize($query->toPostData())));
      $cid = self::getConsumerCacheCid($cid);
    }
    
    if (self::cacheGet($cid)) {
      return $cache->data;
    }
    
    if ($query->private) {
      $data = self::getLoggedInUserInstance()->searchActivities($query);
    }
    else {
      $data = self::getConsumerInstance()->searchActivities($query);
    }
    
    self::cacheSet($cid, $data);
    
    return $data;
  }

  public static function getTopEvents($type, $max = 5) {
    $cid = sprintf('topevents:%s:%s', $type, $max);
    $cid = self::getConsumerCacheCid($cid);
    
    if (self::cacheGet($cid)) {
      return $cache->data;
    }
    
    $data = self::getConsumerInstance()->getTopEvents($type, $max);
    
    self::cacheSet($cid, $data);
    
    return $data;
  }

  public static function getRecommendationsForUser($id, CultureFeed_RecommendationsQuery $query = NULL) {
    $cid = 'recommendation:user:' . $id . ':' . md5(serialize($query->toPostData()));
    $cid = self::getLoggedInUserCacheCid($cid);
    
    if (self::cacheGet($cid)) {
      return $cache->data;
    }
    
    $data = self::getLoggedInUserInstance()->getRecommendationsForUser($id, $query);
    
    self::cacheSet($cid, $data);
    
    return $data;
  }

  public static function getRecommendationsForEvent($id, CultureFeed_RecommendationsQuery $query = NULL) {
    $cid = 'recommendation:event:' . $id . ':' . md5(serialize($query->toPostData()));
    $cid = self::getConsumerCacheCid($cid);
    
    if (self::cacheGet($cid)) {
      return $cache->data;
    }
    
    $data = self::getConsumerInstance()->getRecommendationsForEvent($id, $query);
    
    self::cacheSet($cid, $data);
    
    return $data;
  }

  public static function evaluateRecommendation($id, $evaluation) {
    $result = self::getLoggedInUserInstance()->evaluateRecommendation($id, $evaluation);
    
    $cid_base = sprintf('recommendation:user:%s', self::getLoggedInUserId());
    $cid_base = self::getLoggedInUserCacheCid($cid_base);
    self::cacheClear($cid_base, TRUE);
    
    return $result;
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

  public static function userDidActivity($type, $nodeId, $contentType, $user = NULL) {
    $userDidActivity = &drupal_static(__FUNCTION__, array());

    if (!$user) {
      $user = self::getLoggedInUserId();
    }

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
      $query->userId = self::getLoggedInUserId();
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