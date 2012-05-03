<?php

/**
 * @file
 * Views integration.
 */

/**
 * Implements hook_views_data_alter().
 */
function uitpas_ui_views_data_alter(&$data) {
  $data['cnapievents']['cityregion']['argument'] = array(
    'handler' => 'uitpas_ui_handler_argument_cnapi_cityregion',
  );
}