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

  $last_registration_timestamp = variable_get('culturefeed_search_ui_last_deletion_registration', 0);
  if ($last_registration_timestamp) {
    $last_registration = date('Y-m-d', $last_registration_timestamp);
  }

  watchdog('culturefeed_search_ui', 'Fetching deleted objects from API for cleanup.', array(), WATCHDOG_INFO);

  $service = culturefeed_get_search_service();

  try {

    $deletions = $service->getDeletions($last_registration);
    $deletions = array_unique($deletions);
    print_R($deletions);
    foreach ($deletions as $cdbid) {

      $existing = db_query('SELECT cdbid FROM {culturefeed_search_deleted_objects} WHERE cdbid = :cdbid', array(':cdbid' => $cdbid))->fetchField();
      $update = ($existing) ? 'cdbid' : array();

      $record = array();
      $record['cdbid'] = $cdbid;
      $record['deleted'] = $_SERVER['REQUEST_TIME'];

      drupal_write_record('culturefeed_search_deleted_objects', $record, $update);

      watchdog('culturefeed_search_ui', 'Finished registration of deleted objects.', array(), WATCHDOG_INFO);
      variable_set('culturefeed_search_ui_last_deletion_registration', REQUEST_TIME);

    }

  } catch (Exception $e) {
    watchdog('culturefeed_search_ui', 'Failed registration of deleted objects. Retrying again on the next run.', array(), WATCHDOG_ERROR);
  }

}