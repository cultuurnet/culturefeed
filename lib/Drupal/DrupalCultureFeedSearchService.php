<?php
/**
 * @file
 * DrupalCultureFeedSearchService
 */

use \CultuurNet\Auth\ConsumerCredentials;

/**
 * Singleton for the CultureFeed Search Service.
 */
class DrupalCultureFeedSearchService {

  /**
   * @var DrupalCultureFeedSearchService
   */
  private static $searchService = NULL;
  
  /**
   * @var \CultuurNet\Search\Guzzle\Service
   */
  private $service = NULL;
  
  /**
   * Constructor
   * 
   * @param ConsumerCredentials $consumerCredentials
   */
  private function __construct(ConsumerCredentials $consumerCredentials) {
    
    $endpoint = variable_get('culturefeed_api_location_v2', CULTUREFEED_API_LOCATION_V2);
    $service = new \CultuurNet\Search\Guzzle\Service($endpoint, $consumerCredentials);

    if (variable_get('culturefeed_cache_status', CULTUREFEED_CACHE_DISABLED) == CULTUREFEED_CACHE_ENABLED) {
      $this->service = new DrupalCultureFeedSearchService_Cache($service,
        $consumerCredentials,
        DrupalCultureFeed::getLoggedInUserId());
    }
    else {
      $this->service = $service;
    }
    
  }
  
  /**
   * getClient().
   *
   * @param ConsumerCredentials $consumerCredentials
   * @return Ambigous <CultureFeed, DrupalCultureFeed_Cache>
   */
  public static function getClient(ConsumerCredentials $consumerCredentials) {
    if (!self::$searchService) {
      self::$searchService = new DrupalCultureFeedSearchService($consumerCredentials);
    }
    
    return self::$searchService;
  }

  /**
   * Executes a search call to the CultureFeed Search API V2.
   */
  public function search(Array $parameters = array()) {
    return $this->service->search($parameters);
  }

}
