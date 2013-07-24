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
function culturefeed_agenda_activity_suggestion_autocomplete_page($search_string) {
  
  $matches = array();

  if ($search_string) {

    try {

      // Get the list of suggestions from service.
      $result = culturefeed_get_search_service()->searchSuggestions($search_string, array('event', 'production'));
      if ($result->hasSuggestions()) {
        $suggestions = $result->getSuggestions();
        foreach ($suggestions as $suggestion) {
          $matches[$suggestion] = check_plain($suggestion);
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
