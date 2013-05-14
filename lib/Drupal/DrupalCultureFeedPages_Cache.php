<?php

/**
 * @class
 * Drupal cache layer for the CultureFeed pages Service.
 */
class DrupalCultureFeedPages_Cache implements CultureFeed_Pages {

  /**
   * @var CultureFeed_Pages_Default
   */
  protected $realCultureFeedPages;

  /**
   * Constructor.
   *
   * Culturefeed pages instance to be called.
   * @param CultureFeed_Pages_Default $realCultureFeed
   */
  public function __construct(CultureFeed_Pages_Default $realCultureFeedPages) {
    $this->realCultureFeedPages = $realCultureFeedPages;
  }

  /**
   * Get the cache prefix.
   */
  protected function getCachePrefix() {
    return 'culturefeed:pages:';
  }

  /**
   * Get the cache suffix.
   * @return string
   */
  protected function getCacheSuffix() {
    $consumer = $this->realCultureFeedPages->getConsumer();
    return sprintf(':%s:%s', $consumer->key, $consumer->secret);
  }

  /**
   * Get the full cache id.
   * @param string $cid
   *   Cid to use in the full cache id.
   * @return string
   */
  protected function getCacheCid($cid) {
    return self::getCachePrefix() . $cid . self::getCacheSuffix();
  }

  /**
   * Set the cache.
   */
  protected function cacheSet($cid, $data, $expires = CACHE_PERMANENT) {
    $cid = $this->getCacheCid($cid);
    cache_set($cid, $data, 'cache_culturefeed', $expires);
  }

  /**
   * Get the cache.
   */
  protected function cacheGet($cid) {
    $cid = $this->getCacheCid($cid);
    return cache_get($cid, 'cache_culturefeed');
  }

  /**
   * Clear the cache.
   */
  protected function cacheClear($cid = NULL, $wildcard = FALSE) {
    cache_clear_all($cid, 'cache_culturefeed', $wildcard);
  }

  /**
   * Clear the userlist cache.
   * @param string $id
   *   Page id.
   * @param string $memberType
   *   Member type ex ADMIN
   */
  protected function clearUserListCache($id, $memberType) {
    $cid_base = 'userList:' . $id . ':' . md5(serialize(array($memberType))) . ':';
    // Clear both authorized and non authorized calls.
    $this->cacheClear($cid_base . '1');
    $this->cacheClear($cid_base . '0');
  }

  /**
   * @see CultureFeed_Pages::getPage()
   */
  public function getPage($id) {

    $cid = 'page:' . $id;

    if ($cache = $this->cacheGet($cid)) {
      return $cache->data;
    }

    $data = $this->realCultureFeedPages->getPage($id);

    $this->cacheSet($cid, $data, REQUEST_TIME + CULTUREFEED_CACHE_EXPIRES);

    return $data;

  }

  /**
   * @see CultureFeed_Pages::addPage()
   */
  public function addPage(array $params) {
    $result = $this->realCultureFeedPages->addPage($params);
    $this->cacheClear('page:' . $id);
    return $result;
  }

  /**
   * @see CultureFeed_Pages::updatePage()
   */
  public function updatePage($id, array $params) {
    $result = $this->realCultureFeedPages->updatePage($id, $params);
    $this->cacheClear('page:' . $id);
    return $result;
  }

  /**
   * @see CultureFeed_Pages::removePage()
   */
  public function removePage($id) {
    $result = $this->realCultureFeedPages->removePage($id);
    $this->cacheClear('page:' . $id);
    return $result;
  }

  /**
   * @see CultureFeed_Pages::publishPage()
   */
  public function publishPage($id) {
    $result = $this->realCultureFeedPages->publishPage($id);
    $this->cacheClear('page:' . $id);
    return $result;
  }

  /**
   * @see CultureFeed_Pages::addImage()
   */
  public function addImage($id, array $params) {
    $result = $this->realCultureFeedPages->addImage($id, $params);
    $this->cacheClear('page:' . $id);
    return $result;
  }

  /**
   * @see CultureFeed_Pages::changePermissions()
   */
  public function changePermissions($id, array $params) {
    $result = $this->realCultureFeedPages->changePermissions($id, $params);
    $this->cacheClear('page:' . $id);
    return $result;
  }

  /**
   * @see CultureFeed_Pages::getUserList()
   */
  public function getUserList($id, $roles = array(), $use_auth = TRUE) {

    $cid = 'userList:' . $id . ':' . md5(serialize($roles)) . ':' . $use_auth;

    if ($cache = $this->cacheGet($cid)) {
      return $cache->data;
    }

    $data = $this->realCultureFeedPages->getUserList($id, $roles, $use_auth);

    $this->cacheSet($cid, $data, REQUEST_TIME + CULTUREFEED_CACHE_EXPIRES);

    return $data;

  }

  /**
   * @see CultureFeed_Pages::addMember()
   */
  public function addMember($id, $userId, $params = array()) {
    $this->realCultureFeedPages->addMember($id, $userId, $params);
    $this->clearUserListCache($id, CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_MEMBER);
  }

  /**
   * @see CultureFeed_Pages::updateMember()
   */
  public function updateMember($id, $userId, array $params) {
    $this->realCultureFeedPages->updateMember($id, $userId, $params);
    $this->clearUserListCache($id, CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_MEMBER);
  }

  /**
   * @see CultureFeed_Pages::removeMember()
   */
  public function removeMember($id, $userId) {
    $this->realCultureFeedPages->removeMember($id, $userId, $params);
    $this->clearUserListCache($id, CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_MEMBER);
  }

  /**
   * @see CultureFeed_Pages::follow()
   */
  public function follow($id, array $params) {
    $this->realCultureFeedPages->follow($id, $params);
    $this->clearUserListCache($id, CultureFeed_Pages_Follower::ROLE);
  }

  /**
   * @see CultureFeed_Pages::defollow()
   */
  public function defollow($id, $userId, array $params) {
    $this->realCultureFeedPages->defollow($id, $userId, $params);
    $this->clearUserListCache($id, CultureFeed_Pages_Follower::ROLE);
  }

  /**
   * @see CultureFeed_Pages::addAdmin()
   */
  public function addAdmin($id, $userId, $params = array()) {
    $this->realCultureFeedPages->addAdmin($id, $userId, $params);
    $this->clearUserListCache($id, CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN);
  }

  /**
   * @see CultureFeed_Pages::updateAdmin()
   */
  public function updateAdmin($id, $userId, array $params) {
    $this->realCultureFeedPages->updateMember($id, $userId, $params);
    $this->clearUserListCache($id, CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN);
  }

  /**
   * @see CultureFeed_Pages::removeAdmin()
   */
  public function removeAdmin($id, $userId) {
    $this->realCultureFeedPages->removeAdmin($id, $userId);
    $this->clearUserListCache($id, CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN);
  }

  /**
   * @see CultureFeed_Pages::getTimeline()
   */
  public function getTimeline($id, $dateFrom = NULL) {

    $cid = 'timeline:' . $id . ':' . $dateFrom;

    if ($cache = $this->cacheGet($cid)) {
      return $cache->data;
    }

    $data = $this->realCultureFeedPages->getTimeline($id, $dateFrom);

    $this->cacheSet($cid, $data, REQUEST_TIME + CULTUREFEED_CACHE_EXPIRES);

    return $data;

  }

}