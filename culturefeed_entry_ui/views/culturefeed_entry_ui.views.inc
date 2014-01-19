<?php

/**
 * @file
 * Views integration.
 */

/**
 * Implements hook_views_data_alter().
 */
function culturefeed_entry_ui_views_data_alter(&$data) {
  $data['cdb_items']['operations'] = array(
    'title' => t('Operations'),
    'help' => t('Links to operations that can be performed'),
    'field' => array(
      'handler' => 'views_handler_field_cdb_item_operations',
    ),
  );
}
