<?php

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
function culturefeed_ui_privacy_settings_form(array $form, array &$form_state, $culturefeed_user) {

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