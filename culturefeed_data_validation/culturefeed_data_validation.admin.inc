<?php

/**
 * @file
 * Callback and handler implementations for administration pages.
 */

/**
 * Configuration form for general settings.
 *
 * @ingroup forms
 *
 * @see system_settings_form()
 */
function culturefeed_data_validation_admin_settings() {
  $form = array();

  $form['culturefeed_data_validation_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#description' => t('Your DataValidation service API key'),
    '#default_value' => variable_get('culturefeed_data_validation_api_key'),
  );

  return system_settings_form($form);
}
