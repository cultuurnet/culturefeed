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
    case 'culturefeed_search:culturefeed-search-import-cnapi-headings':
      return dt("Import all cnapi headings to be used for 301 redirects from old cnapi urls");
    case 'culturefeed_search:culturefeed-search-cache-clear':
      return dt("Clear all culturefeed search cache");
  }

}

/**
 * Implements hook_drush_command().
 */
function culturefeed_search_drush_command() {
  $items['culturefeed-search-import-terms'] = array(
    'description' => 'Import all Culturefeed Search domains and terms.',
    'aliases' => array('csit'),
  );
  $items['culturefeed-search-import-cities'] = array(
    'description' => 'Import all Culturefeed Search cities.',
    'aliases' => array('csic'),
  );
  $items['culturefeed-search-import-cnapi-headings'] = array(
    'description' => 'Import all old cnapi headings.',
    'aliases' => array('csicnapi'),
  );
  $items['culturefeed-search-cache-clear'] = array(
    'description' => 'Clear all culturefeed search cache.',
    'aliases' => array('cs-cc'),
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

/**
 * Implements the 'culturefeed-search-import-cnapi-headings' drush command.
 */
function drush_culturefeed_search_import_cnapi_headings() {

  drush_log('Starting import', 'success');

  $import = new CnapiHeadingImport();
  $import->import();

  drush_log('Cnapi headings are now imported', 'success');

}

/**
 * Clears all culturefeed search cache.
 */
function drush_culturefeed_search_cache_clear() {
  if (is_callable('_culturefeed_search_flush_caches')) {
    _culturefeed_search_flush_caches();
  }
}
