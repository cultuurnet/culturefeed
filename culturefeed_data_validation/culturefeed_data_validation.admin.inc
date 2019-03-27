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

  $form['culturefeed_data_validation_api_allowed_email_verification_grades'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Allowed email verification grades'),
    '#options' => array(
      'A+' => t('A+ (Deliverable + Engagement Activity)'),
      'A' => t('A (Deliverable)'),
      'B' => t('B (Accepts-all / Catch-all)'),
      'D' => t('D (Indeterminate)'),
      'F' => t('F (Undeliverable)'),
    ),
    '#default_value' => variable_get('culturefeed_data_validation_api_allowed_email_verification_grades', array('A+', 'A', 'B', 'D')),
  );

  return system_settings_form($form);
}
