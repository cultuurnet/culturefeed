<?php
/**
 * @file
 * Defines page callbacks which access the Search api.
 */

/**
 * Page callback to show the detail from an agenda item.
 * @param \CultuurNet\Search\ActivityStatsExtendedEntity $item
 *   Extended entity to view.
 */
function culturefeed_agenda_detail_page(\CultuurNet\Search\ActivityStatsExtendedEntity $item) {

  // Hardcoded breadcrumb requested. Use active trail, so breadcrumb can be altered.

  if ($item->getType() == 'actor') {
    culturefeed_agenda_set_actor_breadcrumb($item);
  }
  else {
    culturefeed_agenda_set_activity_breadcrumb($item);
  }

  return array(
    '#theme' => 'culturefeed_' . $item->getType(),
    '#item' => $item,
  );

}

/**
 * Page callback to do an autocomplete search on activity titles.
 */
function culturefeed_agenda_activity_suggestion_autocomplete_page($search_string, $past = FALSE) {

  $matches = array();

  if ($search_string) {

    try {

      // Get the list of suggestions from service.
      $suggestions = culturefeed_get_search_service()->searchSuggestions($search_string, array('event', 'production'), $past);
      if ($suggestions->hasSuggestions()) {
        foreach ($suggestions as $suggestion) {
          $matches[$suggestion->getTitle()] = check_plain($suggestion->getTitle());
        }
      }

    }
    catch (ClientErrorResponseException $e) {
      watchdog_exception('culturefeed_search', $e);
    }

  }

  drupal_json_output($matches);

  // Cache if possible per request.
  drupal_page_footer();
}

/**
 * Menu callback return 'de lijn' widget.
 *
 * @param \CultuurNet\Search\ActivityStatsExtendedEntity $item
 *   Item to show 'lijn' info for.
 */
function culturefeed_agenda_page_de_lijn_widget(\CultuurNet\Search\ActivityStatsExtendedEntity $item) {

  $element = array(
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'robots',
      'content' => 'noindex, follow',
    ),
  );
  drupal_add_html_head($element, 'nofollow');

  return array(
    '#theme' => 'culturefeed_agenda_de_lijn_widget',
    '#item' => $item,
    '#attached' => array(
      'css' => array(
        array('data' => 'http://pluggable.reisinfo.delijn.be/routeplannerPluggable/css/pluggableRouteplanner.css', 'type' => 'external'),
      ),
      'js' => array(
        array('data' => 'http://pluggable.reisinfo.delijn.be/routeplannerPluggable/js/calendar.js', 'type' => 'external'),
        array('data' => 'http://pluggable.reisinfo.delijn.be/routeplannerPluggable/dwr/interface/ReisAdvies.js', 'type' => 'external'),
        array('data' => 'http://pluggable.reisinfo.delijn.be/routeplannerPluggable/dwr/engine.js', 'type' => 'external'),
        array('data' => 'http://pluggable.reisinfo.delijn.be/routeplannerPluggable/js/dojo/dojo.xd.js', 'type' => 'external'),
        array('data' => 'http://pluggable.reisinfo.delijn.be/routeplannerPluggable/js/dojo/pluggableRP.xd.js', 'type' => 'external'),
      ),
    ),
  );
}

/**
 * Menu callback to show current activity on a map.
 * @param \CultuurNet\Search\ActivityStatsExtendedEntity $item
 *   Item to show the map for.
 */
function culturefeed_agenda_page_map(\CultuurNet\Search\ActivityStatsExtendedEntity $item) {

  $element = array(
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'robots',
      'content' => 'noindex, follow',
    ),
  );
  drupal_add_html_head($element, 'nofollow');

  return culturefeed_agenda_get_map_render_array($item);
}

/**
 * Ajax callback: load the related activities for an item.
 */
function culturefeed_agenda_page_ajax_related_activities($cdb_id) {

  $data = '';
  $item = culturefeed_search_item_load($cdb_id);
  if ($item) {

    $total_items = variable_get('agenda_related_activities_total_items', 3);
    $parameters = array();
    $parameters[] = new \CultuurNet\Search\Parameter\Query('*:*');
    // 1 extra then requested, because it's possible that current event is in the result.
    $parameters[] = new \CultuurNet\Search\Parameter\Rows($total_items + 1);
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
        $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery('category_id' . ':(' . implode(' OR ', $context_filter) . ')');
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
      if ($address && $address->getPhysicalAddress()) {
        $coordinates = $address->getPhysicalAddress()->getGeoInformation();
        if ($coordinates) {
          $distance = new \CultuurNet\Search\Parameter\Spatial\Distance(variable_get('agenda_related_activities_range', 0));
          $point = new \CultuurNet\Search\Parameter\Spatial\Point($coordinates->getYCoordinate(), $coordinates->getXCoordinate());
          $field = new \CultuurNet\Search\Parameter\Spatial\SpatialField('physical_gis');
          $parameters[] = new \CultuurNet\Search\Parameter\Spatial\GeoFilterQuery($point, $distance, $field);
        }
      }
    }

    // Execute search.
    try {

      $result = culturefeed_get_search_service()->search($parameters);
      if ($result->getTotalCount() >= 0) {

        $search_results = $result->getItems();

        if(module_exists('culturefeed_social')) {
          culturefeed_social_warmup_activities_cache($search_results);
        }

        // Render the results.
        $items = array();
        $count = 0;
        foreach ($search_results as $search_result) {

          // Don't add current activity to the list.
          if ($search_result->getEntity()->getCdbId() == $item->getEntity()->getCdbid()) {
            continue;
          }

          // If current activity was not in results. Make sure we show only the request amount.
          if ($count < $total_items) {
            $items[] = $search_result;
          }

          $count++;

          $output = array(
            '#theme' => 'culturefeed_agenda_related_activities',
            '#results' => $items,
          );

          $data = drupal_render($output);
        }
      }
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_agenda', $e);
    }

  }

  $commands = array();
  $commands[] = ajax_command_html('#related-activities', $data);

  ajax_deliver(array('#type' => 'ajax', '#commands' => $commands));

}

function culturefeed_agenda_actor_search_autocomplete($search, $past) {

  $matches = array();

  if ($search) {

    try {

      // Get the list of suggestions from service.
      $suggestions = culturefeed_get_search_service()->searchSuggestions($search, array('actor'), $past);
      // @todo: only use suggestions for the actortypes saved in block configure.
      if ($suggestions->hasSuggestions()) {
        foreach ($suggestions as $suggestion) {
          $path = culturefeed_search_detail_url('actor', $suggestion->getCdbid(), $suggestion->getTitle());
          $matches[$path] = check_plain($suggestion->getTitle());
        }
      }

    }
    catch (ClientErrorResponseException $e) {
      watchdog_exception('culturefeed_agenda', $e);
    }

  }
  drupal_json_output($matches);
}
