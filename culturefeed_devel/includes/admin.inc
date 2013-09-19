<?php

/**
 * @file
 * Administration forms for culturefeed devel.
 */

/**
 * General settings form.
 */
function culturefeed_devel_admin_settings_form() {

  $form = array();

  $form['culturefeed_devel_log_slow_request'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log slow requests'),
    '#description' => t('Requests that where slower then the entered milliseconds
       will be logged to the watchdog'),
    '#default_value' => variable_get('culturefeed_devel_log_slow_request', FALSE),
  );

  $form['culturefeed_devel_show_requests'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display full requests log'),
    '#description' => t('All requests will be shown in the footer of the page'),
    '#default_value' => variable_get('culturefeed_devel_show_requests', TRUE),
  );

  $form['culturefeed_devel_slow_request'] = array(
    '#type' => 'textfield',
    '#title' => t('Mark requests slower then'),
    '#description' => t('Queries slower then the entered milliseconds will be seen as slow'),
    '#default_value' => variable_get('culturefeed_devel_slow_request', CULTUREFEED_DEVEL_DEFAULT_SLOW_REQUEST),
  );

  return system_settings_form($form);
}

