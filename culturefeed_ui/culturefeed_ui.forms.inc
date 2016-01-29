<?php

/**
 * @file
 * Contains culturefeed_ui.forms.inc.
 */

/**
 * Form constructor for the account edit form.
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
    '#required' => TRUE,
    '#title' => t('Email address'),
    '#type' => 'textfield',
  );

  // Tooltip.
  $info = t('This email address has been connected to your UiTPAS card. Any communication regarding your UiTPAS will be sent to this email address as well.');
  /* @var CultureFeed $cf */
  $cf = DrupalCultureFeed::getLoggedInUserInstance();
  try {
    $passholder = $cf->uitpas()->getPassholderByUser($culturefeed_user->id);
  }
  catch (Exception $e) {
    $passholder = NULL;
  }

  if ($passholder && $info) {

    $form['#attached'] = array(
      'js' => array(
        drupal_get_path('module', 'culturefeed_ui') . '/js/account_edit_tooltip.js',
        array(
          'type' => 'setting',
          'data' => array('culturefeed_ui_account_edit_email_description_tooltip' => $info),
        ),
      ),
      'library' => array(
        array('system', 'ui.tooltip'),
      ),
    );

  }

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
function culturefeed_ui_account_edit_form_validate(array $form, array &$form_state) {

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
function culturefeed_ui_account_edit_form_submit(array $form, array &$form_state) {

  /* @var CultureFeed_User $culturefeed_user */
  $culturefeed_user = $form['#culturefeed_user'];

  $error = FALSE;
  $duplicate = FALSE;

  // User.
  $culturefeed_user_update = new CultureFeed_User();
  $culturefeed_user_update->id = $culturefeed_user->id;
  $culturefeed_user_update->mbox = $form_state['values']['mbox'];

  try {
    DrupalCultureFeed::updateUser($culturefeed_user_update);
  }
  catch (Exception $e) {
    if ($e->error_code == CultureFeed::CODE_DUPLICATE_MBOX) {
      $duplicate = TRUE;
    }
    else {
      $error = TRUE;
      watchdog_exception('culturefeed_ui_account_edit_user', $e);
    }
  }

  // Passholder.
  /* @var CultureFeed $cf */
  $cf = DrupalCultureFeed::getLoggedInUserInstance();
  
  try {
    $passholder = $cf->uitpas()->getPassholderByUser($culturefeed_user->id);
  }
  catch (Exception $e) {
    $passholder = NULL;
  }
  
  if ($passholder) {

    $card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');
    // Save for first card, or do we have to save for all cards?
    $first_card = key($passholder->cardSystemSpecific);

    if ($card_system_id && isset($passholder->cardSystemSpecific[$card_system_id]->currentCard->uitpasNumber)) {
      $uitpas_number = $passholder->cardSystemSpecific[$card_system_id]->currentCard->uitpasNumber;
    }
    elseif (isset($passholder->cardSystemSpecific[$first_card]->currentCard->uitpasNumber)) {
      $uitpas_number = $passholder->cardSystemSpecific[$first_card]->currentCard->uitpasNumber;
    }

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
  elseif ($duplicate) {
    drupal_set_message(t('The email address you entered (@email) is already in use by another user. </br> Please provide a another email address.', array('@email' => $culturefeed_user_update->mbox)), 'error');
  }
  else {
    drupal_set_message(t('Changes saved successfully.'));
  }

}

/**
 * Form constructor for the online account publish activities form.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 * @param string $online_account_type
 *   The online account type.
 * @param CultureFeed_OnlineAccount $online_account
 *   The online account.
 *
 * @return array
 *   The form.
 */
function culturefeed_ui_online_account_publish_activities_form(array $form, array &$form_state, $online_account_type, CultureFeed_OnlineAccount $online_account) {

  $form_state['culturefeed_ui_online_account_type'] = $online_account_type;
  $form_state['culturefeed_ui_online_account'] = $online_account;

  $html_id = drupal_html_id('culturefeed-ui-online-account-publish-activities-' . $online_account_type);

  $statement = t(
    'I accept that my UiTiD actions on !site_name will be published automatically on !online_account_type.',
    array(
      '!site_name' => check_plain(variable_get('site_name', 'Drupal')),
      '!online_account_type' => $online_account_type,
    )
  );

  $form['wrapper'] = array(
    '#attributes' => array(
      'class' => array('statement'),
      'id' => $html_id,
    ),
    '#type' => 'container',
    'messages' => array(
      '#type' => 'markup',
    ),
    'publish' => array(
      '#ajax' => array(
        'callback' => 'culturefeed_ui_online_account_publish_activities_form_ajax_callback',
        'wrapper' => $html_id,
      ),
      '#default_value' => $online_account->publishActivities,
      '#executes_submit_callback' => TRUE,
      '#title' => $statement,
      '#type' => 'checkbox',
    ),
  );

  return $form;

}

/**
 * Ajax callback for the online account publish activities form.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 *
 * @return array
 *   The form.
 */
function culturefeed_ui_online_account_publish_activities_form_ajax_callback($form, $form_state) {
  $messages = theme('status_messages');
  if ($messages) {
    $form['wrapper']['messages'] = array(
      '#markup' => $messages,
    );
  }
  return $form['wrapper'];
}

/**
 * Submit handler for the online account publish activities form.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 *
 * @return array
 *   The form.
 */
function culturefeed_ui_online_account_publish_activities_form_submit(array $form, array &$form_state) {

  $online_account_type = $form_state['culturefeed_ui_online_account_type'];
  /* @var CultureFeed_OnlineAccount $online_account */
  $online_account =& $form_state['culturefeed_ui_online_account'];

  $account = new CultureFeed_OnlineAccount();
  $account->publishActivities = $form_state['values']['publish'];
  $account->accountName = $online_account->accountName;
  $account->accountType = $online_account_type;

  try {
    DrupalCultureFeed::updateUserOnlineAccount(DrupalCultureFeed::getLoggedInUserId(), $account);
    $online_account = DrupalCultureFeed::getUserOnlineAccount($online_account_type);
    $online_account->publishActivities = $account->publishActivities;
    drupal_set_message(t('Your account was updated successfully.'));
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    drupal_set_message(t('An error occurred when updating your account.'), 'error');
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

  $user_id = culturefeed_get_uid_for_cf_uid($culturefeed_user->id, $culturefeed_user->nick);

  $form['#culturefeed_user'] = $culturefeed_user;

  $form['setting'] = array(
    '#default_value' => $default_value,
    '#options' => $options,
    '#title' => t('Select the way your user details are visible to others in your <a href="@profile-url">public profile</a>.', array('@profile-url' => url('user/' . $user_id))),
    '#type' => 'radios',
  );

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    ),
  );

  // Tooltip.
  $info = t('All your user activities will be set to private too.');
  if ($info) {

    $form['#attached'] = array(
      'js' => array(
        drupal_get_path('module', 'culturefeed_ui') . '/js/privacy_tooltip.js',
        array(
          'type' => 'setting',
          'data' => array('culturefeed_ui_privacy_settings_anonymous_tooltip' => $info),
        ),
      ),
      'library' => array(
        array('system', 'ui.tooltip'),
      ),
    );

  }

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

  if ($setting == 'anonymous') {

    // Activities.
    $query = new CultureFeed_SearchActivitiesQuery();
    $query->userId = $culturefeed_user->id;
    $query->start = 0;
    $query->max = 1;

    $activities = array();
    do {

      /* @var CultureFeed_ResultSet $result_set */
      $result_set = DrupalCultureFeed::searchActivities($query);
      $activities = array_merge($activities, $result_set->objects);
      $query->start += $query->max;

    } while ($query->start < $result_set->total);

    foreach ($activities as $activity) {
      /* @var CultureFeed_Activity $activity */
      DrupalCultureFeed::updateActivity($activity->id, TRUE);
    }

    // Preferences.
    culturefeed_ui_set_all_activity_privacy_preferences($culturefeed_user, TRUE);

  }

  else {

    // Preferences.
    culturefeed_ui_set_all_activity_privacy_preferences($culturefeed_user, FALSE);

  }

  try {
    DrupalCultureFeed::updateUserPrivacy($culturefeed_user->id, $privacy_config);
    drupal_set_message(t('Your privacy settings have been saved.'));
  }
  catch (Exception $e) {
    form_set_error('submit', t('Error occurred while saving your privacy settings.'));
  }

  $uid = db_query("SELECT uid FROM {culturefeed_user} WHERE cf_uid = :uid", array(':uid' => $culturefeed_user->id))->fetchField();
  $cid = url('user/' . $uid, array('absolute' => TRUE));
  cache_clear_all($cid, 'cache_page');

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
function culturefeed_ui_profile_edit_form(array $form, array &$form_state, CultureFeed_User $culturefeed_user) {

  $form['#culturefeed_user'] = $culturefeed_user;
  $form['#theme'] = 'culturefeed_ui_profile_edit_form';

  form_load_include($form_state, 'inc', 'culturefeed_ui', 'culturefeed_ui.forms');

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
  $default_value = '';
  if (!empty($culturefeed_user->dob)) {
    $dob = new DateTime();
    $time_zone = new DateTimeZone('Europe/Brussels');
    $dob->setTimestamp($culturefeed_user->dob);
    $dob->setTimezone($time_zone);
    $default_value = $dob->format('d/m/Y');
  }
  $form['dob'] = array(
    '#title' => t('Date of birth'),
    '#type' => 'textfield',
    '#default_value' => $default_value,
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

  // Address.
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

  // Bio.
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

  $form['remove_account'] = array(
    '#prefix' => '<div id="remove-account">',
    '#markup' => l(t('Delete account'), 'culturefeed/removeaccount'),
    '#suffix' => '</div>',
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
  $culturefeed_user_update->dob = NULL;
  // For the moment this field is removed, so we default to Dutch.
  $culturefeed_user_update->preferredLanguage = (isset($values['preferredLanguage'])) ? $values['preferredLanguage'] : 'nl';

  if (!empty($values['dob'])) {

    $dob_parts = explode('/', $values['dob']);

    if (count($dob_parts) == 3) {

      list($day, $month, $year) = $dob_parts;

      if (is_numeric($day) && is_numeric($month) && is_numeric($year)) {

        $timezone = new DateTimeZone('Europe/Brussels');
        $dob = new DateTime($year . '-' . $month . '-' . $day, $timezone);
        $culturefeed_user_update->dob = $dob->getTimestamp();

      }

    }

  }

  $fields = array(
    'id', 'givenName', 'familyName', 'gender', 'bio', 'street', 'zip', 'city',
    'country', 'dob',
  );

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

/**
 * Form constructor for the users search form.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 *
 * @return array
 *   The form.
 */
function culturefeed_ui_users_search_form(array $form, array $form_state) {

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

/**
 * Submit handler for culturefeed_ui_profile_edit_form().
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 */
function culturefeed_ui_users_search_form_submit(array $form, array &$form_state) {

  $name = trim($form_state['values']['name']);

  if (!empty($name)) {
    $form_state['redirect'] = 'culturefeed/users/search/' . $name;
  }
  else {
    $form_state['redirect'] = 'culturefeed/users/search';
  }

}
