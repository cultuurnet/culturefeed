<?php

/**
 * @file
 * Helper functions for UiTPAS profile details.
 */

/**
 * Form constructor for profile details.
 */
function culturefeed_uitpas_profile_details_form($form, &$form_state) {

  $uitpas_user = culturefeed_uitpas_get_uitpas_user();

  if ($uitpas_user->user && $uitpas_user->passholder && $uitpas_user->card_system) {

    /* @var CultureFeed_Uitpas_Passholder $passholder */
    $passholder = $uitpas_user->passholder;

    $form['#theme'] = 'culturefeed_uitpas_profile_details_form';

    $form['uitpas_user'] = array(
      '#type' => 'value',
      '#value' => $uitpas_user,
    );

    $form['first_name'] = array(
      '#default_value' => $passholder->firstName,
      '#disabled' => TRUE,
      '#title' =>  t('First name'),
      '#type' => 'textfield',
    );

    $form['last_name'] = array(
      '#default_value' => $passholder->name,
      '#disabled' => TRUE,
      '#title' =>  t('Name'),
      '#type' => 'textfield',
    );

    // Set date of birth in right timezone
    $default_value = '';
    if (!empty($passholder->dateOfBirth)) {
      $dob = new DateTime();
      $time_zone = new DateTimeZone('Europe/Brussels');
      $dob->setTimestamp($passholder->dateOfBirth);
      $dob->setTimezone($time_zone);
      $default_value = $dob->format('d/m/Y');
    }
    $form['dob'] = array(
      '#default_value' => $default_value,
      '#disabled' => TRUE,
      '#title' =>  t('Date of birth'),
      '#type' => 'textfield',
    );

    $form['pob'] = array(
      '#default_value' => $passholder->placeOfBirth,
      '#disabled' => TRUE,
      '#title' =>  t('Place of birth'),
      '#type' => 'textfield',
    );

    $form['gender'] = array(
      '#default_value' => ($passholder->gender == 'MALE') ? t('Male') : t('Female'),
      '#disabled' => TRUE,
      '#title' =>  t('Gender'),
      '#type' => 'textfield',
    );

    $form['nationality'] = array(
      '#default_value' => $passholder->nationality,
      '#disabled' => TRUE,
      '#title' =>  t('Nationality'),
      '#type' => 'textfield',
    );

    $form['street'] = array(
      '#default_value' => $passholder->street,
      '#disabled' => TRUE,
      '#title' =>  t('Street'),
      '#type' => 'textfield',
    );

    $form['nr'] = array(
      '#default_value' => $passholder->number,
      '#disabled' => TRUE,
      '#title' =>  t('Nr'),
      '#type' => 'textfield',
    );

    $form['zip'] = array(
      '#default_value' => $passholder->postalCode,
      '#disabled' => TRUE,
      '#title' =>  t('Zip'),
      '#type' => 'textfield',
    );

    $form['city'] = array(
      '#default_value' => $passholder->city,
      '#disabled' => TRUE,
      '#title' =>  t('City'),
      '#type' => 'textfield',
    );

    $form['tel'] = array(
      '#default_value' => $passholder->telephone,
      '#title' =>  t('Telephone'),
      '#type' => 'textfield',
    );

    $form['mobile'] = array(
      '#default_value' => $passholder->gsm,
      '#title' =>  t('Mobile'),
      '#type' => 'textfield',
    );

    $form['email'] = array(
      '#default_value' => $passholder->email,
      '#required' => TRUE,
      '#title' =>  t('Email'),
      '#type' => 'textfield',
    );

    $form['email_description'] = array(
      '#attributes' => array('class' => array('form-item', 'form-item-email-description')),
      '#type' => 'container',
      '#title' => ' ',
      'markup' => array(
        '#markup' => t('This email address is synchronized with your <a href="@edit-account">online account</a> and  will be used for all communication regarding @site-name and @card-name.', array('@edit-account' => url('culturefeed/account/edit'), '@site-name' => variable_get('site_name', 'this website'), '@card-name' => variable_get('culturefeed_uitpas_card_name', 'UiTPAS'))),
      ),
    );

    $form['actions'] = array(
      '#type' => 'actions',
      'submit' => array(
        '#value' =>  t('Save'),
        '#type' => 'submit',
      ),
    );

  }

  return $form;

}

/**
 * Validation handler for culturefeed_uitpas_profile_details_form().
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 */
function culturefeed_uitpas_profile_details_form_validate(array $form, array &$form_state) {

  if (!empty($form_state['values']['email']) && !valid_email_address($form_state['values']['email'])) {
    form_set_error('email', t('Invalid e-mail address'));
  }

}

/**
 * Submit handler for culturefeed_uitpas_profile_details_form().
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 */
function culturefeed_uitpas_profile_details_form_submit(array $form, array &$form_state) {

  $uitpas_user = $form_state['values']['uitpas_user'];
  /* @var CultureFeed_User $culturefeed_user */
  $culturefeed_user = $uitpas_user->user;
  /* @var CultureFeed_Uitpas_Passholder $passholder */
  $passholder = $uitpas_user->passholder;
  /* @var CultureFeed_Uitpas_Passholder_CardSystemSpecific $card_system */
  $card_system = $uitpas_user->card_system;

  $error = FALSE;

  if ($uitpas_user->user && $uitpas_user->passholder && $uitpas_user->card_system) {

    // Passholder.
    $passholder_update = new CultureFeed_Uitpas_Passholder();
    $passholder_update->uitpasNumber = $card_system->currentCard->uitpasNumber;
    $passholder_update->email = $form_state['values']['email'];
    if ($form_state['values']['tel']) {
      $passholder_update->telephone = $form_state['values']['tel'];
    }
    if ($form_state['values']['mobile']) {
      $passholder_update->gsm = $form_state['values']['mobile'];
    }

    try {
      $uitpas_user->cf->uitpas()->updatePassholder($passholder_update);
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_uitpas_profile_details_uitpas', $e);
      $error = TRUE;
    }

    // CultureFeed user.
    $culturefeed_user_update = new CultureFeed_User();
    $culturefeed_user_update->id = $culturefeed_user->id;
    $culturefeed_user_update->mbox = $form_state['values']['email'];

    try {
      DrupalCultureFeed::updateUser($culturefeed_user_update);
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_uitpas_profile_details_account', $e);
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
    drupal_set_message(t('Your changes have been saved.'));
  }

}

/**
 * Returns profile details.
 */
function culturefeed_uitpas_profile_details_get() {

  $uitpas_user = culturefeed_uitpas_get_uitpas_user();

  if ($uitpas_user->user && $uitpas_user->passholder) {

    return array(
      '#theme' => 'culturefeed_uitpas_profile_details',
      '#uitpas_user' => $uitpas_user,
    );

  }

}

/**
 * Title callback for the uitpas profile page.
 */
function culturefeed_uitpas_profile_title() {

  return t('My UiTPAS');

}
