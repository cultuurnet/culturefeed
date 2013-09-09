<?php
/**
 * @class
 * Drupal caching for culturefeed.
 */
class DrupalCultureFeed_Cache implements ICultureFeed {

  protected $loggedInUserId;
  protected $realCultureFeed;
  protected $pages;
  protected $messages;

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
    $result = $this->realCultureFeed->createActivity($activity);

    // Let other modules hook onto the activity creation result.
    module_invoke_all('culturefeed_social_activity_created', $result);

    $this->cacheClearActivities();


    return $result;
  }

  public function updateActivity($id, $private) {
    $this->realCultureFeed->updateActivity($id, $private);

    $this->cacheClearActivities();
  }

  public function deleteActivity($id) {
    $result = $this->realCultureFeed->deleteActivity($id);
    module_invoke_all('culturefeed_social_activity_deleted', $result);
    $this->cacheClearActivities();
    return $result;
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

  public function getTotalActivities($userId, $type_contentType, $private = FALSE) {
    return $this->realCultureFeed->getTotalActivities($userId, $type_contentType, $private);
  }

  /**
   * @see CultureFeed::getActivityPointsTimeline()
   */
  public function getActivityPointsTimeline($userId) {
    return $this->realCultureFeed->getActivityPointsTimeline($userId);
  }

  public function getActivityPointsPromotion($promotionId) {
    return $this->realCultureFeed->getActivityPointsPromotion($promotionId);
  }

  public function getActivityPointsPromotions($params = array()) {
    return $this->realCultureFeed->getActivityPointsPromotions($params);
  }

  public function cashInPromotion($userId, array $promotionId, array $promotionCount) {
    return $this->realCultureFeed->cashInPromotion($userId, $promotionId, $promotionCount);
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

    /**
     * Get pages service.
     * @return DrupalCultureFeedPages_Cache
     */
    public function pages() {

      if (!$this->pages) {
        $this->pages = new DrupalCultureFeedPages_Cache($this->realCultureFeed->pages());
      }

      return $this->pages;

    }

    /**
     * Get messages service.
     * Messages don't need cache.
     */
    public function messages() {
      return $this->realCultureFeed->messages();
    }

    public function getClient() {
      return $this->realCultureFeed->getClient();
    }

}