<?php
/**
 * @file
 * Contains synchronize.inc
 */

/**
 * Page callback: synchronization of uitpas data to uitid.
 */
function culturefeed_uitpas_synchronization() {

  $form = drupal_get_form('culturefeed_uitpas_synchronization_form');
  $output = drupal_render($form);
  drupal_json_output($output);

}

/**
 * Form constructor to confirm synchronizing UiTPAS data.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 *
 * @return array
 *   The form.
 */
function culturefeed_uitpas_synchronization_form($form, &$form_state) {

  $form['intro'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => 'uitpas-synchronize-confirmation'
    ),
    'text' => array(
      '#markup' => t('Can we use your UiTPAS data to complete your profile?'),
    ),
  );

  $form['actions'] = array(
    '#type' => 'actions',
    'yes' => array(
      '#name' => 'yes',
      '#type' => 'submit',
      '#value' => t('Yes'),
    ),
    'no' => array(
      '#name' => 'no',
      '#type' => 'submit',
      '#value' => t('No'),
    ),

  );

  return $form;

}

/**
 * Submit handler for culturefeed_uitpas_synchronize_form().
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 */
function culturefeed_uitpas_synchronization_form_submit($form, &$form_state) {

  $trigger = '';
  if (isset($form_state['triggering_element']['#parents'][0])) {
    $trigger = $form_state['triggering_element']['#parents'][0];
  }

  if ($trigger == 'yes') {
    culturefeed_uitpas_synchronization_synchronize();
  }

  $form_state['redirect'] = 'culturefeed/profile/edit';

}

/**
 * Synchronizes the UiTPAS data to the UiTid user.
 */
function culturefeed_uitpas_synchronization_synchronize() {

  $uitpas_user = culturefeed_uitpas_get_uitpas_user();
  /* @var CultureFeed_User $culturefeed_user */
  $culturefeed_user = $uitpas_user->user;
  /* @var CultureFeed_Uitpas_Passholder $passholder */
  $passholder = $uitpas_user->passholder;
  $error = FALSE;

  if ($uitpas_user->user && $uitpas_user->passholder) {

    $culturefeed_user_update = new CultureFeed_User();
    $culturefeed_user_update->id = $culturefeed_user->id;
    $culturefeed_user_update->familyName = $passholder->name;
    $culturefeed_user_update->givenName = $passholder->firstName;
    $culturefeed_user_update->street = $passholder->street;
    $culturefeed_user_update->zip = $passholder->postalCode;
    $culturefeed_user_update->city = $passholder->city;
    $belgian_nationalities = array(
      'belg', 'belgisch', 'belgische',
    );
    if (in_array(strtolower($passholder->nationality), $belgian_nationalities)) {
      $culturefeed_user_update->country = 'BE';
    }
    $culturefeed_user_update->dob = $passholder->dateOfBirth;
    $culturefeed_user_update->gender = strtolower($passholder->gender);

    try {
      DrupalCultureFeed::updateUser($culturefeed_user_update);
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_uitpas_synchronization', $e);
      $error = TRUE;
    }

  }

  else {
    $error = TRUE;
  }

  if ($error) {
    drupal_set_message(t('An error occurred.'), 'error');
  }
  else {

    if (module_exists('culturefeed_ui')) {

      /* @var CultureFeed_User $culturefeed_user */
      $message = culturefeed_ui_profile_edit_message($culturefeed_user);
      drupal_set_message($message);

    }
    else {
      drupal_set_message(t('Your changes have been saved.'));
    }

  }

}

/**
 * Title callback for the synchronization page.
 */
function culturefeed_uitpas_synchronization_title() {

  return t('Update personal data');

}
