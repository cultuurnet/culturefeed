<?php

/**
 * @class
 * Drupal cache layer for the CultureFeed savedSearches Service.
 */
class DrupalCultureFeedSavedSearches_Cache implements CultureFeed_SavedSearches {

  /**
   * @var CultureFeed_SavedSearches_Default
   */
  protected $realCultureFeedSavedSearches;

  /**
   * Constructor.
   *
   * Culturefeed savedSearches instance to be called.
   * @param CultureFeed_SavedSearches $realCultureFeedSavedSearches
   */
  public function __construct(CultureFeed_SavedSearches $realCultureFeedSavedSearches) {
    $this->realCultureFeedSavedSearches = $realCultureFeedSavedSearches;
  }

  /**
   * Get the cache prefix.
   */
  protected function getCachePrefix() {
    return 'culturefeed:savedSearches:';
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
   * {@inheritdoc}
   */
  public function subscribe(CultureFeed_SavedSearches_SavedSearch $savedSearch, $use_auth = TRUE) {
    $this->realCultureFeedSavedSearches->subscribe($savedSearch);
    $this->cacheClear('list:' . DrupalCulturefeed::getLoggedInUserId() . ':', TRUE);
  }

  /**
   * {@inheritdoc}
   */
  public function unsubscribe($savedSearchId, $userId, $use_auth = TRUE) {
    $this->realCultureFeedSavedSearches->unsubscribe($savedSearchId, $userId);
    $this->cacheClear('list:' . DrupalCulturefeed::getLoggedInUserId() . ':', TRUE);
    $this->cacheClear('detail:' . $savedSearchId);
  }

  /**
   * {@inheritdoc}
   */
  public function changeFrequency($savedSearchId, $frequency) {
    $this->realCultureFeedSavedSearches->changeFrequency($savedSearchId, $frequency);
    $this->cacheClear('list:' . DrupalCulturefeed::getLoggedInUserId() . ':', TRUE);
    $this->cacheClear('detail:' . $savedSearchId);
  }

  /**
   * {@inheritdoc}
   */
  public function getList($allConsumers = FALSE) {

    $cid = 'list:' . DrupalCulturefeed::getLoggedInUserId() . ':' . $allConsumers;

    if ($cache = $this->cacheGet($cid)) {
      return $cache->data;
    }

    $data = $this->realCultureFeedSavedSearches->getList($allConsumers);

    $this->cacheSet($cid, $data, REQUEST_TIME + CULTUREFEED_CACHE_EXPIRES);

    return $data;

  }

  /**
   * {@inheritdoc}
   */
  public function getSavedSearch($savedSearchId) {

    $cid = 'detail:' . $savedSearchId;

    if ($cache = $this->cacheGet($cid)) {
      return $cache->data;
    }

    $data = $this->realCultureFeedSavedSearches->getSavedSearch($savedSearchId);

    $this->cacheSet($cid, $data, REQUEST_TIME + CULTUREFEED_CACHE_EXPIRES);

    return $data;

  }

}

