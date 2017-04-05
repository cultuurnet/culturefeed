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
    '#description' => t('The URL where the CultureFeed API resides. End with a slash. Example: http://acc.uitid.be/uitid/rest/'),
    '#default_value' => variable_get('culturefeed_api_location', CULTUREFEED_API_LOCATION),
  );
  $form['culturefeed_api_application_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Application key'),
    '#description' => t('Your CultureFeed Application key.'),
    '#default_value' => variable_get('culturefeed_api_application_key', ''),
    '#size' => 40,
    '#maxlength' => 40,
  );
  $form['culturefeed_api_shared_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Shared secret'),
    '#description' => t('Your CultureFeed Shared Secret.'),
    '#default_value' => variable_get('culturefeed_api_shared_secret', ''),
    '#size' => 40,
    '#maxlength' => 40,
  );

  $form['culturefeed_entry_api_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Entry API path'),
    '#description' => t('The path where the CultureFeed Entry API resides. End with a slash. Example: entry/test.rest.uitdatabank.be/api/v2/'),
    '#default_value' => variable_get('culturefeed_entry_api_path', CULTUREFEED_ENTRY_API_PATH),
  );

  $form['culturefeed_api_uitid_light_permission'] = array(
    '#type' => 'checkbox',
    '#title' => t('My consumer has UiTiD light permissions.'),
    '#default_value' => CULTUREFEED_API_LIGHT_ID_ALLOWED,
  );

  $cdb_xml_versions = culturefeed_cdb_xml_versions();
  $form['culturefeed_cdb_version'] = array(
    '#default_value' => variable_get('culturefeed_cdb_version', CULTUREFEED_CDB_DEFAULT_VERSION),
    '#description' => t('The culturefeed cdb schema version used by the entry API.'),
    '#title' => t('Cdb schema version'),
    '#type' => 'select',
    '#options' => array_combine($cdb_xml_versions, $cdb_xml_versions),
  );

  $form['culturefeed_http_client_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('HTTP client timeout'),
    '#description' => t('Timeout on requests by the HTTP client.'),
    '#default_value' => variable_get('culturefeed_http_client_timeout', 10),
    '#field_suffix' => t('seconds'),
  );

  $form['culturefeed_composer_lock_path'] = array(
    '#type' => 'textfield',
    '#title' => 'Path to the composer.lock file.',
    '#description' => t('Use "." (without quotes) for the root directory of the Drupal installation.'),
    '#default_value' => variable_get('culturefeed_composer_lock_path', CULTUREFEED_DEFAULT_COMPOSER_LOCK_PATH),
    '#field_suffix' => '/composer.lock',
  );

  return system_settings_form($form);
}
