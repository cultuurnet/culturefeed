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
