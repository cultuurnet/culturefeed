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

    $form['dob'] = array(
      '#default_value' => date('j/m/Y', $passholder->dateOfBirth),
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
        '#markup' => variable_get('culturefeed_uitpas_profile_details_email_description'),
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
 * Returns form elements for profile details settings.
 */
function culturefeed_uitpas_profile_details_settings_get(&$form) {

  $form['profile_details'] = array(
    '#type' => 'fieldset',
    '#title' => t('UiTPAS profile details settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['profile_details']['culturefeed_uitpas_profile_details_menu_advantages_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu: advantages title'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_details_menu_advantages_title', t('My advantages')),
  );
  $form['profile_details']['culturefeed_uitpas_profile_details_menu_advantages_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Menu: advantages description'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_details_menu_advantages_description', t('Discover which promotions and advantages you are entitled to.')),
  );
  $form['profile_details']['culturefeed_uitpas_profile_details_menu_actions_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu: actions title'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_details_menu_actions_title', t('Points history')),
  );
  $form['profile_details']['culturefeed_uitpas_profile_details_menu_actions_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Menu: actions description'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_details_menu_actions_description', t('An overview of all your UiTPAS actions.')),
  );
  $form['profile_details']['culturefeed_uitpas_profile_details_menu_notifications_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu: notifications title'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_details_menu_notifications_title', t('My notifications')),
  );
  $form['profile_details']['culturefeed_uitpas_profile_details_menu_notifications_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Menu: notifications description'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_details_menu_notifications_description', t('Change your preferences for receiving personal notifications via email.')),
  );
  $form['profile_details']['culturefeed_uitpas_profile_details_title'] = array(
    '#type' => 'textfield',
    '#title' => t('UiTPAS Title'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_details_title', t('My UiTPAS')),
  );
  $form['profile_details']['culturefeed_uitpas_profile_details_uitpas_number'] = array(
    '#type' => 'textfield',
    '#title' => t('UiTPAS Number (label)'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_details_uitpas_number', t('UiTPAS number(s)')),
  );
  $form['profile_details']['culturefeed_uitpas_profile_details_intro'] = array(
    '#type' => 'textarea',
    '#title' => t('Intro'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_details_intro', t('This page shows the UiTPAS card(s) you own and the personal information that has been registered when buying your card.')),
  );
  $form['profile_details']['culturefeed_uitpas_profile_details_form_title'] = array(
    '#type' => 'textarea',
    '#title' => t('Form title'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_details_form_title', t('My personal information')),
  );
  $form['profile_details']['culturefeed_uitpas_profile_details_form_intro'] = array(
    '#type' => 'textarea',
    '#title' => t('Form intro'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_details_form_intro', t('This information has been captured from your eID and cannot be changed. Updates can only be done at official UiTPAS registration counters')),
  );
  $form['profile_details']['culturefeed_uitpas_profile_details_email_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Email description'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_details_email_description', t('This email address has been connected to your <a href="@edit-profile">online profile</a>. This address will be used for all communication regarding @site_name and UiTPAS.', array('@edit-profile' => url('culturefeed/profile/edit')))),
  );
  $form['profile_details']['culturefeed_uitpas_profile_details_outro'] = array(
    '#type' => 'textarea',
    '#title' => t('Outro'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_details_outro', t('This information has been captured from your eID and cannot be changed. Updates can only be done at official UiTPAS registration counters')),
  );

}
