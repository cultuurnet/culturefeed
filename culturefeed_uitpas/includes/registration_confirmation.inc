<?php

/**
 * @file
 * Helper functions for UiTPAS registration confirmation.
 */

/**
 * Returns registration confirmation form.
 */
function culturefeed_uitpas_registration_confirmation_get() {

  $uitpas_user = culturefeed_uitpas_get_uitpas_user();

  return array(
    'intro' => array(
      '#type' => 'markup',
      '#markup' => variable_get('culturefeed_uitpas_registration_confirmation_intro', ''),
    ),
    'form' => drupal_get_form('culturefeed_uitpas_registration_confirmation_form', $uitpas_user->user, $uitpas_user->cf),
  );

}

/**
 * Form constructor for the registration confirmation form.
 */
function culturefeed_uitpas_registration_confirmation_form($form, &$form_state, $uitpas_user, $cf) {

  form_load_include($form_state, 'inc', 'culturefeed_uitpas', 'includes/registration_confirmation.inc');

  $form['uitpas_user'] = array(
    '#type' => 'value',
    '#value' => $uitpas_user,
  );
  $form['cf'] = array(
    '#type' => 'value',
    '#value' => $cf,
  );

  $form['publish_actions'] = array(
    '#type' => 'checkbox',
    '#title' => variable_get('culturefeed_uitpas_registration_confirmation_actions_label', 'Make my UiTPAS actions available for others'),
    '#default_value' => 1,
  );

  $form['actions'] = array(
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Confirm'),
    ),
  );

  $form['#suffix'] = l(t('Click here to continue to the homepage'), '<front>');

  return $form;

}

/**
 * Submit handler for culturefeed_uitpas_registration_confirmation_form().
 */
function culturefeed_uitpas_registration_confirmation_form_submit(&$form, &$form_state) {

  try {
    $uitpas_user = $form_state['values']['uitpas_user'];
    $cf = $form_state['values']['cf'];

    // Preferences.
    $preferences = new CultureFeed_Preferences();
    $private = ($form_state['values']['publish_actions']) ? FALSE : TRUE;
    $preference = new CultureFeed_ActivityPrivacyPreference(CultureFeed_Activity::TYPE_UITPAS, $private);
    $preferences->activityPrivacyPreferences[] = $preference;
    $cf->setUserPreferences($uitpas_user->id, $preferences);

    // Form.
    drupal_set_message(t('Your changes have been saved.'));
    $form_state['redirect'] = '<front>';

  }
  catch (Exception $e) {

    watchdog_exception('culturefeed_uitpas_registration_confirmation', $e);
    drupal_set_message(t('An error occured.'), 'error');

  }

}

/**
 * Returns form elements for registration confirmation settings.
 */
function culturefeed_uitpas_registration_confirmation_settings_get(&$form) {

  $form['registration_confirmation'] = array(
    '#type' => 'fieldset',
    '#title' => t('UiTPAS registration confirmation settings'),
    '#collapsible' => TRUE,
  );
  $form['registration_confirmation']['culturefeed_uitpas_registration_confirmation_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Page title'),
    '#default_value' => variable_get('culturefeed_uitpas_registration_confirmation_title', 'You are now registered on UiTPAS'),
  );
  $form['registration_confirmation']['culturefeed_uitpas_registration_confirmation_intro'] = array(
    '#type' => 'textarea',
    '#title' => t('Intro text'),
    '#default_value' => variable_get('culturefeed_uitpas_registration_confirmation_intro'),
  );
  $form['registration_confirmation']['culturefeed_uitpas_registration_confirmation_actions_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Actions label'),
    '#default_value' => variable_get('culturefeed_uitpas_registration_confirmation_actions_label', 'Make my UiTPAS actions available for others'),
  );

}

/**
 * Saves settings for recent actions.
 */
function culturefeed_uitpas_registration_confirmation_settings_save($values) {

  /* Not needed for the moment as settings are saved via the 
   * system_settings_form on admin/config/culturefeed/uitpas */;

}
