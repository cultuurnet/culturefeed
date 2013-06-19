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
    case 'culturefeed_search:culturefeed-search-import-terms':
      return dt("Import all Culturefeed Search domains and terms");
    case 'culturefeed_search:culturefeed-search-import-cities':
      return dt("Import all Culturefeed Search cities");
  }

}

/**
 * Implements hook_drush_command().
 */
function culturefeed_search_drush_command() {
  $items['culturefeed-search-import-terms'] = array(
    'description' => 'Import all Culturefeed Search domains and terms.',
  );
  $items['culturefeed-search-import-cities'] = array(
    'description' => 'Import all Culturefeed Search cities.',
  );
  return $items;
}

/**
 * Implements the 'culturefeed-search-import-terms' drush command.
 */
function drush_culturefeed_search_import_terms() {

  drush_log('Starting import', 'success');

  $import = new CultureFeedDomainImport();
  $import->import();
  
  foreach ($import->logMessages as $message) {
    drush_log($message['message'], $message['code']);
  }

  drush_log('Domains and terms are now imported', 'success');

}

/**
 * Implements the 'culturefeed-search-import-cities' drush command.
 */
function drush_culturefeed_search_import_cities() {

  drush_log('Starting import', 'success');

  $import = new CultureFeedCityImport();
  $import->import();
  
  foreach ($import->logMessages as $message) {
    drush_log($message['message'], $message['code']);
  }

  drush_log('Cities are now imported', 'success');

}