<?php
/**
 * @file 
 * Drupal cache layer for the CultureFeed Search Service.
 */

use \CultuurNet\Search\Guzzle\Service;
use \CultuurNet\Auth\ConsumerCredentials;

/**
 * DrupalCultureFeedSearchService_Cache
 */
class DrupalCultureFeedSearchService_Cache {

  /**
   * @var Integer
   */
  protected $loggedInUserId;

  /**
   * @var \CultuurNet\Search\Guzzle\Service
   */
  protected $realSearchService;
  
  /**
   * Consumer.
   * @var \CultuurNet\Auth\ConsumerCredentials
   */
  protected $consumer;
  
  /**
   * Constructor.
   * 
   * @param Service $realSearchService
   * @param ConsumerCredentials $consumerCredentials
   * @param Integer $loggedInUserId
   */
  public function __construct(Service $realSearchService, 
      $consumerCredentials, $loggedInUserId) {
    $this->loggedInUserId = $loggedInUserId;
    $this->consumerCredentials = $consumerCredentials;
    $this->realSearchService = $realSearchService;
  }
  
  public function getRealSearchService() {
    return $this->realSearchService();
  }
  
  protected function getCachePrefix() {
    return 'culturefeed:results:';
  }
  
  protected function getCacheSuffix() {
    return sprintf(':%s:%s', $this->getConsumer()->getKey(), $this->getConsumer()->getSecret());
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
  
  /**
   * @return ConsumerCredentials
   */
  protected function getConsumer() {
    $this->consumer;
  }
  
  /**
   * Executes a search call to the CultureFeed Search API V2.
   */
  public function search(Array $parameters = array()) {
    return $this->realSearchService->search($parameters);
  }
  
}