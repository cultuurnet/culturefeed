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
  }

}

/**
 * Implements hook_drush_command().
 */
function culturefeed_search_drush_command() {
  $items['culturefeed-search-import-terms'] = array(
    'description' => 'Import all Culturefeed Search domains and terms.',
  );
  return $items;
}

/**
 * Implements the 'cnapi-import-values' drush command.
 */
function drush_culturefeed_import_terms() {

  drush_log('Starting import', 'success');

  $import = new CultureFeedDomainImport();
  $import->import();

  drush_log('Domains and terms are now imported', 'success');

}