<?php

/**
 * @file
 * Drush integration for the Culturefeed search ui module
 */

/**
 * Implements hook_drush_help().
 */
function culturefeed_search_ui_drush_help($section) {
  switch ($section) {
    case 'culturefeed_search_ui:culturefeed-search-register-deletions':
      return dt("Store deleted cdbis in a table, so we can trigger 410's.");
  }
}

/**
 * Implements hook_drush_command().
 */
function culturefeed_search_ui_drush_command() {

  $items['culturefeed-search-register-deletions'] = array(
    'description' => 'Store deleted cdbids.',
    'aliases' => array('csrd'),
  );

  return $items;
}

/**
 * Store deleted cdb ids in a table.
 */
function drush_culturefeed_search_ui_culturefeed_search_register_deletions() {
  culturefeed_search_ui_register_deletions();
}
