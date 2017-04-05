<?php

/**
 * @file
 * Callback and handler implementations for administration pages.
 */

/**
 * Configuration form for general settings.
 */
function culturefeed_calendar_admin_settings() {

  $form['culturefeed_calendar_beta'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable calendar only for beta users'),
    '#description' => t('If enabled, the calendar will only be visible for beta users based on cookie'),
    '#default_value' => variable_get('culturefeed_calendar_beta', FALSE),
  );

  return system_settings_form($form);

}
