<?php

/**
 * @file
 * Plugin to provide a content type for uitpas user profile notifications.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Uitpas: user profile notifications'),
  'description' => t('Uitpas: user profile notifications.'),
  'category' => t('Uitpas'),
  'required context' => new ctools_context_required(t('Uitpas user'), 'uitpas_user'),
);

function uitpas_ui_uitpas_user_profile_ntfctns_content_type_edit_form($form, &$form_state) {
  return $form;
}

function uitpas_ui_uitpas_user_profile_ntfctns_content_type_render($subtype, $conf, $args, $context) {
  if ($context->data->passholder) {
    $uitpas_user = $context->data->user;
    $passholder = $context->data->passholder;
    $cf = $context->data->cf;
    $block = new stdClass();
    $block->title = t('My notifications');
    $form_id = 'uitpas_ui_uitpas_user_profile_ntfctns_content_type_render_form';
    $block->content = drupal_get_form($form_id, $uitpas_user, $passholder, $cf);
    return $block;
  }
}

function uitpas_ui_uitpas_user_profile_ntfctns_content_type_render_form($form, &$form_state, $uitpas_user, $passholder, $cf) {
  $form['uitpasnumber'] = array(
    '#type' => 'value',
    '#value' => $passholder->uitpasNumber,
  );
  $form['cf'] = array(
    '#type' => 'value',
    '#value' => $cf,
  );
  $form['intro'] = array(
    '#markup' => variable_format_value('uitpas_ui_user_profile_notifications_intro'),
  );
  $form['email_notifications'] = array(
    '#type' => 'checkbox',
    '#title' => t('via e-mail'),
    '#default_value' => ($passholder->emailPreference == CultureFeed_Uitpas_Passholder::EMAIL_NOTIFICATION_MAILS) ? 1 : 0,
  );
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('to e-mail address'),
    '#default_value' => ($passholder->email) ? $passholder->email : $uitpas_user->email,
    '#required' => TRUE,
  );
  $form['sms_notifications'] = array(
    '#type' => 'checkbox',
    '#title' => t('via sms'),
    '#default_value' => ($passholder->smsPreference == CultureFeed_Uitpas_Passholder::SMS_NOTIFICATION_SMS) ? 1 : 0,
  );
  $form['mobile'] = array(
    '#type' => 'textfield',
    '#title' => t('to mobile number'),
    '#default_value' => $passholder->gsm,
    '#required' => TRUE,
  );
  $form['actions'] = array(
    'save' => array(
      '#type' => 'submit',
      '#value' => t('Confirm'),
    )
  );
  return $form;
}

function uitpas_ui_uitpas_user_profile_ntfctns_content_type_render_form_validate(&$form, &$form_state) {
  if (!valid_email_address($form_state['values']['email'])) {
    form_set_error('email', t('Invalid e-mail address'));
  }
}

function uitpas_ui_uitpas_user_profile_ntfctns_content_type_render_form_submit(&$form, &$form_state) {
  $passholder = new CultureFeed_Uitpas_Passholder();
  $passholder->uitpasNumber = $form_state['values']['uitpasnumber'];
  $cf = $form_state['values']['cf'];
  if ($form_state['values']['email_notifications']) {
    $email_preference = CultureFeed_Uitpas_Passholder::EMAIL_NOTIFICATION_MAILS;
  }
  else {
    $email_preference = CultureFeed_Uitpas_Passholder::EMAIL_NO_MAILS;
  }
  $passholder->emailPreference = $email_preference;
  //$passholder->email = $form_state['values']['email'];
  if ($form_state['values']['sms_notifications']) {
    $sms_preference = CultureFeed_Uitpas_Passholder::SMS_NOTIFICATION_SMS;
  }
  else {
    $sms_preference = CultureFeed_Uitpas_Passholder::SMS_NO_SMS;
  }
  $passholder->smsPreference = $sms_preference;
  $passholder->gsm = $form_state['values']['mobile'];
  try {
    $cf->uitpas()->updatePassholder($passholder);
    drupal_set_message(t('Your changes have been saved.'));
  }
  catch (Exception $e) {
    watchdog_exception('uitpas_ui_content_type_user_profile_ntfctns', $e);
    drupal_set_message(t('An error occured.'), 'error');
  }
}