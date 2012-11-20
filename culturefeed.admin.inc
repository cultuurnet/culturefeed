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
  // general api settings
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configuration'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['settings']['culturefeed_api_location'] = array(
    '#type' => 'textfield',
    '#title' => t('API location'),
    '#description' => t('The URL where the CultureFeed API resides. End with a slash. Example: http://test.uitid.be/culturefeed/rest/'),
    '#default_value' => variable_get('culturefeed_api_location', CULTUREFEED_API_LOCATION),
  );
  $form['settings']['culturefeed_api_application_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Application key'),
    '#description' => t('Your Culture Feed Application key.'),
    '#default_value' => variable_get('culturefeed_api_application_key', ''),
    '#size' => 40,
    '#maxlength' => 40,
  );
  $form['settings']['culturefeed_api_shared_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Shared secret'),
    '#description' => t('Your Culture Feed Shared Secret.'),
    '#default_value' => variable_get('culturefeed_api_shared_secret', ''),
    '#size' => 40,
    '#maxlength' => 40,
  );
  $form['settings']['culturefeed_entry_api_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Entry API path'),
    '#description' => t('The path where the Culture Feed Entry API resides. End with a slash. Example: entry/test.rest.uitdatabank.be/api/v1/'),
    '#default_value' => variable_get('culturefeed_entry_api_path', CULTUREFEED_ENTRY_API_PATH),
  );

  // proxy settings
  $form['proxy'] = array(
    '#type' => 'fieldset',
    '#title' => t('Proxy server'),
    '#description' => t('If you are connecting over a proxy server, specify its settings here.'),
    '#collapsible' => TRUE,
    '#collapsed' => !variable_get('culturefeed_proxy_enabled', FALSE),
  );
  $form['proxy']['culturefeed_proxy_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Connect over a proxy server'),
    '#default_value' => variable_get('culturefeed_proxy_enabled', FALSE),
  );
  $form['proxy']['culturefeed_proxy_server'] = array(
    '#type' => 'textfield',
    '#title' => t('Web proxy server'),
    '#description' => t('The HTTP proxy to tunnel requests through.'),
    '#default_value' => variable_get('culturefeed_proxy_server', ''),
    '#states' => array(
      'invisible' => array(
        'input[name="culturefeed_proxy_enabled"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['proxy']['culturefeed_proxy_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Port'),
    '#description' => t('Specify the port number of the proxy to connect to. Leave empty for default.'),
    '#default_value' => variable_get('culturefeed_proxy_port', ''),
    '#states' => array(
      'invisible' => array(
        'input[name="culturefeed_proxy_enabled"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['proxy']['culturefeed_proxy_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#description' => t('Specify the username for the proxy to connect to. Leave empty for no username.'),
    '#default_value' => variable_get('culturefeed_proxy_username', ''),
    '#states' => array(
      'invisible' => array(
        'input[name="culturefeed_proxy_enabled"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['proxy']['culturefeed_proxy_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#description' => t('Specify the password for the proxy to connect to. Leave empty for no password.'),
    '#default_value' => variable_get('culturefeed_proxy_password', ''),
    '#states' => array(
      'invisible' => array(
        'input[name="culturefeed_proxy_enabled"]' => array('checked' => FALSE),
      ),
    ),
  );

  return system_settings_form($form);
}
