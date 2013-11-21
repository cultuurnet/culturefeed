<?php

/**
 * @file
 * Helper functions for UiTPAS profile notifications.
 */

/**
 * Returns profile notifications.
 */
function culturefeed_uitpas_profile_notifications_get() {

  $uitpas_user = culturefeed_uitpas_get_uitpas_user();

  return array(
    'form' => drupal_get_form('culturefeed_uitpas_profile_notifications_form', $uitpas_user),
  );

}

/**
 * Returns profile notifications form.
 */
function culturefeed_uitpas_profile_notifications_form($form, &$form_state, $uitpas_user) {

  form_load_include($form_state, 'inc', 'culturefeed_uitpas', 'includes/profile_notifications');

  $current_card = NULL;
  $card_system = $uitpas_user->card_system;

  /* @var CultureFeed_Uitpas_Passholder $passholder */
  $passholder = $uitpas_user->passholder;

  if (!$card_system) {
    // No known card system specific data. Initialize one with defaults.
    $card_system = new CultureFeed_Uitpas_Passholder_CardSystemSpecific();
    $card_system->emailPreference = CultureFeed_Uitpas_Passholder::EMAIL_ALL_MAILS;
    $card_system->smsPreference = CultureFeed_Uitpas_Passholder::SMS_ALL_SMS;
  }

  $form['uitpas_user'] = array(
    '#type' => 'value',
    '#value' => $uitpas_user,
  );
  $form['intro'] = array(
    '#markup' => variable_get('culturefeed_uitpas_profile_notifications_intro'),
  );
  $form['email_notifications'] = array(
    '#type' => 'checkbox',
    '#title' => t('via e-mail'),
    '#default_value' => ($card_system->emailPreference == CultureFeed_Uitpas_Passholder_CardSystemPreferences::EMAIL_ALL_MAILS) ? 1 : 0,
  );
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('to e-mail address'),
    '#default_value' => ($passholder->email) ? $passholder->email : $uitpas_user->user->mbox,
    '#required' => TRUE,
  );
  $form['sms_notifications'] = array(
    '#type' => 'checkbox',
    '#title' => t('via sms'),
    '#default_value' => ($card_system->smsPreference == CultureFeed_Uitpas_Passholder_CardSystemPreferences::SMS_ALL_SMS) ? 1 : 0,
  );
  $form['mobile'] = array(
    '#type' => 'textfield',
    '#title' => t('to mobile number'),
    '#default_value' => $passholder->gsm,
    '#states' => array(
      'required' => array(
        ':input[name="sms_notifications"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['actions'] = array(
    'save' => array(
      '#type' => 'submit',
      '#value' => t('Confirm'),
    ),
  );
  return $form;
}

/**
 * Validation handler for culturefeed_uitpas_profile_notifications_form().
 */
function culturefeed_uitpas_profile_notifications_form_validate(&$form, &$form_state) {

  if (!valid_email_address($form_state['values']['email'])) {
    form_set_error('email', t('Invalid e-mail address'));
  }
  if ($form_state['values']['sms_notifications'] && !$form_state['values']['mobile']) {
    form_set_error('mobile', t('to mobile number field is required.'));
  }

}

/**
 * Submit handler for culturefeed_uitpas_profile_notifications_form().
 */
function culturefeed_uitpas_profile_notifications_form_submit(&$form, &$form_state) {

  $uitpas_user = $form_state['values']['uitpas_user'];
  $card_system = $uitpas_user->card_system;

  $uitpas_number = 0;
  if ($card_system && isset($card_system->currentCard->uitpasNumber)) {
    $uitpas_number = $card_system->currentCard->uitpasNumber;
  }

  $error = FALSE;

  if ($uitpas_number) {

    // Passholder.
    if ($form_state['values']['email'] || $form_state['values']['mobile']) {

      $passholder = new CultureFeed_Uitpas_Passholder();
      $passholder->uitpasNumber = $uitpas_number;

      // Passholder email.
      if ($form_state['values']['email']) {
        $passholder->email = $form_state['values']['email'];
      }

      // Passholder mobile.
      if ($form_state['values']['email']) {
        $passholder->gsm = $form_state['values']['mobile'];
      }

      try {
        $uitpas_user->cf->uitpas()->updatePassholder($passholder);
      }
      catch (Exception $e) {

        $error = TRUE;
        watchdog_exception('culturefeed_uitpas_profile_notifications_passholder', $e);

      }

    }

    // Preferences.
    $preferences = new CultureFeed_Uitpas_Passholder_CardSystemPreferences();
    $preferences->id = $uitpas_user->user->id;
    $preferences->cardSystemId = variable_get('culturefeed_uitpas_cardsystem_id');

    // Email preference.
    if ($form_state['values']['email_notifications']) {
      $email_preference = CultureFeed_Uitpas_Passholder_CardSystemPreferences::EMAIL_ALL_MAILS;
    }
    else {
      $email_preference = CultureFeed_Uitpas_Passholder_CardSystemPreferences::EMAIL_NOTIFICATION_MAILS_MAILS;
    }

    $preferences->emailPreference = $email_preference;

    // SMS preference.
    if ($form_state['values']['sms_notifications']) {
      $sms_preference = CultureFeed_Uitpas_Passholder_CardSystemPreferences::SMS_ALL_SMS;
    }
    else {
      $sms_preference = CultureFeed_Uitpas_Passholder_CardSystemPreferences::SMS_NOTIFICATION_SMS;
    }
    $preferences->smsPreference = $sms_preference;

    try {
      $uitpas_user->cf->uitpas()->updatePassholderCardSystemPreferences($preferences);
    }
    catch (Exception $e) {

      $error = TRUE;
      watchdog_exception('culturefeed_uitpas_profile_notifications_preferences', $e);

    }

  }

  if ($error) {
    drupal_set_message(t('An error occurred.'), 'error');
  }
  else {
    drupal_set_message(t('Your changes have been saved.'));
  }

}

/**
 * Returns form elements for profile notifications settings.
 */
function culturefeed_uitpas_profile_notifications_settings_get(&$form) {

  $form['profile_notifications'] = array(
    '#type' => 'fieldset',
    '#title' => t('UiTPAS profile notifications settings'),
    '#collapsible' => TRUE,
  );
  $form['profile_notifications']['culturefeed_uitpas_profile_notifications_intro'] = array(
    '#type' => 'textarea',
    '#title' => t('Intro'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_notifications_intro'),
  );

}
