<?php

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

  $activities = DrupalCultureFeed::getConsumerInstance()->searchActivities($query);

  pager_default_initialize($activities->total, $pagelength);

  $items = array();

  $events = culturefeed_get_events_for_activities($activities->objects);

  foreach ($activities->objects as $activity) {
    $node = isset($events[$activity->nodeId]) ? $events[$activity->nodeId] : NULL;

    $items[] = theme('culturefeed_ui_activity_summary', array('activity' => $activity, 'node' => $node));
  }

  $data = array();


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
  $pagelength = 20; // @todo make this a const
  $page = isset($_GET['page']) ? $_GET['page'] : 0;

  $query = new CultureFeed_SearchUsersQuery();

  $query->max   = $pagelength;
  $query->start = $page * $pagelength;
  $query->sort  = CultureFeed_SearchUsersQuery::SORT_CREATIONDATE;
  $query->order = CultureFeed_SearchUsersQuery::SORT_ORDER_DESCENDING;

  if (!empty($name)) {
    $query->name = $name;
  }

  $users = DrupalCultureFeed::getConsumerInstance()->searchUsers($query);

  pager_default_initialize($users->total, $pagelength);

  $items = array();

  foreach ($users->objects as $user) {
    $items[] = theme('culturefeed_ui_user_summary', array('user' => $user));
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

  $form['givenName'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#default_value' => $cf_account->givenName,
  );
  $form['familyName'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name'),
    '#default_value' => $cf_account->familyName,
  );

  $form['#attributes']['enctype'] = 'multipart/form-data';
  
  $form['current_picture'] = array(
    '#theme' => 'image',
    '#path' => $cf_account->depiction,
  );

  $form['picture'] = array(
    '#type' => 'file',
    '#title' => t('Picture')
  );
  
  $form['gender'] = array(
    '#type' => 'radios',
    '#title' => t('Gender'),
    '#options' => array('male' => t('Male'), 'female' => t('Female')),
    '#default_value' => $cf_account->gender,
  );
  $form['homeAddress'] = array(
    '#type' => 'textfield',
    '#title' => t('Street and nr'),
    '#default_value' => $cf_account->homeAddress,
  );
  $form['bio'] = array(
    '#type' => 'textarea',
    '#title' => t('Bio'),
    '#default_value' => $cf_account->bio,
  );
  $form['dob'] = array(
    '#title' => t('Date of birth'),
    '#type' => 'textfield',
    '#default_value' => '',
    '#description' => t('Format is dd/mm/yyyy'),
    '#size' => 10,
  );

  if (module_exists('cnapi_browse')) {
    drupal_add_js("(function ($) { $(document).ready(function() { $('#edit-dob').datePicker({startDate:'01/01/1900'}); }); })(jQuery);", 'inline');

    $form['#attached']['library'][] = array('cnapi_browse', 'jquery.datePicker');
  }

  /*$form['country'] = array(
    '#type' => 'textfield',
    '#title' => t('Country'),
    '#default_value' => $cf_account->country,
  );*/

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

function culturefeed_ui_profile_edit_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $user_update = new CultureFeed_User();

  $user_update->id         = DrupalCultureFeed::getLoggedInAccount()->culturefeed_userid;
  $user_update->givenName  = $values['givenName'];
  $user_update->familyName = $values['familyName'];
  $user_update->gender     = $values['gender'];
  $user_update->bio        = $values['bio'];
  // $user_update->country    = $values['country'];

  $dob_parts = explode('/', $values['dob']);

  if (count($dob_parts) == 3) {
    list($day, $month, $year) = $dob_parts;

    $user_update->dob = mktime(0, 0, 0, (int)$month, (int)$day, (int)$year);
  }

  $cf = DrupalCultureFeed::getLoggedInUserInstance();

  try {
    $cf->updateUser($user_update);
  }
  catch (Exception $e) {
    drupal_set_message(t('Some error occurred'), 'error');
    return;
  }
  
  if ($_FILES['files']['tmp_name']['picture']) {
    global $user;
    
    $account = user_load($user->uid);

    $picture_path = $_FILES['files']['tmp_name']['picture'];

    $picture = file_get_contents($picture_path);

    try {
      $result = DrupalCultureFeed::getLoggedInUserInstance()->uploadUserDepiction($account->culturefeed_userid, $picture);
    }
    catch (Exception $e) {
      drupal_set_message(t('Some error occurred while uploading picture'), 'error');
      return;
    }
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

  $destination = '';

  $form['change_password'] = array(
    '#prefix' => '<div id="change-password">',
    '#markup' => l(t('Change password'), DrupalCultureFeed::getLoggedInUserInstance()->getUrlChangePassword(DrupalCultureFeed::getLoggedInAccount()->culturefeed_userid, $destination)),
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

  $cf = DrupalCultureFeed::getLoggedInUserInstance();

  $cf_account = new CultureFeed_User();

  $cf_account->id          = DrupalCultureFeed::getLoggedInAccount()->culturefeed_userid;
  $cf_account->mbox        = $mbox;
  $cf_account->homeAddress = 'hello';

  $cf->updateUser($cf_account);

  drupal_set_message(t('Updates saved.'));
}


function culturefeed_ui_page_most_active_users() {
  $cf = DrupalCultureFeed::getConsumerInstance();

  $query = new CultureFeed_SearchUsersQuery();
  $query->max = 10;
  $query->order = CultureFeed_SearchUsersQuery::SORT_ORDER_DESCENDING;
  $query->sort = variable_get('culturefeed_ui_block_most_active_user_sort', CultureFeed_SearchUsersQuery::SORT_NUMBEROFACTIVEACTIVITIES);

  $users = $cf->searchUsers($query)->objects;

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
  $userid = DrupalCultureFeed::getLoggedInAccount()->culturefeed_userid;
  DrupalCultureFeed::getLoggedInUserInstance()->revokeUserServiceConsumer($userid, $id);
  drupal_goto('culturefeed/serviceconsumers');
}

function culturefeed_ui_page_serviceconsumers() {
  $consumers = DrupalCultureFeed::getLoggedInUserInstance()->getUserServiceConsumers(DrupalCultureFeed::getLoggedInAccount()->culturefeed_userid);

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