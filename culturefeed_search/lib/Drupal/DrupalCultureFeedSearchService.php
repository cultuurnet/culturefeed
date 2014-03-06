<?php
/**
 * @file
 * DrupalCultureFeedSearchService
 */

use \CultuurNet\Auth\ConsumerCredentials;
use \CultuurNet\Search\Parameter;
use \CultuurNet\Search\ServiceInterface;

/**
 * Singleton for the CultureFeed Search Service.
 */
class DrupalCultureFeedSearchService implements ServiceInterface {

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

    $endpoint = variable_get('culturefeed_search_api_location', CULTUREFEED_SEARCH_API_LOCATION);
    $service = new \CultuurNet\Search\Guzzle\Service($endpoint, $consumerCredentials);

    module_invoke_all('culturefeed_search_service_created', $service);

    if (module_exists('culturefeed_devel')) {
      $service->enableLogging();
    }

    if (variable_get('culturefeed_search_cache_enabled', FALSE)) {
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
   * @return self
   */
  public static function getClient(ConsumerCredentials $consumerCredentials) {
    if (!self::$searchService) {
      self::$searchService = new DrupalCultureFeedSearchService($consumerCredentials);
    }

    return self::$searchService;
  }

  /**
   * @see \CultuurNet\Search\Service::search().
   */
  public function search($parameters = array()) {

    DrupalCultureFeedSearchService::addLanguageParameter($parameters);
    $items = $this->service->search($parameters);
    DrupalCultureFeedSearchService::translateCategories($items);

    return $items;

  }

  /**
   * @see \CultuurNet\Search\Service::search().
   */
  public function searchPages($parameters = array()) {
    return $this->service->searchPages($parameters);
  }

  /**
   * @see \CultuurNet\Search\Service::searchSuggestions().
   */
  public function searchSuggestions($search_string, $types = array(), $past = FALSE) {
    return $this->service->searchSuggestions($search_string, $types, $past);
  }

  public function detail($type, $id) {
    return $this->service->detail($type, $id);
  }

  /**
   * Adds the language parameter to the search.
   */
  public static function addLanguageParameter(&$parameters) {
    $parameters[] = new Parameter\FilterQuery('language:' . culturefeed_search_get_preferred_language());
  }

  /**
   * Translates the categories.
   */
  public static function translateCategories(CultuurNet\Search\SearchResult $result) {

    $items = $result->getItems();
    $tids = array();
    foreach ($items as $item) {
      $categories = $item->getEntity()->getCategories();
      foreach ($categories as $category) {
        $categoryId = is_object($category) ? $category->getId() : $category;
        $tids[$categoryId] = $categoryId;
      }
    }

    // Translate the labels.
    if (culturefeed_search_term_translations($tids)) {
      foreach ($items as $item) {
        $categories = $item->getEntity()->getCategories();
        foreach ($categories as $category) {
          if (is_object($category)) {
            $category->setName(culturefeed_search_get_term_translation($category->getId()));
          }
        }
      }
    }
  }

}
