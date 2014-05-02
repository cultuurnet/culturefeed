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
   * Get the full cache id.
   * @param string $cid
   *   Cid to use in the full cache id.
   * @return string
   */
  protected function getCacheCid($cid) {
    return self::getCachePrefix() . $cid;
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
    $full_cid = $this->getCacheCid($cid);
    cache_clear_all($full_cid, 'cache_culturefeed', $wildcard);
  }

  /**
   * Clear the userlist cache.
   * @param string $id
   *   Page id.
   * @param string $memberType
   *   Member type ex ADMIN
   */
  protected function clearUserListCache($id, $memberType) {
    $this->cacheClear('userList:' . $id, TRUE);
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
    $pageId = $this->realCultureFeedPages->addPage($params);
    return $pageId;
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

    // Also flush search results.
    cache_clear_all('culturefeed:results:search/page:', 'cache_culturefeed_search', TRUE);

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
   * @see CultureFeed_Pages::removeImage()
   */
  public function removeImage($id) {
    $result = $this->realCultureFeedPages->removeImage($id);
    $this->cacheClear('page:' . $id);
  }

  /**
   * @see CultureFeed_Pages::addCover()
   */
  public function addCover($id, array $params) {
    $result = $this->realCultureFeedPages->addCover($id, $params);
    $this->cacheClear('page:' . $id);
    return $result;
  }

  /**
   * @see CultureFeed_Pages::removeCover()
   */
  public function removeCover($id) {
    $result = $this->realCultureFeedPages->removeCover($id);
    $this->cacheClear('page:' . $id);
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
    $this->realCultureFeedPages->removeMember($id, $userId);
    $this->clearUserListCache($id, CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_MEMBER);
  }

  /**
   * @see CultureFeed_Pages::follow()
   */
  public function follow($id, array $params = array()) {
    $this->realCultureFeedPages->follow($id, $params);
    $this->clearUserListCache($id, CultureFeed_Pages_Follower::ROLE);
  }

  /**
   * @see CultureFeed_Pages::defollow()
   */
  public function defollow($id, $userId, array $params = array()) {
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
    $this->realCultureFeedPages->updateAdmin($id, $userId, $params);
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
  public function getTimeline($id, $dateFrom = NULL, $type = '') {

    $cid = 'timeline:' . $id . ':' . $dateFrom . ':' . $type;

    if ($cache = $this->cacheGet($cid)) {
      return $cache->data;
    }

    $data = $this->realCultureFeedPages->getTimeline($id, $dateFrom, $type);

    $this->cacheSet($cid, $data, REQUEST_TIME + CULTUREFEED_CACHE_EXPIRES);

    return $data;

  }

  /**
   * @see CultureFeed_Pages::getNotifications()
   */
  public function getNotifications($id, $params = array()) {
    return $this->realCultureFeedPages->getNotifications($id, $params);
  }

}