<?php

/**
 * Form constructor for the account edit form.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 * @param CultureFeed_User $culturefeed_user
 *
 * @return array
 *   The form.
 */
function culturefeed_ui_account_edit_form(array $form, array $form_state, CultureFeed_User $culturefeed_user) {

  $form['#culturefeed_user'] = $culturefeed_user;
  $form['#theme'] = 'culturefeed_ui_account_edit_form';

  $form['nick'] = array(
    '#disabled' => TRUE,
    '#title' => t('Username'),
    '#type' => 'textfield',
    '#value' => $culturefeed_user->nick,
  );

  $form['mbox'] = array(
    '#default_value' => $culturefeed_user->mbox,
    '#description' => variable_get('culturefeed_ui_account_edit_email_description', ''),
    '#required' => TRUE,
    '#title' => t('Email address'),
    '#type' => 'textfield',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $destination = url('culturefeed/account/edit', array('absolute' => TRUE));
  $url = DrupalCultureFeed::getUrlChangePassword($culturefeed_user->id, $destination);
  $options = array('attributes' => array('class' => array('culturefeedconnect')), 'query' => drupal_get_destination());

  $form['change_password'] = array(
    '#prefix' => '<div id="change-password">',
    '#markup' => l(t('Change password'), $url, $options),
    '#suffix' => '</div>',
  );

  return $form;

}

/**
 * Validation handler for culturefeed_ui_account_edit_form().
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 */
function culturefeed_ui_account_edit_form_validate($form, &$form_state) {

  if (!valid_email_address($form_state['values']['mbox'])) {
    form_set_error('mbox', t('Invalid email'));
  }

}

/**
 * Submit handler for culturefeed_ui_account_edit_form().
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 */
function culturefeed_ui_account_edit_form_submit($form, &$form_state) {

  /* @var CultureFeed_User $culturefeed_user */
  $culturefeed_user = $form['#culturefeed_user'];

  $error = FALSE;

  // User.
  $culturefeed_user_update = new CultureFeed_User();
  $culturefeed_user_update->id = $culturefeed_user->id;
  $culturefeed_user_update->mbox = $form_state['values']['mbox'];

  try {
    DrupalCultureFeed::updateUser($culturefeed_user_update);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui_account_edit_user', $e);
    $error = TRUE;
  }

  // Passholder.
  /* @var CultureFeed $cf */
  $cf = DrupalCultureFeed::getLoggedInUserInstance();
  $passholder = $cf->uitpas()->getPassholderByUser($culturefeed_user->id);
  $card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');
  if ($passholder && $card_system_id && isset($passholder->cardSystemSpecific[$card_system_id])) {

    $uitpas_number = $passholder->cardSystemSpecific[$card_system_id]->currentCard->uitpasNumber;

    $passholder_update = new CultureFeed_Uitpas_Passholder();
    $passholder_update->uitpasNumber = $uitpas_number;
    $passholder_update->email = $form_state['values']['mbox'];

    try {
      $cf->uitpas()->updatePassholder($passholder_update);
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_ui_account_edit_uitpas', $e);
      $error = TRUE;
    }

  }

  if ($error) {
    drupal_set_message(t('An error occurred.'), 'error');
  }
  else {
    drupal_set_message(t('Changes saved successfully.'));
  }

}

/**
 * Form constructor for the profile privacy form.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 * @param CultureFeed_User $culturefeed_user
 *   The culturefeed user.
 *
 * @return array
 *   The form.
 */
function culturefeed_ui_privacy_settings_form(array $form, array &$form_state, CultureFeed_User $culturefeed_user) {

  form_load_include($form_state, 'inc', 'culturefeed_ui', 'includes/privacy_settings');

  $options = culturefeed_ui_privacy_settings_get_options($culturefeed_user);
  $default_value = culturefeed_ui_privacy_settings_get_current_setting($culturefeed_user);
  if (!in_array($default_value, array_keys($options))) {
    $default_value = 'nick';
  }

  $form['#culturefeed_user'] = $culturefeed_user;

  $form['setting'] = array(
    '#default_value' => $default_value,
    '#options' => $options,
    '#title' => t('Select they way your user name is displayed'),
    '#type' => 'radios',
  );

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    ),
  );

  return $form;

}

/**
 * Submit handler for culturefeed_ui_privacy_settings_form().
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 */
function culturefeed_ui_privacy_settings_form_submit(array $form, array &$form_state) {

  /* @var CultureFeed_User $culturefeed_user */
  $culturefeed_user = $form['#culturefeed_user'];

  $setting = $form_state['values']['setting'];
  $privacy_config = new CultureFeed_UserPrivacyConfig();
  culturefeed_ui_privacy_settings_set_options($privacy_config, $setting);


  try {
    DrupalCultureFeed::updateUserPrivacy($culturefeed_user->id, $privacy_config);
    drupal_set_message(t('Your privacy settings have been saved.'));
  }
  catch (Exception $e) {
    form_set_error('submit', t('Error occurred while saving your privacy settings.'));
  }

}

/**
 * Form constructor for the profile edit form.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 * @param CultureFeed_User $culturefeed_user
 *   The culturefeed user.
 *
 * @return array
 *   The form.
 */
function culturefeed_ui_profile_edit_form($form, &$form_state, CultureFeed_User $culturefeed_user) {

  $form['#culturefeed_user'] = $culturefeed_user;
  $form['#theme'] = 'culturefeed_ui_profile_edit_form';

  // First name.
  $form['givenName'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#default_value' => $culturefeed_user->givenName,
  );

  // Name.
  $form['familyName'] = array(
    '#type' => 'textfield',
    '#title' => t('Family name'),
    '#default_value' => $culturefeed_user->familyName,
  );

  // Date of birth.
  $form['dob'] = array(
    '#title' => t('Date of birth'),
    '#type' => 'textfield',
    '#default_value' => $culturefeed_user->dob ? date('d/m/Y', $culturefeed_user->dob) : '',
    '#description' => t('Format : dd/mm/yyyy'),
    '#size' => 10,
  );

  // Gender.
  $form['gender'] = array(
    '#type' => 'radios',
    '#title' => t('Gender'),
    '#options' => array('male' => t('Male'), 'female' => t('Female')),
    '#default_value' => $culturefeed_user->gender,
  );

  // Picture.
  $form_state['#old_picture'] = 0;
  $form['picture'] = array(
    '#type' => 'managed_file',
    '#title' => t('Choose picture'),
    '#description' => t('Allowed extensions: jpg, jpeg, gif or png'),
    '#process' => array('file_managed_file_process', 'culturefeed_image_file_process'),
    '#upload_validators' => array(
      'file_validate_extensions' => array('jpg jpeg png gif'),
    ),
    '#upload_location' => 'public://culturefeed',
  );

  // Check if the depiction is not the default one.
  if (!empty($culturefeed_user->depiction) && !strstr($culturefeed_user->depiction, '/' . CULTUREFEED_UI_DEFAULT_DEPICTION)) {
    $file = culturefeed_create_temporary_image($culturefeed_user->depiction, file_default_scheme() . '://culturefeed');
    if ($file) {
      $form_state['#old_picture'] = $file->fid;
      $form['picture']['#default_value'] = $file->fid;
    }
  }

  // Address
  $form['street'] = array(
    '#type' => 'textfield',
    '#title' => t('Street and number'),
    '#default_value' => $culturefeed_user->street,
  );
  $form['zip'] = array(
    '#type' => 'textfield',
    '#title' => t('Zipcode'),
    '#default_value' => $culturefeed_user->zip,
  );
  $form['city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#default_value' => $culturefeed_user->city,
  );
  $form['country'] = array(
    '#type' => 'select',
    '#options' => country_get_list(),
    '#title' => t('Country'),
    '#default_value' => !empty($culturefeed_user->country) ? $culturefeed_user->country : 'BE',
  );

  // Bio
  $form['bio'] = array(
    '#type' => 'textarea',
    '#title' => t('Biography'),
    '#default_value' => $culturefeed_user->bio,
    '#description' => t('Maximum 250 characters'),
  );

  if (variable_get('culturefeed_ui_profile_show_language_settings', FALSE)) {

    // Default language.
    $form['preferredLanguage'] = array(
      '#type' => 'select',
      '#title' => t('Preferred language'),
      '#default_value' => !empty($culturefeed_user->preferredLanguage) ? $culturefeed_user->preferredLanguage : '',
      '#options' => array(
        'nl' => t('Dutch'),
        'fr' => t('French'),
        'en' => t('English'),
        'de' => t('German'),
      ),
    );

  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Validation handler for culturefeed_ui_profile_edit_form().
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 */
function culturefeed_ui_profile_edit_form_validate(array $form, array &$form_state) {

  $values = $form_state['values'];
  $culturefeed_user = $form['#culturefeed_user'];

  // Custom validations first.
  if (drupal_strlen($values['bio']) > 250) {
    form_set_error('bio', t('The maximum of 250 characters is exceeded'));
    return;
  }

  // Update profile information.
  $culturefeed_user_update = new CultureFeed_User();

  $culturefeed_user_update->id = $culturefeed_user->id;
  $culturefeed_user_update->givenName = $values['givenName'];
  $culturefeed_user_update->familyName = $values['familyName'];
  $culturefeed_user_update->gender = $values['gender'];
  $culturefeed_user_update->bio = $values['bio'];
  $culturefeed_user_update->street = $values['street'];
  $culturefeed_user_update->zip = $values['zip'];
  $culturefeed_user_update->city = $values['city'];
  $culturefeed_user_update->country = $values['country'];
  // For the moment this field is removed, so we default to Dutch.
  $culturefeed_user_update->preferredLanguage = (isset($values['preferredLanguage'])) ? $values['preferredLanguage'] : 'nl';

  if (empty($values['dob'])) {
    $culturefeed_user_update->dob = '';
  }
  else {
    $dob_parts = explode('/', $values['dob']);

    if (count($dob_parts) == 3) {
      list($day, $month, $year) = $dob_parts;

      if (is_numeric($day) && is_numeric($month) && is_numeric($year)) {
        if ($date = mktime(0, 0, 0, (int) $month, (int) $day, (int) $year)) {
          $culturefeed_user_update->dob = $date;
        }
      }
    }
  }

  $fields = array('id', 'givenName', 'familyName', 'gender', 'bio', 'street', 'zip', 'city', 'country', 'dob');

  try {
    DrupalCultureFeed::updateUser($culturefeed_user_update, $fields);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    form_set_error('submit', t('Error occurred while saving your personal data.'));
  }

  // Remove the profile picture if requested.
  if (empty($form_state['values']['picture']) && $form_state['#old_picture'] > 0) {
    try {
      DrupalCultureFeed::removeUserDepiction($culturefeed_user->id);
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_ui', $e);
    }
  }

  // Upload profile picture.
  if ($form_state['values']['picture'] && $form_state['#old_picture'] != $form_state['values']['picture']) {

    $file = file_load($form_state['values']['picture']);
    if ($file) {
      try {
        $file_upload = culturefeed_prepare_curl_upload_from_file($file);
        DrupalCultureFeed::uploadUserDepiction($culturefeed_user->id, $file_upload);
      }
      catch (Exception $e) {
        watchdog_exception('culturefeed_ui', $e);
        form_set_error('picture', t('Error occurred while saving your picture.'));
      }
    }
  }

  // Save the default privacy settings.  Adjustable privacy settings are moved
  // to privacy settings tab.
  $privacy_config = new CultureFeed_UserPrivacyConfig();
  $privacy_config->gender = CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE;
  $privacy_config->bio = CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;

  try {
    DrupalCultureFeed::updateUserPrivacy($culturefeed_user->id, $privacy_config);
  }
  catch (Exception $e) {
    form_set_error('submit', t('Error occurred while saving your privacy settings.'));
  }

}

/**
 * Submit handler for culturefeed_ui_profile_edit_form().
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 */
function culturefeed_ui_profile_edit_form_submit(array $form, array &$form_state) {

  /* @var CultureFeed_User $culturefeed_user */
  $culturefeed_user = $form['#culturefeed_user'];
  $message = culturefeed_ui_profile_edit_message($culturefeed_user);
  drupal_set_message($message);

}

function culturefeed_ui_users_search_form() {
  $form = array();

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#size' => 20,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  return $form;
}

function culturefeed_ui_users_search_form_submit($form, &$form_state) {
  $name = trim($form_state['values']['name']);

  if (!empty($name)) {
    $form_state['redirect'] = 'culturefeed/users/search/' . $name;
  }
  else {
    $form_state['redirect'] = 'culturefeed/users/search';
  }
}