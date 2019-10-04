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

  /**
   * Get the curator cache prefix.
   *
   * @return string
   */
  protected function getCachePrefix() {
    return 'culturefeed:curator:';
  }

  /**
   * Get the curator cache suffix.
   *
   * @return string
   */
  protected function getCacheSuffix() {
    // Don't cache per user. Curator requests are the same for every user. (for now)
    return '';
  }

  /**
   * Add prefix/suffix to given cid.
   *
   * @param $cid
   *
   * @return string
   */
  protected function getCacheCid($cid) {
    return self::getCachePrefix() . $cid . self::getCacheSuffix();
  }

  /**
   * Cache data into curator cache.
   *
   * @param $cid
   * @param $data
   * @param int $expires
   */
  protected function cacheSet($cid, $data, $expires = CACHE_PERMANENT) {
    $cid = $this->getCacheCid($cid);
    cache_set($cid, $data, 'cache_culturefeed_curator', $expires);
  }

  /**
   * Get cached data from curator cache.
   *
   * @param $cid
   *
   * @return mixed
   */
  protected function cacheGet($cid) {
    $cid = $this->getCacheCid($cid);
    return cache_get($cid, 'cache_culturefeed_curator');
  }

  /**
   * Clear CultureFeed curator cache.
   *
   * @param null $cid
   * @param bool $wildcard
   */
  protected function cacheClear($cid = NULL, $wildcard = FALSE) {
    cache_clear_all($cid, 'cache_culturefeed_curator', $wildcard);
  }

  /**
   * Get external articles for a given CDB ID.
   *
   * @param $cdb_id
   *
   * @return \CultureFeed_CuratorArticle[]
   */
  public function getExternalArticlesForCdbItem($cdb_id) {

    $cid = 'news_articles:' . md5($cdb_id);
    if ($cache = $this->cacheGet($cid)) {
      return $cache->data;
    }

    $results = $this->realCuratorClient->getExternalArticlesForCdbItem($cdb_id);

    $this->cacheSet($cid, $results, REQUEST_TIME + CULTUREFEED_CURATOR_API_CACHE_EXPIRES);

    return $results;

  }

}