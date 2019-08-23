<?php
/**
 * @file
 * Drupal cache layer for the CultureFeed Curator Service.
 */

/**
 * DrupalCultureFeedCuratorClient_Cache
 */
class DrupalCultureFeedCuratorClient_Cache extends DrupalCultureFeedCuratorClient {

  /**
   * @var Integer
   */
  protected $loggedInUserId;

  /**
   * @var \DrupalCultureFeedCuratorClient
   */
  protected $realCuratorClient;

  /**
   * Constructor.
   *
   * @param \DrupalCultureFeedCuratorClient $realCuratorClient
   * @param Integer $loggedInUserId
   */
  public function __construct(DrupalCultureFeedCuratorClient $realCuratorClient, $loggedInUserId) {
    $this->loggedInUserId = $loggedInUserId;
    $this->realCuratorClient = $realCuratorClient;
  }

  protected function getCachePrefix() {
    return 'culturefeed:curator:';
  }

  protected function getCacheSuffix() {
    // Don't cache per user. Search is the same for every user. (for now)
    return '';
  }

  protected function getCacheCid($cid) {
    return self::getCachePrefix() . $cid . self::getCacheSuffix();
  }

  protected function cacheSet($cid, $data, $expires = CACHE_PERMANENT) {
    $cid = $this->getCacheCid($cid);
    cache_set($cid, $data, 'cache_culturefeed_curator', $expires);
  }

  protected function cacheGet($cid) {
    $cid = $this->getCacheCid($cid);
    return cache_get($cid, 'cache_culturefeed_curator');
  }

  protected function cacheClear($cid = NULL, $wildcard = FALSE) {
    cache_clear_all($cid, 'cache_culturefeed_curator', $wildcard);
  }

  public function getExternalArticlesForCdbItem($cdb_id) {

    $cid = 'news_articles:' . md5($cdb_id);
    if ($cache = $this->cacheGet($cid)) {
      return $cache->data;
    }

    $request = $this->realCuratorClient->getExternalArticlesForCdbItem($cdb_id);

    // TODO: see DrupalCultureFeedClient
    $result = 'TODO';

    $this->cacheSet($cid, $result, REQUEST_TIME + CULTUREFEED_CURATOR_API_CACHE_EXPIRES);

    return $result;

  }

}