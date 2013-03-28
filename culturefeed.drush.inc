<?php

/**
 * @file
 * Drush integration for the Culturefeed API module.
 */

/**
 * Implements hook_drush_help().
 */
function culturefeed_drush_help($section) {

  switch ($section) {
    case 'culturefeed:culturefeed-import-terms':
      return dt("Import all Culturefeed Search domains and terms");
  }

}

/**
 * Implements hook_drush_command().
 */
function culturefeed_drush_command() {
  $items['culturefeed-import-terms'] = array(
    'description' => 'Import all Culturefeed Search domains and terms.',
  );
  return $items;
}

/**
 * Implements the 'cnapi-import-values' drush command.
 */
function drush_culturefeed_import_terms() {
}