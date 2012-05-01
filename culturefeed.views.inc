<?php

/**
 * @file
 * Views integration.
 */

/**
 * Implements hook_views_data_alter().
 */
function culturefeed_views_data_alter(&$data) {
  $data['cnapievents']['cityregion']['argument'] = array(
    'handler' => 'culturefeed_handler_argument_cnapi_cityregion',
  );
}