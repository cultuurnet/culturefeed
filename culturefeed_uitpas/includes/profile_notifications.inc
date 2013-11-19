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

  $card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');
  $uitpas_number = 0;
  if ($card_system_id && isset($uitpas_user->passholder->cardSystemSpecific[$card_system_id]->currentCard->uitpasNumber)) {
    $uitpas_number = $uitpas_user->passholder->cardSystemSpecific[$card_system_id]->currentCard->uitpasNumber;
  }

  $form['uitpas_user'] = array(
    '#type' => 'value',
    '#value' => $uitpas_user,
  );
  $form['cf'] = array(
    '#type' => 'value',
    '#value' => $uitpas_user->cf,
  );
  $form['intro'] = array(
    '#markup' => variable_get('culturefeed_uitpas_profile_notifications_intro'),
  );
  $form['email_notifications'] = array(
    '#type' => 'checkbox',
    '#title' => t('via e-mail'),
    '#default_value' => ($uitpas_user->passholder->emailPreference == CultureFeed_Uitpas_Passholder::EMAIL_NOTIFICATION_MAILS) ? 1 : 0,
  );
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('to e-mail address'),
    '#default_value' => ($uitpas_user->passholder->email) ? $uitpas_user->passholder->email : $uitpas_user->user->mbox,
    '#required' => TRUE,
  );
  $form['sms_notifications'] = array(
    '#type' => 'checkbox',
    '#title' => t('via sms'),
    '#default_value' => ($uitpas_user->passholder->smsPreference == CultureFeed_Uitpas_Passholder::SMS_NOTIFICATION_SMS) ? 1 : 0,
  );
  $form['mobile'] = array(
    '#type' => 'textfield',
    '#title' => t('to mobile number'),
    '#default_value' => $uitpas_user->passholder->gsm,
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

  $card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');
  $uitpas_number = 0;
  if ($card_system_id && isset($uitpas_user->passholder->cardSystemSpecific[$card_system_id]->currentCard->uitpasNumber)) {
    $uitpas_number = $uitpas_user->passholder->cardSystemSpecific[$card_system_id]->currentCard->uitpasNumber;
  }

  if ($uitpas_number) {

    $passholder = new CultureFeed_Uitpas_Passholder();

    $passholder->uitpasNumber = $uitpas_number;

    // Email.
    if ($form_state['values']['email_notifications']) {
      $email_preference = CultureFeed_Uitpas_Passholder::EMAIL_NOTIFICATION_MAILS;
    }
    else {
      $email_preference = CultureFeed_Uitpas_Passholder::EMAIL_NO_MAILS;
    }
    $passholder->emailPreference = $email_preference;
    $passholder->email = $form_state['values']['email'];

    // SMS.
    if ($form_state['values']['sms_notifications']) {
      $sms_preference = CultureFeed_Uitpas_Passholder::SMS_NOTIFICATION_SMS;
    }
    else {
      $sms_preference = CultureFeed_Uitpas_Passholder::SMS_NO_SMS;
    }
    $passholder->smsPreference = $sms_preference;
    $passholder->gsm = $form_state['values']['mobile'];

    try {

      $uitpas_user->cf->uitpas()->updatePassholder($passholder);
      drupal_set_message(t('Your changes have been saved.'));

    }
    catch (Exception $e) {

      watchdog_exception('culturefeed_uitpas_profile_notifications', $e);
      drupal_set_message(t('An error occured.'), 'error');

    }

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
