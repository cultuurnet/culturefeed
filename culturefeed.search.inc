<?php
/**
 * @file 
 * Leverages convenience function to perform global or specific searches.
 */

/**
 * Gets the search service as a singleton.
 *
 * @return \CultuurNet\Search\ServiceInterface
 */
function culturefeed_get_search_service() {

  $consumerCredentials = new \CultuurNet\Auth\ConsumerCredentials();
  $consumerCredentials->setKey(variable_get('culturefeed_api_application_key', ''));
  $consumerCredentials->setSecret(variable_get('culturefeed_api_application_secret', ''));

  return DrupalCultureFeedSearchService::getClient($consumerCredentials);

}

/**
 * Load the detail from an item on the search service.
 * @param $cdb_id
 *   Cdb id to load.
 * @return mixed
 *   FALSE if not found, otherwise CultuurNet\Search\ActivityStatsExtendedEntity.
 */
function culturefeed_search_item_load($cdb_id) {

  $search_service = culturefeed_get_search_service();

  $parameters = array();
  $parameters[] = new \CultuurNet\Search\Parameter\Group();
  $queryParameter = new \CultuurNet\Search\Parameter\Query($cdb_id);

  $queryParameter->setLocalParam('type', 'term');
  $queryParameter->setLocalParam('f', 'cdbid');
  $parameters[] = $queryParameter;
  $result = $search_service->search($parameters);

  $items = $result->getItems();

  return reset($items);

}

/**
 * @param string $type
 * @param CultureFeed_Cdb_Item_Base $item
 * @return array()
 */
function culturefeed_cdb_item_get_categories($type, CultureFeed_Cdb_Item_Base $item) {
  $categories = array();
  if ($item->getCategories()) {
    foreach ($item->getCategories() as $category) {
      if ($category->getType() == $type) {
        $categories[] = $category;
      }
    }
  }
  return $categories;
}
