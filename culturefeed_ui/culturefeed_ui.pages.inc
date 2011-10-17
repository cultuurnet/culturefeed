<?php

function culturefeed_ui_page_users_like($type, $cdbid) {
  if (!in_array($type, array('actor', 'event', 'production'))) {
    drupal_not_found();
  }
  
  $pagelength = 20;
  $page = isset($_GET['page']) ? $_GET['page'] : 0;
  
  $request = array('action' => 'detail', 'type' => $type, 'query' => array('cdbid' => $cdbid));
  $object = cnapi_get($request);
  
  $query = new CultureFeed_SearchActivitiesQuery();
  $query->type = CultureFeed_Activity::TYPE_LIKE;
  $query->nodeId = $cdbid;

  try {
    $users = DrupalCultureFeed::searchActivityUsers($cdbid, CultureFeed_Activity::TYPE_LIKE, CultureFeed_Activity::CONTENT_TYPE_EVENT);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    return;
  }

  pager_default_initialize($users->total, $pagelength);

  $items = culturefeed_ui_users_list($users);
  
  if (empty($items)) {
    return;
  }

  return array(
    array(
      '#theme' => 'item_list',
      '#items' => $items,
    ),
    array(
      '#theme' => 'pager',
    ),
  );
}

function culturefeed_ui_page_recent_activities() {
  $pagelength = 20;
  $page = isset($_GET['page']) ? $_GET['page'] : 0;

  $query = new CultureFeed_SearchActivitiesQuery();

  $query->max         = $pagelength;
  $query->start       = $page * $pagelength;
  $query->contentType = CultureFeed_Activity::CONTENT_TYPE_EVENT;
  $query->type        = array(
    CultureFeed_Activity::TYPE_LIKE,
    CultureFeed_Activity::TYPE_FACEBOOK,
    CultureFeed_Activity::TYPE_TWITTER,
    CultureFeed_Activity::TYPE_IK_GA
  );

  $activities = DrupalCultureFeed::searchActivities($query);

  pager_default_initialize($activities->total, $pagelength);

  $items = culturefeed_ui_activities_list($activities);
  
  if (empty($items)) {
    return;
  }

  return array(
    array(
      '#theme' => 'item_list',
      '#items' => $items,
    ),
    array(
      '#theme' => 'pager',
    ),
  );
}

function culturefeed_ui_page_search_users($name = '') {
  return array(
    drupal_get_form('culturefeed_ui_search_users_form'),
    culturefeed_ui_search_users_results($name)
  );
}

function culturefeed_ui_search_users_results($name = '') {
  $pagelength = 10; // @todo make this a const
  $page = isset($_GET['page']) ? $_GET['page'] : 0;

  $query = new CultureFeed_SearchUsersQuery();

  $query->max   = $pagelength;
  $query->start = $page * $pagelength;
  $query->sort  = CultureFeed_SearchUsersQuery::SORT_CREATIONDATE;
  $query->order = CultureFeed_SearchUsersQuery::SORT_ORDER_DESCENDING;

  if (!empty($name)) {
    $query->name = $name;
  }

  $users = DrupalCultureFeed::searchUsers($query);

  pager_default_initialize($users->total, $pagelength);

  $items = culturefeed_ui_users_list($users);
  
  if (empty($items)) {
    return;
  }

  return array(
    array(
      '#theme' => 'item_list',
      '#items' => $items,
    ),
    array(
      '#theme' => 'pager',
    ),
  );
}

function culturefeed_ui_profile_edit_form() {
  $form = array();

  if (!DrupalCultureFeed::isCultureFeedUser()) {
    return;
  }

  $cf_account = DrupalCultureFeed::getLoggedInUser();

  // Firstname.
  $form['givenName'] = array(
    '#type' => 'textfield',
    '#title' => 'Voornaam',
    '#default_value' => $cf_account->givenName,
  );
  $form['givenNamePrivacy'] = array(
    '#type' => 'checkbox',
    '#title' => 'Verberg \'voornaam\' in openbaar profiel',
    '#default_value' => $cf_account->privacyConfig->givenName == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
  );
  
  // Name.
  $form['familyName'] = array(
    '#type' => 'textfield',
    '#title' => 'Naam',
    '#default_value' => $cf_account->familyName,
  );
  $form['familyNamePrivacy'] = array(
    '#type' => 'checkbox',
    '#title' => 'Verberg \'naam\' in openbaar profiel',
    '#default_value' => $cf_account->privacyConfig->familyName == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
  );

  // Picture.
  $form['#attributes']['enctype'] = 'multipart/form-data';

  $form['picture'] = array(
    '#type' => 'file',
    '#title' => 'Kies een foto',
  );

  $form['current_picture'] = array(
    '#theme' => 'image',
    '#path' => $cf_account->depiction . '?width=100&height=100&crop=auto',
  );

  // Gender.
  $form['gender'] = array(
    '#type' => 'radios',
    '#title' => 'Geslacht',
    '#options' => array('male' => 'Man', 'female' => 'Vrouw'),
    '#default_value' => $cf_account->gender,
  );
  $form['genderPrivacy'] = array(
    '#type' => 'checkbox',
    '#title' => 'Verberg \'geslacht\' in openbaar profiel',
    '#default_value' => $cf_account->privacyConfig->gender == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
  );
  
  // Address
  $form['street'] = array(
    '#type' => 'textfield',
    '#title' => 'Straatnaam en nummer',
    '#default_value' => $cf_account->street,
  );
  $form['zip'] = array(
    '#type' => 'textfield',
    '#title' => 'Postcode',
    '#default_value' => $cf_account->zip,
  );
  $form['city'] = array(
    '#type' => 'textfield',
    '#title' => 'Woonplaats',
    '#default_value' => $cf_account->city,
  );
  $form['country'] = array(
    '#type' => 'textfield',
    '#title' => 'Land',
    '#default_value' => $cf_account->country,
  );
  $form['homeAddressPrivacy'] = array(
    '#type' => 'checkbox',
    '#title' => 'Verberg \'woonplaats\' in openbaar profiel',
    '#default_value' => $cf_account->privacyConfig->homeAddress == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
  );
  
  // Date of birth.
  $form['dob'] = array(
    '#title' => 'Geboortedatum',
    '#type' => 'textfield',
    '#default_value' => $cf_account->dob ? date('d/m/Y', $cf_account->dob) : '',
    '#description' => t('Format is dd/mm/yyyy'),
    '#size' => 10,
  );
  $form['dobPrivacy'] = array(
    '#type' => 'checkbox',
    '#title' => 'Verberg \'geboortedatum\' in openbaar profiel',
    '#default_value' => $cf_account->privacyConfig->dob == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
  );
  
  // Bio
  $form['bio'] = array(
    '#type' => 'textarea',
    '#title' => 'Beschrijf jezelf',
    '#default_value' => $cf_account->bio,
  );
  $form['bioPrivacy'] = array(
    '#type' => 'checkbox',
    '#title' => 'Verberg \'bio\' in openbaar profiel',
    '#default_value' => $cf_account->privacyConfig->bio == CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE,
  );

  if (module_exists('cnapi_browse')) {
    drupal_add_js("(function ($) { $(document).ready(function() { $('#edit-dob').datePicker({startDate:'01/01/1900'}); }); })(jQuery);", 'inline');

    $form['#attached']['library'][] = array('cnapi_browse', 'jquery.datePicker');
  }
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

function culturefeed_ui_profile_edit_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $user_update = new CultureFeed_User();

  $user_update->id         = DrupalCultureFeed::getLoggedInUserId();
  $user_update->givenName  = $values['givenName'];
  $user_update->familyName = $values['familyName'];
  $user_update->gender     = $values['gender'];
  $user_update->bio        = $values['bio'];
  $user_update->street     = $values['street'];
  $user_update->zip        = $values['zip'];
  $user_update->city       = $values['city'];
  $user_update->country    = $values['country'];

  $dob_parts = explode('/', $values['dob']);

  if (count($dob_parts) == 3) {
    list($day, $month, $year) = $dob_parts;

    $user_update->dob = mktime(0, 0, 0, (int)$month, (int)$day, (int)$year);
  }

  try {
    DrupalCultureFeed::updateUser($user_update);
  }
  catch (Exception $e) {
    drupal_set_message(t('Some error occurred'), 'error');
    watchdog_exception('culturefeed_ui', $e);
    return;
  }

  if ($_FILES['files']['tmp_name']['picture']) {
    global $user;

    $account = user_load($user->uid);

    $picture_path = $_FILES['files']['tmp_name']['picture'];

    $picture = file_get_contents($picture_path);

    try {
      DrupalCultureFeed::uploadUserDepiction($account->culturefeed_uid, $picture);
    }
    catch (Exception $e) {
      drupal_set_message(t('Some error occurred while uploading picture'), 'error');
      watchdog_exception('culturefeed_ui', $e);
      return;
    }
  }

  $privacy_config = new CultureFeed_UserPrivacyConfig();

  $privacy_config->givenName   = $values['givenNamePrivacy'] ? CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE : CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;
  $privacy_config->familyName  = $values['familyNamePrivacy'] ? CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE : CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;
  $privacy_config->gender      = $values['genderPrivacy'] ? CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE : CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;
  $privacy_config->homeAddress = $values['genderPrivacy'] ? CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE : CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;
  $privacy_config->dob         = $values['dobPrivacy'] ? CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE : CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;
  $privacy_config->bio         = $values['bioPrivacy'] ? CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE : CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC;

  try {
    DrupalCultureFeed::updateUserPrivacy(DrupalCultureFeed::getLoggedInUserId(), $privacy_config);
  }
  catch (Exception $e) {
    drupal_set_message(t('Some error occurred while trying to update your privacy settings.'), 'error');
    watchdog_exception('culturefeed_ui', $e);
    return;
  }

  drupal_set_message(t('Updates saved.'));
}

function culturefeed_ui_page_account_edit() {
  return drupal_render(drupal_get_form('culturefeed_ui_account_edit_form')) . culturefeed_ui_online_accounts();
}

function culturefeed_ui_online_accounts() {
  if (!DrupalCultureFeed::isCultureFeedUser()) {
    return;
  }

  $cf_account = DrupalCultureFeed::getLoggedInUser();

  $online_accounts = array();

  if ($cf_account->holdsAccount) {
    foreach ($cf_account->holdsAccount as $online_account) {
      $online_accounts[$online_account->accountType] = $online_account;
    }
  }

  $supported_account_types = array('twitter', 'facebook', 'google');

  foreach ($supported_account_types as $account_type) {
    if (isset($online_accounts[$account_type])) {

    }
  }

  return print_r($online_accounts, TRUE);
}

function culturefeed_ui_account_edit_form() {
  $form = array();

  if (!DrupalCultureFeed::isCultureFeedUser()) {
    return;
  }

  $cf_account = DrupalCultureFeed::getLoggedInUser();

  $form['nick'] = array(
    '#type' => 'textfield',
    '#title' => t('Nick'),
    '#disabled' => TRUE,
    '#value' => $cf_account->nick,
  );

  // @todo validate email
  $form['mbox'] = array(
    '#type' => 'textfield',
    '#title' => t('Mail'),
    '#default_value' => $cf_account->mbox,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  
  $destination = drupal_get_destination();
  $destination = url($destination['destination'], array('absolute' => TRUE, 'query' => array('closepopup' => 'true')));
  
  $url = DrupalCultureFeed::getUrlChangePassword(DrupalCultureFeed::getLoggedInUserId(), $destination);

  $options = array('attributes' => array('class' => array('culturefeedconnect')), 'query' => drupal_get_destination());

  $form['change_password'] = array(
    '#prefix' => '<div id="change-password">',
    '#markup' => l(t('Change password'), $url, $options),
    '#suffix' => '</div>',
  );

  $form['manage_consumers'] = array(
    '#prefix' => '<div id="manage-consumers">',
    '#markup' => l(t('Manage consumers'), 'culturefeed/serviceconsumers'),
    '#suffix' => '</div>',
  );

  return $form;
}

function culturefeed_ui_account_edit_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $mbox = $values['mbox'];

  $cf_account = new CultureFeed_User();

  $cf_account->id          = DrupalCultureFeed::getLoggedInUserId();
  $cf_account->mbox        = $mbox;

  DrupalCultureFeed::updateUser($cf_account);

  drupal_set_message(t('Updates saved.'));
}


function culturefeed_ui_page_most_active_users() {
  $query = new CultureFeed_SearchUsersQuery();
  $query->max = 10;
  $query->order = CultureFeed_SearchUsersQuery::SORT_ORDER_DESCENDING;
  $query->sort = variable_get('culturefeed_ui_block_most_active_user_sort', CultureFeed_SearchUsersQuery::SORT_NUMBEROFACTIVEACTIVITIES);

  $users = DrupalCultureFeed::searchUsers($query)->objects;

  $items = array();

  foreach ($users as $user) {
    $items[] = theme('image', array('path' => $user->depiction)) . '<br />' . $user->nick . '<br />' . $user->sortValue;
  }

  return array(
    '#theme' => 'item_list',
    '#items' => $items,
    '#type' => 'ol',
  );
}

function culturefeed_ui_page_serviceconsumers_revoke($id) {
  $cf_uid = DrupalCultureFeed::getLoggedInUserId();
  DrupalCultureFeed::revokeUserServiceConsumer($cf_uid, $id);
  drupal_goto('culturefeed/serviceconsumers');
}

function culturefeed_ui_page_serviceconsumers() {
  $consumers = DrupalCultureFeed::getUserServiceConsumers(DrupalCultureFeed::getLoggedInUserId());

  $items = array();

  foreach ($consumers as $consumer) {
    $output = '';
    $output .= ($consumer->logo ? theme('image' , array('path' => $consumer->logo)) : '') . '<br />';
    $output .= $consumer->name . '<br />';
    $output .= $consumer->description . '<br />';
    $output .= format_date($consumer->creationDate) . '<br />';
    $output .= l(t('Revoke'), 'culturefeed/serviceconsumers/revoke/' . $consumer->id);
    $items[] = $output;
  }

  return array(
    '#theme' => 'item_list',
    '#items' => $items,
  );
}