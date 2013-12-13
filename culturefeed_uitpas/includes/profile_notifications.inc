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

  /* @var CultureFeed_Uitpas_Passholder $passholder */
  $passholder = $uitpas_user->passholder;

  $form['uitpas_user'] = array(
    '#type' => 'value',
    '#value' => $uitpas_user,
  );
  $form['intro'] = array(
    '#markup' => variable_get('culturefeed_uitpas_profile_notifications_intro'),
  );

  $form['notifications'] = array(
    '#tree' => TRUE,
  );

  $drupal_card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');
  $card_system_specific_list = $passholder->cardSystemSpecific;

  $sort = function(CultureFeed_Uitpas_Passholder_CardSystemSpecific $a, CultureFeed_Uitpas_Passholder_CardSystemSpecific $b) use ($drupal_card_system_id) {
    // Preferences for the card system Drupal is configured with, should always
    // be at the top.
    if ($a->cardSystem->id == $drupal_card_system_id) {
      return 1;
    }

    if ($b->cardSystem->id == $drupal_card_system_id) {
      return -1;
    }

    // Otherwise, sort alphabetically.
    return strcasecmp(culturefeed_uitpas_cardsystem_name($a->cardSystem), culturefeed_uitpas_cardsystem_name($b->cardSystem));
  };
  uasort($card_system_specific_list, $sort);

  foreach ($card_system_specific_list as $card_system_id => $card_system_specific) {
    $form['notifications'][$card_system_id] = array(
      '#type' => 'fieldset',
      '#title' => check_plain(culturefeed_uitpas_cardsystem_name($card_system_specific->cardSystem)),
    );

    $form['notifications'][$card_system_id]['email'] = array(
      '#type' => 'checkbox',
      '#title' => t('via e-mail'),
      '#default_value' => ($card_system_specific->emailPreference == CultureFeed_Uitpas_Passholder_CardSystemPreferences::EMAIL_ALL_MAILS) ? 1 : 0,
    );

    $form['notifications'][$card_system_id]['sms'] = array(
      '#type' => 'checkbox',
      '#title' => t('via sms'),
      '#default_value' => ($card_system_specific->smsPreference == CultureFeed_Uitpas_Passholder_CardSystemPreferences::SMS_ALL_SMS) ? 1 : 0,
    );
  }

  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('to e-mail address'),
    '#default_value' => ($passholder->email) ? $passholder->email : $uitpas_user->user->mbox,
    '#required' => TRUE,
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

  if (!empty($form_state['values']['email']) && !valid_email_address($form_state['values']['email'])) {
    form_set_error('email', t('Invalid e-mail address'));
  }

  $use_sms = FALSE;
  foreach ($form_state['values']['notifications'] as $notification_preferences) {
    if ($notification_preferences['sms']) {
      $use_sms = TRUE;
      break;
    }
  }

  if ($use_sms && !$form_state['values']['mobile']) {
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
  }

  // Preferences.
  foreach ($form_state['values']['notifications'] as $card_system_id => $notification_preferences) {

    $preferences = new CultureFeed_Uitpas_Passholder_CardSystemPreferences();
    $preferences->id = $uitpas_user->user->id;
    $preferences->cardSystemId = $card_system_id;

    // Email preference.
    if ($notification_preferences['email']) {
      $email_preference = CultureFeed_Uitpas_Passholder_CardSystemPreferences::EMAIL_ALL_MAILS;
    }
    else {
      $email_preference = CultureFeed_Uitpas_Passholder_CardSystemPreferences::EMAIL_NOTIFICATION_MAILS;
    }

    $preferences->emailPreference = $email_preference;

    // SMS preference.
    if ($notification_preferences['sms']) {
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
    '#collapsed' => TRUE,
  );
  $form['profile_notifications']['culturefeed_uitpas_profile_notifications_intro'] = array(
    '#type' => 'textarea',
    '#title' => t('Intro'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_notifications_intro'),
  );

}

/**
 * Title callback for the profile notifications page.
 */
function culturefeed_uitpas_profile_notifications_title() {

  return variable_get('culturefeed_uitpas_profile_details_menu_notifications_title', t('My notifications'));

}
