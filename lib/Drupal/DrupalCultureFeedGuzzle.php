<?php

/**
 * @file 
 * DrupalCultureFeedGuzzle
 */

/**
 * Singleton for the CultureFeed Search Service.
 */
class DrupalCultureFeedSearchService {

  /**
   * 
   * @var unknown_type
   */
  private static $service = NULL;
  
  /**
   * 
   * @param \CultuurNet\Auth\ConsumerCredentials $consumerCredentials
   * @return Ambigous <CultureFeed, DrupalCultureFeed_Cache>
   */
  public static function getClient(\CultuurNet\Auth\ConsumerCredentials $consumerCredentials) {

    if (!self::$service) {
      $endpoint = variable_get('culturefeed_api_location_v2', CULTUREFEED_API_LOCATION_V2);
      $service = new \CultuurNet\Search\Guzzle\Service($endpoint, $consumerCredentials);
      
      if (self::isCacheEnabled()) {
        self::$service = new DrupalCultureFeedSearchService_Cache($service, 
            DrupalCultureFeed::getLoggedInUserId());
      }
    }
    
    return self::$service;
    
  }
  
  /**
   * Executes a search call to the CultureFeed Search API V2.
   */
  public function search() {
    
  }
  
}
