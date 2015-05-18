<?php
/**
 * @file
 * Defines page callback to render the search results.
 */

use \Guzzle\Http\Exception;
use \Guzzle\Http\Client;

/**
 * Autcomplete callback for the suggestions on Cdb items.
 */
function culturefeed_search_suggestions_autocomplete_page($search_string, $past = FALSE, $type = NULL, $all = FALSE) {

  $matches = array();

  if ($search_string) {

    try {

      $types = !empty($type) ? array($type) : array();
      $extra_parameters[] = !empty($all) ? new \CultuurNet\Search\Parameter\Parameter('admin','true') : array();
      
      // Get the list of suggestions from service.
      $suggestions = culturefeed_get_search_service()->searchSuggestions($search_string, $types, $past, $extra_parameters);
      if ($suggestions->hasSuggestions()) {
        foreach ($suggestions as $suggestion) {
          $title = $location_title = check_plain($suggestion->getTitle());
          $location = $suggestion->getLocation();
          if (!empty($location)) {
            $location_title .= ' - <small class="muted">(' . $location . ')<small>';
          }
          $object = new stdClass();
          $object->title = $title;
          $object->key = $suggestion->getCdbId();
          $object->locationTitle = $location_title;
          $matches[$suggestion->getCdbId()] = $object;
        }
      }

    }
    catch (ClientErrorResponseException $e) {
      watchdog_exception('culturefeed_search', $e);
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_search', $e);
    }

  }

  drupal_json_output($matches);

}

/**
 * Page callback for the region suggestions autocomplete.
 */
function culturefeed_search_region_suggestion_autocomplete_page($search_string) {

  $matches = array();

  if ($search_string) {

    try {

      // Get the list of suggestions from service.
      $suggestions = culturefeed_search_get_flandersregion_suggestions($search_string, TRUE);
      foreach ($suggestions as $key => $suggestion) {
        $matches[$key] = check_plain($suggestion);
      }
    }
    catch (ClientErrorResponseException $e) {
      watchdog_exception('culturefeed_search', $e);
    }

  }

  drupal_json_output($matches);

}

/**
 * Page callback for the city suggestions autocomplete.
 */
function culturefeed_search_city_suggestion_autocomplete_page($search_string, $include_regions = TRUE) {

  $matches = array();

  if ($search_string) {

    // Get the list of suggestions from service.
    $suggestions = culturefeed_search_get_city_suggestions($search_string, $include_regions);
    foreach ($suggestions as $key => $suggestion) {
      $matches[$key] = check_plain($suggestion);
    }

  }

  drupal_json_output($matches);

}
