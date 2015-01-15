<?php

/**
 * @file
 * Admin settings for culturefeed_agenda.
 */

/**
 * General settings for culturefeed_agenda.
 */
function culturefeed_calendar_admin_settings_form() {

  $form = array();

  $form['culturefeed_calendar_fbAppId'] = array(
    '#title' => t('facebook App Id'),
    '#type' => 'textfield',
    '#default_value' => variable_get('culturefeed_calendar_fbAppId'),
    '#description' => t('Enter the facebook app id.'),
  );

  return system_settings_form($form);;

}


