<?php
/**
 * @class
 * Drupal caching for culturefeed.
 */
class DrupalCultureFeed_Cache extends CultureFeed_ICultureFeedDecoratorBase {

  protected $loggedInUserId;

  /**
   * @var DrupalCultureFeedPages_Cache
   */
  protected $pages;
  protected $savedSearches;

  public function __construct(ICultureFeed $realCultureFeed, $loggedInUserId) {
    $this->loggedInUserId = $loggedInUserId;

    parent::__construct($realCultureFeed);
  }

  public function getRealCultureFeed() {
    return $this->realCultureFeed;
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
  
  protected function cacheClearTemplates() {
    $cid_base = 'template:';
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

  public function createActivity(CultureFeed_Activity $activity) {

    $result = parent::createActivity($activity);
    $this->cacheClearActivities();

    // Also clear the timelines.
    if (module_exists('culturefeed_pages')) {
      cache_clear_all('culturefeed:pages:timeline:', 'cache_culturefeed', TRUE);
    }

    // Also clear searches (people can search on user attend_users, like_users, ...
    if (module_exists('culturefeed_search')) {
      cache_clear_all('culturefeed:results:', 'cache_culturefeed_search', TRUE);
    }

    return $result;
  }

  public function updateActivity($id, $private) {
    parent::updateActivity($id, $private);

    $this->cacheClearActivities();
  }

  public function deleteActivity($id) {
    $result = parent::deleteActivity($id);
    module_invoke_all('culturefeed_social_activity_deleted', $result);
    $this->cacheClearActivities();

    // Also clear the timelines.
    if (module_exists('culturefeed_pages')) {
      cache_clear_all('culturefeed:pages:timeline:', 'cache_culturefeed', TRUE);
    }

    // Also clear searches (people can search on user attend_users, like_users, ...
    if (module_exists('culturefeed_search')) {
      cache_clear_all('culturefeed:results:', 'cache_culturefeed_search', TRUE);
    }

    return $result;
  }

  public function searchActivities(CultureFeed_SearchActivitiesQuery $query) {

    // If cache should be skipped, don't do cache_get.
    if (!$query->skipCache) {
      return parent::searchActivities($query);
    }

    $cid = sprintf('activity:activities:%s', md5(serialize($query->toPostData())));

    if ($cache = $this->cacheGet($cid)) {
      return $cache->data;
    }

    $data = parent::searchActivities($query);
    $this->cacheSet($cid, $data, REQUEST_TIME + CULTUREFEED_CACHE_EXPIRES);

    return $data;
  }

  public function searchActivityUsers($nodeId, $type, $contentType, $start = NULL, $max = NULL) {
    $cid = sprintf('activity:users:%s:%s:%s:%s:%s', $nodeId, $type, $contentType, $start, $max);

    if ($cache = $this->cacheGet($cid)) {
      return $cache->data;
    }

    $data = parent::searchActivityUsers($nodeId, $type, $contentType, $start, $max);

    $this->cacheSet($cid, $data, REQUEST_TIME + CULTUREFEED_CACHE_EXPIRES);

    return $data;
  }
  
  public function createTemplate(CultureFeed_Template $template) {
    parent::createTemplate($template);

    $this->cacheClearTemplates();
  }

  public function updateTemplate(CultureFeed_Template $template, $fields = array()) {
    parent::updateTemplate($template, $fields);

    $this->cacheClearTemplates();
  }

  public function deleteTemplate($id) {
    parent::deleteTemplate($id);

    $this->cacheClearTemplates();
  }

  public function createMailing(CultureFeed_Mailing $mailing) {
    $mailing = parent::createMailing($mailing);

    $this->cacheClearMailings();

    return $mailing;
  }

  public function updateMailing(CultureFeed_Mailing $mailing, $fields = array()) {
    parent::updateMailing($mailing, $fields);

    $this->cacheClearMailings();
  }

  public function disableMailing($id) {
    parent::disableMailing($id);

    $this->cacheClearMailings();
  }

  public function deleteMailing($id) {
    parent::deleteMailing($id);

    $this->cacheClearMailings();
  }

  public function getTopEvents($type, $max = 5) {
    $cid = sprintf('topevents:%s:%s', $type, $max);

    if ($cache = $this->cacheGet($cid)) {
      return $cache->data;
    }

    $data = parent::getTopEvents($type, $max);

    $this->cacheSet($cid, $data, REQUEST_TIME + CULTUREFEED_CACHE_EXPIRES);

    return $data;
  }

  public function getRecommendationsForUser($id, CultureFeed_RecommendationsQuery $query = NULL, $skip_cache = FALSE) {
    $cid = 'recommendation:user:' . $id . ':' . md5(serialize($query->toPostData()));

    if (!$skip_cache && $cache = $this->cacheGet($cid)) {
      return $cache->data;
    }

    $data = parent::getRecommendationsForUser($id, $query);

    $this->cacheSet($cid, $data, REQUEST_TIME + CULTUREFEED_CACHE_EXPIRES);

    return $data;
  }

  public function getRecommendationsForEvent($id, CultureFeed_RecommendationsQuery $query = NULL) {
    $cid = 'recommendation:event:' . $id . ':' . md5(serialize($query->toPostData()));

    if ($cache = $this->cacheGet($cid)) {
      return $cache->data;
    }

    $data = parent::getRecommendationsForEvent($id, $query);

    $this->cacheSet($cid, $data, REQUEST_TIME + CULTUREFEED_CACHE_EXPIRES);

    return $data;
  }

    public function evaluateRecommendation($id, $evaluation) {
      parent::evaluateRecommendation($id, $evaluation);

      $this->cacheClearRecommendations();

      if ($evaluation == CultureFeed::RECOMMENDATION_EVALUATION_POSITIVE) {
        $this->cacheClearActivities();
      }
    }

    /**
     * Get pages service.
     * @return DrupalCultureFeedPages_Cache
     */
    public function pages() {

      if (!$this->pages) {
        $this->pages = new DrupalCultureFeedPages_Cache(parent::pages());
      }

      return $this->pages;

    }

    /**
     * Get the savedSearches service.
     *
     * @return DrupalCultureFeedSavedSearches_Cache
     */
    public function savedSearches() {

      if (!$this->savedSearches) {
        $this->savedSearches = new DrupalCultureFeedSavedSearches_Cache($this->realCultureFeed->savedSearches());
      }

      return $this->savedSearches;

    }

}
