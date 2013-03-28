<?php

/**
 * @file
 * Callback and handler implementations for administration pages.
 */

/**
 * Configuration form for general settings.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function culturefeed_admin_settings() {
  $form['culturefeed_api_location'] = array(
    '#type' => 'textfield',
    '#title' => t('API location'),
    '#description' => t('The URL where the CultureFeed API resides. End with a slash. Example: http://test.uitid.be/culturefeed/rest/'),
    '#default_value' => variable_get('culturefeed_api_location', CULTUREFEED_API_LOCATION),
  );
  $form['culturefeed_search_api_location'] = array(
    '#type' => 'textfield',
    '#title' => t('Search API location'),
    '#description' => t('The URL where the CultureFeed Search API resides. End with a slash. Example: http://test.uitid.be/uitid/rest/searchv2/'),
    '#default_value' => variable_get('culturefeed_search_api_location', CULTUREFEED_SEARCH_API_LOCATION),
  );
  $form['culturefeed_api_application_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Application key'),
    '#description' => t('Your Culture Feed Application key.'),
    '#default_value' => variable_get('culturefeed_api_application_key', ''),
    '#size' => 40,
    '#maxlength' => 40,
  );
  $form['culturefeed_api_shared_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Shared secret'),
    '#description' => t('Your Culture Feed Shared Secret.'),
    '#default_value' => variable_get('culturefeed_api_shared_secret', ''),
    '#size' => 40,
    '#maxlength' => 40,
  );
  $form['culturefeed_entry_api_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Entry API path'),
    '#description' => t('The path where the Culture Feed Entry API resides. End with a slash. Example: entry/test.rest.uitdatabank.be/api/v1/'),
    '#default_value' => variable_get('culturefeed_entry_api_path', CULTUREFEED_ENTRY_API_PATH),
  );

  return system_settings_form($form);
}
