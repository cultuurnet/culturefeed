<?php

/**
 * @file
 * Drush integration for the Culturefeed Search UI module.
 */

/**
 * Implements hook_drush_help().
 */
function culturefeed_search_ui_drush_help($section) {

  switch ($section) {
    case 'culturefeed_search:culturefeed-search-ui-register-deletions':
      return dt("Store deleted ids so 410 headers can be triggered");
  }
}

/**
 * Implements hook_drush_command().
 */
function culturefeed_search_ui_drush_command() {

  $items = array();

  $items['culturefeed-search-ui-register-deletions'] = array(
    'description' => 'Store deleted ids to trigger 410',
    'aliases' => array('csuird'),
  );

  return $items;
}

/**
 * Implements the 'culturefeed-search-ui-register-deletions' drush command.
 */
function drush_culturefeed_search_ui_register_deletions() {

  // Return for now. Waiting for a more performant solution on service side.
  //return;

  $last_check = variable_get('culturefeed_search_ui_last_deleted_check', 0);
  $last_check = 0;
  try {
    $client = culturefeed_get_search_service();
    $params = array();

    $params[] = new CultuurNet\Search\Parameter\Group();
    $params[] = new CultuurNet\Search\Parameter\Query('*:*');
    $params[] = new CultuurNet\Search\Parameter\Parameter('deletedsince', date('Y-m-d', $last_check));
    $params[] = new CultuurNet\Search\Parameter\Rows(1000);
    $result = $client->search($params, TRUE);
    dsm($result->getTotalCount());
    if ($result->getCurrentCount() > 0) {
      $items = $result->getItems();
      foreach ($items as $item) {
        $record = array(
          'cdbid' => $item->getId(),
          'type' => $item->getType(),
          'deleted' => $_SERVER['REQUEST_TIME'],
        );

        drupal_write_record('culturefeed_search_deleted_items', $record);
      }
    }

    variable_set('culturefeed_search_ui_last_deleted_check', $_SERVER['REQUEST_TIME']);
    watchdog('Finished the registration of deleted items', array(), WATCHDOG_INFO);
  }
  catch (Exception $e) {
    watchdog('culturefeed_search_ui', 'Error on search api while searching for last deletions. Message: !message', array('!message' => $e->getMessage()), WATCHDOG_ERROR);
  }
}
