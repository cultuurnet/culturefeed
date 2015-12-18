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
  
  // User is registered so we clear cache
  cache_clear_all('culturefeed:uitpas:passholder:' . $uitpas_user->user->id, 'cache_culturefeed');
  cache_clear_all('culturefeed:uitpas:passholders:' . $uitpas_user->user->mbox, 'cache_culturefeed');

  return array(
    'intro' => array(
      '#type' => 'markup',
      '#markup' => t('You can now manage your points balance and personal promotions'),
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
    '#title' => t('Make my UiTPAS actions available for others'),
    '#default_value' => 1,
  );

  $form['actions'] = array(
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Confirm'),
    ),
  );

  $form['#suffix'] = l(t('Complete your profile'), 'culturefeed/profile/edit');

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
    $form_state['redirect'] = 'culturefeed/profile/edit';

  }
  catch (Exception $e) {

    watchdog_exception('culturefeed_uitpas_registration_confirmation', $e);
    drupal_set_message(t('An error occured.'), 'error');

  }

}

/**
 * Title callback for the registration confirmation page.
 */
function culturefeed_uitpas_registration_confirmation_title() {
  return t('Your UiTPAS is registered');
}
