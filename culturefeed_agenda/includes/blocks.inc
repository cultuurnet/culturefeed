<?php
/**
 * @file
 * Block callbacks for culturefeed agenda.
 */

/**
 * Block to show the related events from an item.
 */
function culturefeed_agenda_block_related_activities() {

  // Try to load the agenda detail.
  $item = culturefeed_agenda_get_active_object();
  if (!$item) {
    return;
  }

  $parameters = array();
  $parameters[] = new \CultuurNet\Search\Parameter\Query('*');
  // 1 extra then requested, because it's possible that current event is in the result.
  $parameters[] = new \CultuurNet\Search\Parameter\Rows(variable_get('agenda_related_activities_total_items', 5) + 1);
  $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery('type:event OR type:production');
  $parameters[] = new \CultuurNet\Search\Parameter\Group();

  // Add the context filter.
  $context = variable_get('agenda_related_activities_context');
  if ($context) {

    $categories = $item->getEntity()->getCategories()->getCategoriesByType($context);
    if ($categories) {
      $context_filter = array();
      foreach ($categories as $category) {
        $context_filter[] = $category->getId();
      }
      $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery('category_' . $context . '_id' . ':(' . implode(' OR ', $context_filter) . ')');
    }

  }

  // Add the sort.
  if ($sort_field = variable_get('agenda_related_activities_sort', '')) {
    $parameters[] = new \CultuurNet\Search\Parameter\Sort($sort_field, variable_get('agenda_related_activities_sort_direction', \CultuurNet\Search\Parameter\Sort::DIRECTION_ASC));
  }

  // Add extra query.
  if ($filter_query = variable_get('agenda_related_activities_extra_query', '')) {
    $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery($filter_query);
  }

  // Add spatial search.
  if ($item->getType() == 'event' && variable_get('agenda_related_activities_range', 0)) {

    $address = $item->getEntity()->getLocation()->getAddress();
    if ($address) {
      $coordinates = $address->getPhysicalAddress()->getGeoInformation();
      if ($coordinates) {
        $distance = new \CultuurNet\Search\Parameter\Spatial\Distance(variable_get('agenda_related_activities_range', 0));
        $point = new \CultuurNet\Search\Parameter\Spatial\Point($coordinates->getXCoordinate(), $coordinates->getYCoordinate());
        $field = new \CultuurNet\Search\Parameter\Spatial\SpatialField('physical_gis');
        $parameters[] = new \CultuurNet\Search\Parameter\Spatial\GeoFilterQuery($point, $distance, $field);
      }
    }

  }

  // Execute search.
  try {

    $searchService = culturefeed_get_search_service();
    $result = $searchService->search($parameters);

    if ($result->getTotalCount() == 0) {
      return;
    }

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_agenda', $e);
  }

  global $language;
  $search_results = $result->getItems();
  // Render the results.
  $items = array();
  foreach ($search_results as $search_result) {

    // Don't add current activity to the list.
    if ($search_result->getEntity()->getCdbId() == $item->getEntity()->getCdbid()) {
      continue;
    }

    $items[] = theme('culturefeed_' . $search_result->getType() . '_short_summary', array('item' => $search_result));
  }

  $block = array();
  $block['subject'] = 'Gerelateerde activiteiten';
  $block['content'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
    '#class' => 'related-activities'
  );

  return $block;

}
