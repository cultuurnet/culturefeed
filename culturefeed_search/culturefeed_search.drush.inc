<?php

/**
 * @file
 * Drush integration for the Culturefeed Search API module.
 */

/**
 * Implements hook_drush_help().
 */
function culturefeed_search_drush_help($section) {
  switch ($section) {
    case 'culturefeed_search:culturefeed-search-import-cities':
      return dt("Import all Culturefeed Search cities");
  }
}

/**
 * Implements hook_drush_command().
 */
function culturefeed_search_drush_command() {
  $items['culturefeed-search-import-cities'] = array(
    'description' => 'Import all Culturefeed Search cities.',
    'aliases' => array('csic'),
  );
  return $items;
}

/**
 * Implements the 'culturefeed-search-import-cities' drush command.
 */
function drush_culturefeed_search_import_cities() {
  drush_log('Starting import', 'success');
  $import = new Drupal\culturefeed_search\CultureFeedCityImport();
  $import->import();
  foreach ($import->logMessages as $message) {
    drush_log($message['message'], $message['code']);
  }
  drush_log('Cities are now imported', 'success');
}
