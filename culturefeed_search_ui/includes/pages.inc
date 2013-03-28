<?php
/**
 * @file
 * Defines page callback to render the search results.
 */

/**
 * Page callback to search.
 * 
 * @param String $name
 *   The name of the searchable type.
 * @param String $class_name
 *   The name of class to be used to load the correct search page.
 */
function culturefeed_search_ui_search_page($name, $class_name) {
  try {
    return $class_name::loadPage();
  }
  catch (InvalidSearchPageException $e) {
    watchdog('culturefeed_search_ui', "De zoekpagina kon niet geladen worden.", array(), WATCHDOG_CRITICAL);
    return "De zoekpagina kon niet geladen worden.";
  }
}
