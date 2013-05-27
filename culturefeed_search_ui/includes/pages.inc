<?php
/**
 * @file
 * Defines page callback to render the search results.
 */

use \Guzzle\Http\Exception;

/**
 * Page callback to search.
 *
 * @param String $name
 *   The name of the searchable type.
 * @param String $class_name
 *   The name of class to be used to load the correct search page.
 */
function culturefeed_search_ui_search_page($name, $class_name = NULL) {

  // The first search page callback is a dummy callback to redirect to the
  // real search page.
  if ($name == "submitted") {
    return "";
  }

  try {
    $page = new $class_name();
    $page->setResultsPerPage(variable_get('culturefeed_search_ui_pager_items_per_page', CULTUREFEED_SEARCH_UI_PAGER_ITEMS_PER_PAGE));
    $page->setPagerType(variable_get('culturefeed_search_ui_pager_type', CULTUREFEED_SEARCH_UI_PAGER_NORMAL));
    return $page->loadPage();
  }
  catch (Exception\ClientErrorResponseException $e) {
    watchdog_exception('culturefeed_search_ui', $e);
    return "Er is een fout opgetreden tijdens het laden van de zoekresultaten.";
  }
  catch (Exception\CurlException $e) {
    watchdog_exception('culturefeed_search_ui', $e);
    return "Er is een fout opgetreden tijdens het laden van de zoekresultaten.";
  }
  catch (InvalidSearchPageException $e) {
    return $e->getMessage();
  }
  catch (CultureFeed_Cdb_ParseException $e) {
    watchdog_exception('culturefeed_search_ui', $e);
    return "Er is een fout opgetreden tijdens het laden van de zoekresultaten.";
  }
}

/**
 * Autcomplete callback for the suggestions.
 */
function culturefeed_search_ui_suggestions_autocomplete_page($search_string) {

  $searchService = culturefeed_get_search_service();
  $matches = array();

  if ($search_string) {

    try {

      // Get the list of suggestions from service.
      $result = $searchService->searchSuggestions($search_string);
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

}

/**
 * Page callback for the region suggestions autocomplete.
 */
function culturefeed_search_ui_region_suggestion_autocomplete_page($search_string) {

  $matches = array();

  if ($search_string) {

    try {

      // Get the list of suggestions from service.
      $suggestions = culturefeed_search_get_flandersregion_suggestions($search_string);
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
function culturefeed_search_ui_city_suggestion_autocomplete_page($search_string, $include_regions = TRUE) {

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
