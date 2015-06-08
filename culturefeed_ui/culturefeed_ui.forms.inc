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

  $form['culturefeed_user'] = array(
    '#type' => 'value',
    '#value' => $culturefeed_user,
  );

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

  $culturefeed_user = new CultureFeed_User();
  $culturefeed_user->id = $form_state['values']['culturefeed_user']->id;
  $culturefeed_user->mbox = $form_state['values']['mbox'];

  try {
    DrupalCultureFeed::updateUser($culturefeed_user);
    drupal_set_message(t('Changes succesfully saved.'));
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    drupal_set_message(t('Error occurred'), 'error');
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

  $options = culturefeed_ui_privacy_settings_get_options($culturefeed_user);
  $default_value = culturefeed_ui_privacy_settings_current_setting($culturefeed_user);

  $form['culturefeed_user'] = array(
    '#type' => 'value',
    '#value' => $culturefeed_user,
  );

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
  $culturefeed_user = $form_state['values']['culturefeed_user'];

  $setting = $form_state['values']['setting'];

  $privacy_config = new CultureFeed_UserPrivacyConfig();

  // Always on.
  $privacy_config->givenName = CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;
  $privacy_config->familyName = CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;

  // Depending on setting.
  $privacy_config->homeAddress = CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE;
  if ($setting == 'age_location' || $setting == 'location') {
    $privacy_config->homeAddress = CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;
  }
  $privacy_config->dob = CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE;
  if ($setting == 'age_location' || $setting == 'age') {
    $privacy_config->dob = CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;
  }

  // Not used ?
  $privacy_config->gender = CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE;
  $privacy_config->bio = CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE;

  try {
    DrupalCultureFeed::updateUserPrivacy($culturefeed_user->id, $privacy_config);
    drupal_set_message(t('Your privacy settings have been saved.'));
    $form_state['redirect'] = 'user';
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

  $form['culturefeed_user'] = array(
    '#type' => 'value',
    '#value' => $culturefeed_user,
  );

  $form['view-profile'] = array(
    '#prefix' => '<div id="view-profile">',
    '#markup' => l(t('View profile'), 'user/' . culturefeed_get_uid_for_cf_uid($culturefeed_user->id, $culturefeed_user->nick)),
    '#suffix' => '</div>',
  );

  // Firstname.
  $form['givenName'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#default_value' => $culturefeed_user->givenName,
  );
  $form['givenNamePrivacy'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide \'first name\' in public profile'),
    '#default_value' => $culturefeed_user->privacyConfig->givenName == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
  );

  // Name.
  $form['familyName'] = array(
    '#type' => 'textfield',
    '#title' => t('Family name'),
    '#default_value' => $culturefeed_user->familyName,
  );
  $form['familyNamePrivacy'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide \'family name\' in public profile'),
    '#default_value' => $culturefeed_user->privacyConfig->familyName == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
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

  // Gender.
  $form['gender'] = array(
    '#type' => 'radios',
    '#title' => t('Gender'),
    '#options' => array('male' => t('Male'), 'female' => t('Female')),
    '#default_value' => $culturefeed_user->gender,
  );
  $form['genderPrivacy'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide \'gender\' in public profile'),
    '#default_value' => $culturefeed_user->privacyConfig->gender == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
  );

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
  $form['homeAddressPrivacy'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide \'address\' in public profile'),
    '#default_value' => $culturefeed_user->privacyConfig->homeAddress == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
  );

  // Date of birth.
  $form['dob'] = array(
    '#title' => t('Date of birth'),
    '#type' => 'textfield',
    '#default_value' => $culturefeed_user->dob ? date('d/m/Y', $culturefeed_user->dob) : '',
    '#description' => t('Format : dd/mm/yyyy'),
    '#size' => 10,
  );
  $form['dobPrivacy'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide \'date of birth\' in public profile'),
    '#default_value' => $culturefeed_user->privacyConfig->dob == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
  );

  // Bio
  $form['bio'] = array(
    '#type' => 'textarea',
    '#title' => t('Biography'),
    '#default_value' => $culturefeed_user->bio,
    '#description' => t('Maximum 250 characters'),
  );
  $form['bioPrivacy'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide \'biography\' in public profile'),
    '#default_value' => $culturefeed_user->privacyConfig->bio == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
  );

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

  // Custom validations first.
  if (drupal_strlen($values['bio']) > 250) {
    form_set_error('bio', t('The maximum of 250 characters is exceeded'));
    return;
  }

  // Update profile information.
  $user_update = new CultureFeed_User();

  $user_update->id = $values['culturefeed_user']->id;
  $user_update->givenName = $values['givenName'];
  $user_update->familyName = $values['familyName'];
  $user_update->gender = $values['gender'];
  $user_update->bio = $values['bio'];
  $user_update->street = $values['street'];
  $user_update->zip = $values['zip'];
  $user_update->city = $values['city'];
  $user_update->country = $values['country'];
  $user_update->preferredLanguage = $values['preferredLanguage'];

  if (empty($values['dob'])) {
    $user_update->dob = '';
  }
  else {
    $dob_parts = explode('/', $values['dob']);

    if (count($dob_parts) == 3) {
      list($day, $month, $year) = $dob_parts;

      if (is_numeric($day) && is_numeric($month) && is_numeric($year)) {
        if ($date = mktime(0, 0, 0, (int) $month, (int) $day, (int) $year)) {
          $user_update->dob = $date;
        }
      }
    }
  }

  $fields = array('id', 'givenName', 'familyName', 'gender', 'bio', 'street', 'zip', 'city', 'country', 'dob');

  try {
    DrupalCultureFeed::updateUser($user_update, $fields);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    form_set_error('submit', t('Error occurred while saving your personal data.'));
  }

  // Remove the profile picture if requested.
  if (empty($form_state['values']['picture']) && $form_state['#old_picture'] > 0) {
    try {
      DrupalCultureFeed::removeUserDepiction(DrupalCultureFeed::getLoggedInUserId());
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


        DrupalCultureFeed::uploadUserDepiction(DrupalCultureFeed::getLoggedInUserId(), $file_upload);
      }
      catch (Exception $e) {
        watchdog_exception('culturefeed_ui', $e);
        form_set_error('picture', t('Error occurred while saving your picture.'));
      }
    }
  }

  // Update field privacy status.
  $privacy_config = new CultureFeed_UserPrivacyConfig();

  $privacy_config->givenName = $values['givenNamePrivacy'] ? CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE : CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;
  $privacy_config->familyName = $values['familyNamePrivacy'] ? CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE : CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;
  $privacy_config->gender = $values['genderPrivacy'] ? CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE : CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;
  $privacy_config->homeAddress = $values['homeAddressPrivacy'] ? CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE : CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;
  $privacy_config->dob = $values['dobPrivacy'] ? CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE : CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;
  $privacy_config->bio = $values['bioPrivacy'] ? CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE : CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;

  try {
    DrupalCultureFeed::updateUserPrivacy(DrupalCultureFeed::getLoggedInUserId(), $privacy_config);
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
function culturefeed_ui_page_profile_edit_form_submit(array $form, array &$form_state) {
  drupal_set_message(t('Changes succesfully saved.'));
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