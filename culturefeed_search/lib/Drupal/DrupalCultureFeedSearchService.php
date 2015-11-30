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

    $endpoint = variable_get(
      'culturefeed_search_api_location',
      CULTUREFEED_SEARCH_API_LOCATION
    );

    $cdbXmlVersion = variable_get(
      'culturefeed_search_cdb_version',
      CULTUREFEED_SEARCH_CDB_DEFAULT_VERSION
    );

    $service = new \CultuurNet\Search\Guzzle\Service(
      $endpoint,
      $consumerCredentials,
      NULL,
      $cdbXmlVersion
    );

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
    $result = $this->service->search($parameters);
    DrupalCultureFeedSearchService::translateCategories($result->getItems());

    DrupalCultureFeedSearchService::setDetailCache($result);

    return $result;
  }

  /**
   * @see \CultuurNet\Search\Service::search().
   */
  public function searchPages($parameters = array()) {

    $result = $this->service->searchPages($parameters);
    DrupalCultureFeedSearchService::setDetailCache($result);

    return $result;
  }

  /**
   * @see \CultuurNet\Search\Service::searchSuggestions().
   */
  public function searchSuggestions($search_string, $types = array(), $past = FALSE, $extra_parameters = array(), $max = null) {
    return $this->service->searchSuggestions($search_string, $types, $past, $extra_parameters, $max);
  }

  public function detail($type, $id) {
    $item = $this->service->detail($type, $id);
    DrupalCultureFeedSearchService::translateCategories(array($item));
    return $item;
  }

  /**
   * @see \CultuurNet\Search\ServiceInterface::getDeletions().
   */
  public function getDeletions($deleted_since = NULL, $rows = NULL, $start = NULL) {
    return $this->service->getDeletions($deleted_since, $rows, $start);
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
  public static function translateCategories($items = array()) {

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

  /**
   * Put found item in static cache of the detail item load.
   */
  public static function setDetailCache($result) {

    $static_cache = &drupal_static('culturefeed_search_item_load', array());
    $items = $result->getItems();
    foreach ($items as $item) {
      $static_cache[$item->getId()] = $item;
    }
  }

}
