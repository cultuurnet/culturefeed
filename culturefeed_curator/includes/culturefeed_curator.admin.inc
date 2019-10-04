<?php
/**
 * @file
 * Settings file for culturefeed entry ui.
 */

/**
 * Callback for the CultureFeed Curator API settings form.
 */
function culturefeed_curator_api_settings_form($form, &$form_state) {

  $form['culturefeed_curator_api_endpoint'] = array(
    '#type' => 'textfield',
    '#title' => t('Curator API endpoint'),
    '#description' => t('The URL where the CultureFeed Curator API resides. End with a slash. Example: https://curator-test.uitdatabank.be/ (test) or https://curator.uitdatabank.be/ (production)'),
    '#default_value' => variable_get('culturefeed_curator_api_endpoint', CULTUREFEED_CURATOR_API_ENDPOINT),
  );

  $form['culturefeed_curator_api_cache_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cache API requests'),
    '#default_value' => variable_get('culturefeed_curator_api_cache_enabled', TRUE),
  );

  return system_settings_form($form);

}
