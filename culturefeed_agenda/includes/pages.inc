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

  global $language;
  module_load_include('inc', 'culturefeed_agenda', 'theme/theme');
  $variables = array(
    'item' => $item,
    'title' => $item->getTitle($language->language),
    'link' => culturefeed_search_detail_l($item->getType(), $item->getId(), $item->getTitle($language->language)),
  );

  switch ($item->getType()) {

    case 'event':
      _culturefeed_agenda_preprocess_event($variables);
      break;

    case 'actor':
      _culturefeed_agenda_preprocess_actor($variables);
      break;
  }

  // Fetch the user info
  $cf_account = NULL;
  try {
    $cf_account = DrupalCultureFeed::getLoggedInUser();
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_agenda', $e);
  }

  if ($cf_account) {
    $variables['to_address'] = $cf_account->street . ', ' . $cf_account->zip . ' ' . $cf_account->city;
  }

  drupal_set_breadcrumb(array(l(t('Home'), NULL), $variables['link'], 'Stratenplan'));

  $query = array(
    'sensor' => 'true',
    'v' => '3.5',
    'region' => 'BE',
    'hl' => $language->language,
  );

  unset($variables['item']);

  $attached_js = array(
    array('data' => CULTUREFEED_GOOGLE_MAPS_JS_API, 'type' => 'external'),
    drupal_get_path('module', 'culturefeed_agenda') . '/js/map.js',
  );

  $attached_js[] = array(
    'type' => 'setting',
    'data' => array('culturefeed_map' => array('info' => $variables)),
  );

  return array(
    '#theme' => 'culturefeed_agenda_travel_map',
    '#attached' => array(
      'js' => $attached_js,
    ),
  );
}
