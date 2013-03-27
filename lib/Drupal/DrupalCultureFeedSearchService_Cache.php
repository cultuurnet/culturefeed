<?php
/**
 * @file 
 * Drupal cache layer for the CultureFeed Search Service.
 */

/**
 * DrupalCultureFeedSearchService_Cache
 */
class DrupalCultureFeedSearchService_Cache {

  protected $loggedInUserId;

  protected $realSearchService;
  
  public function __construct(CultureFeed $realSearchService, $loggedInUserId) {
    $this->loggedInUserId = $loggedInUserId;
    $this->realSearchService = $realSearchService;
  }
  
  public function getRealCultureFeed() {
    return $this->realSearchService();
  }
  
  protected function getCachePrefix() {
    return 'culturefeed:results:';
  }
  
  protected function getCacheSuffix() {
    $consumer = $this->getConsumer();
    return sprintf(':%s:%s', $consumer->getKey(), $consumer->getSecret());
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
  
  protected function getConsumer() {
    $this->realSearchService->getClient()->getConsumer();
  }
  
  /**
   * Executes a search call to the CultureFeed Search API V2.
   */
  public function search() {
    
  }
  
}