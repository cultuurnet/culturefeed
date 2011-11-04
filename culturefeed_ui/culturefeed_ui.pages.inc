<?php

function culturefeed_ui_undo($activity_type, $content_type, $node_id) {
  $user_id = DrupalCultureFeed::getLoggedInAccount()->culturefeed_uid;
  $content_type = culturefeed_activity_content_type_from_cnapi_type($content_type);
  
  try {
    DrupalCultureFeed::deleteActivities($user_id, $node_id, $content_type, $activity_type);
  }
  catch (Exception $e) {
    drupal_set_message('Er is een fout opgetreden bij deze actie.', 'error');
    watchdog_exception('culturefeed_ui', $e);
  }
  
  if ($type == 'page') {
    drupal_goto('node/' . $node_id);
  }
  else {
    $object = cnapi_get(array('action' => 'detail', 'type' => $type, 'query' => array('cdbid' => $node_id)));
    
    $request = array($content_type => $node_id, 'title' => $object['detail']['nl']['title']);
    
    cnapi_ui_goto($request);
  }
}

function culturefeed_ui_do($activity_type, $type, $id) {
  $cf_account = DrupalCultureFeed::getLoggedInAccount();

  $activity = new CultureFeed_Activity();
  $activity->contentType = culturefeed_activity_content_type_from_cnapi_type($type);
  $activity->type = $activity_type;
  $activity->nodeId = $id;
  $activity->userId = $cf_account->culturefeed_uid;

  try {
    DrupalCultureFeed::createActivity($activity);
  }
  catch (Exception $e) {
    drupal_set_message('Er is een fout opgetreden bij deze actie.', 'error');
    watchdog_exception('culturefeed_ui', $e);
  }
  
  if ($type == 'page') {
    drupal_goto('node/' . $id);
  }
  else {
    $object = cnapi_get(array('action' => 'detail', 'type' => $type, 'query' => array('cdbid' => $id)));
    
    $request = array($type => $id, 'title' => $object['detail']['nl']['title']);
    
    cnapi_ui_goto($request);
  }
}

function culturefeed_ui_recommendations_delete($evaluation, $id = NULL, $type = 'ajax') {

  $evaluation = $evaluation == 'negative' ? CultureFeed::RECOMMENDATION_EVALUATION_NEGATIVE : CultureFeed::RECOMMENDATION_EVALUATION_POSITIVE;

  if ($type == 'ajax') {
    
    try {
      DrupalCultureFeed::evaluateRecommendation($id, $evaluation);
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_ui', $e);
    }

    $output = drupal_render(culturefeed_ui_page_recommendations());
    $commands = array();

    $commands[] = ajax_command_replace('#recommendations-list', $output);
    $page = array('#type' => 'ajax', '#commands' => $commands);
    ajax_deliver($page);
  }
  else {
    $output = t("This is some content delivered via a page load.");
    return $output;
  }
}

function culturefeed_ui_page_recommendations() {
  $query = new CultureFeed_RecommendationsQuery();
  $query->max = 20;
  
  try {
    $cf_account = DrupalCultureFeed::getLoggedInUser();
    $recommendations = DrupalCultureFeed::getRecommendationsForUser($cf_account->id, $query);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
  }
  
  $cdbids = array();
  
  foreach ($recommendations as $recommendation) {
    $cdbids[] = $recommendation->itemid;
  }
  
  $events = culturefeed_get_nodes_for_cdbids('event', $cdbids);
  
  $header = array('name', array('data' => 'link', 'colspan' => 2));
  
  $rows = array();
  
  foreach ($recommendations as $recommendation) {
    $event = isset($events[$recommendation->itemid]) ? $events[$recommendation->itemid] : NULL;
      
    $positive = array(
      '#type' => 'link',
      '#title' => 'positive',
      '#href' => 'culturefeed/recommendations/positive/' . $recommendation->id . '/nojs',
      '#ajax' => array(
        'wrapper' => 'recommendations-list',
        'method' => 'replace',
      ),
    );
      
    $negative = array(
      '#type' => 'link',
      '#title' => 'negative',
      '#href' => 'culturefeed/recommendations/negative/' . $recommendation->id . '/nojs',
      '#ajax' => array(
        'wrapper' => 'recommendations-list',
        'method' => 'replace',
      ),
    );
    
    $title = 'unknown';
    if (isset($event)) {
      $title = $event['title'] . time();
    }
    
    $rows[] = array($title, drupal_render($negative), drupal_render($positive));
  }
  
  return array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#prefix' => '<div id="recommendations-list">',
    '#suffix' => '</div>',
  );
}

function culturefeed_ui_page_mboxverify_resend() {
  if (user_is_anonymous() || !DrupalCultureFeed::isCultureFeedUser()) {
    return;
  }
  
  try {
    $cf_account = DrupalCultureFeed::getLoggedInUser();
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
  }
  
  if ($cf_account->mboxVerified) {
    return;
  }
  
  try {
    DrupalCultureFeed::resendMboxConfirmationForUser($cf_account->id);
    drupal_set_message('De bevestigingsmail werd met succes verzonden.');
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    drupal_set_message('De bevestigingsmail kon niet verstuur worden.', 'error');
  }
  
  drupal_goto('<front>');
}

function culturefeed_ui_page_users_likes($type, $id) {
  if (!in_array($type, array('actor', 'event', 'production', 'page'))) {
    drupal_not_found();
  }
  
  $pagelength = 20;
  $page = isset($_GET['page']) ? $_GET['page'] : 0;
  
  $request = array('action' => 'detail', 'type' => $type, 'query' => array('cdbid' => $cdbid));
  $object = cnapi_get($request);
  
  $query = new CultureFeed_SearchActivitiesQuery();
  $query->type = CultureFeed_Activity::TYPE_LIKE;
  $query->nodeId = $id;

  try {
    $users = DrupalCultureFeed::searchActivityUsers($id, CultureFeed_Activity::TYPE_LIKE);
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

function culturefeed_ui_page_users_attends($type, $cdbid) {
  if (!in_array($type, array('actor', 'event'))) {
    drupal_not_found();
  }
  
  $pagelength = 20;
  $page = isset($_GET['page']) ? $_GET['page'] : 0;
  
  $request = array('action' => 'detail', 'type' => $type, 'query' => array('cdbid' => $cdbid));
  $object = cnapi_get($request);
  
  $query = new CultureFeed_SearchActivitiesQuery();
  $query->type = CultureFeed_Activity::TYPE_IK_GA;
  $query->nodeId = $cdbid;

  try {
    $users = DrupalCultureFeed::searchActivityUsers($cdbid, CultureFeed_Activity::TYPE_IK_GA);
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

  $query->contentType = CultureFeed_Activity::CONTENT_TYPE_EVENT;
  $query->max         = $pagelength;
  $query->start       = $page * $pagelength;
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
    '#path' => $cf_account->depiction . '?maxwidth=100&maxheight=100&crop=auto',
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

  if (module_exists('cnapi_browse')) {
    drupal_add_js("(function ($) { $(document).ready(function() { $('.form-item-dob .description').hide(); $('#edit-dob').datePicker({startDate:'01/01/1900'}); }); })(jQuery);", 'inline');

    $form['#attached']['library'][] = array('cnapi_browse', 'jquery.datePicker');
  }
  
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
  return array(
    'account' => drupal_get_form('culturefeed_ui_account_edit_form'),
    'online_accounts' => drupal_get_form('culturefeed_ui_online_accounts_form'),
    'consumers' => array(
      '#prefix' => '<div id="manage-consumers">',
      '#markup' => l(t('Manage consumers'), 'culturefeed/serviceconsumers'),
      '#suffix' => '</div>',
    )
  );
  //return drupal_render(drupal_get_form('culturefeed_ui_account_edit_form')) . drupal_render(drupal_get_form('culturefeed_ui_online_accounts_form'));
}

function culturefeed_ui_online_accounts_form() {
  $form = array();
  
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
  
  $form['#theme'] = 'culturefeed_ui_online_accounts_form';
  
  $form['accounts']['#tree'] = TRUE;

  foreach ($supported_account_types as $account_type) {
    if (isset($online_accounts[$account_type])) {
      $account = $online_accounts[$account_type];

      $form['accounts'][$account_type]['id'] = array(
        '#type' => 'hidden',
        '#value' => $account->accountName,
      );
      $form['accounts'][$account_type]['name'] = array(
        '#markup' => $account->accountName,
      );
      $form['accounts'][$account_type]['nick'] = array(
        '#markup' => $account->accountNick,
      );
      $form['accounts'][$account_type]['depiction'] = array(
        '#markup' => $account->accountDepiction,
      );
      $form['accounts'][$account_type]['publish'] = array(
        '#type' => 'checkbox',
        '#title' => 'Ik geef toestemming aan UiTid om mijn acties op ' . variable_get('site_name', 'Drupal') .  ' automatisch te publiceren op ' . $account_type . '.',
        '#default_value' => $account->publishActivities,
      );
      $form['accounts'][$account_type]['delete_link'] = array(
        '#markup' => l('Account verwijderen', 'culturefeed/onlineaccounts/delete/' . $account_type . '/' . $account->accountName),
      );
    }
    else {  
      $redirect = url('culturefeed/account/edit', array('absolute' => TRUE));
      $form['accounts'][$account_type]['connect_link'] = array(
        '#markup' => l('Account toevoegen', DrupalCultureFeed::getUrlAddSocialNetwork($account_type, $redirect), array('attributes' => array('class' => 'culturefeedconnect'))),
      );
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

function theme_culturefeed_ui_online_accounts_form($variables) {
  $form = $variables['form'];
  
  $items = array();

  foreach (element_children($form['accounts']) as $account_type) {
    $vars = array(
      'type' => $account_type, 
      'name' => drupal_render($form['accounts'][$account_type]['name']), 
      'nick' => drupal_render($form['accounts'][$account_type]['nick']), 
      'depiction' => drupal_render($form['accounts'][$account_type]['depiction']), 
      'publish_checkbox' => drupal_render($form['accounts'][$account_type]['publish']), 
      'delete_link' => drupal_render($form['accounts'][$account_type]['delete_link']),
      'connect_link' => drupal_render($form['accounts'][$account_type]['connect_link']),
    );
    
    $items[] = theme('culturefeed_ui_online_account', $vars);
  }

  return theme('item_list', array('items' => $items)) . drupal_render_children($form);
}

function culturefeed_ui_online_accounts_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  
  foreach ($values['accounts'] as $type => $account_info) {
    try {
      $cf_uid = DrupalCultureFeed::getLoggedInUserId();

      $account = new CultureFeed_OnlineAccount();
      $account->publishActivities = $account_info['publish'] ? true : false;
      $account->accountName = $account_info['id'];
      $account->accountType = $type;

      DrupalCultureFeed::updateUserOnlineAccount($cf_uid, $account);
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_ui', $e);
      return;
    }
  }
}

function culturefeed_ui_page_online_account_delete_form($form, $form_state, $type, $id) {
  return confirm_form(
    array(
      'type' => array(
        '#type' => 'value',
        '#value' => $type,
      ),
      'id' => array(
        '#type' => 'value',
        '#value' => $id,
      ),
    ),
    t('Are you sure you want to delete %type account?', array('%type' => $type)),
    'culturefeed/account/edit',
    t('This action cannot be undone.'),
    t('Delete account'),
    t('Cancel')
  );
}

function culturefeed_ui_page_online_account_delete_form_submit($form, &$form_state) {
  $type = $form_state['values']['type'];
  $id = $form_state['values']['id'];
  
  try {
    $cf_uid = DrupalCultureFeed::getLoggedInUserId();
    DrupalCultureFeed::deleteUserOnlineAccount($cf_uid, $type, $id);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    return;
  }

  $form_state['redirect'] = 'culturefeed/account/edit';
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

  $users = DrupalCultureFeed::searchUsers($query);
  
  $items = array();
  
  $accounts = culturefeed_get_uids_for_users($users->objects);

  foreach ($users->objects as $user) {
    if (!isset($accounts[$user->id])) {
      continue;
    }
    
    $items[] = theme('culturefeed_ui_ranked_user_summary', array('user' => $user, 'uid' => $accounts[$user->id]));
  }
  
  if (empty($items)) {
    return;
  }

  return array(
    '#theme' => 'item_list',
    '#items' => $items,
    '#type' => 'ol',
  );
}

function culturefeed_ui_page_serviceconsumers() {
  try {
    $consumers = DrupalCultureFeed::getUserServiceConsumers(DrupalCultureFeed::getLoggedInUserId());
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
  }

  $items = array();

  foreach ($consumers as $consumer) {
    $items[] = theme('culturefeed_ui_service_consumer_summary', array('consumer' => $consumer));
  }
  
  if (empty($items)) {
    return;
  }

  return array(
    '#theme' => 'item_list',
    '#items' => $items,
  );
}

function culturefeed_ui_page_serviceconsumers_revoke_form($form, $form_state, $id) {
  try {
    $consumers = DrupalCultureFeed::getUserServiceConsumers(DrupalCultureFeed::getLoggedInUserId());
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    drupal_set_message(t('Error occured.'), 'error');
    return;
  }
  
  $consumer = NULL;
  
  foreach ($consumers as $tmp_consumer) {
    if ($tmp_consumer->id == $id) {
      $consumer = $tmp_consumer;
      break;
    }
  }
  
  if (!$consumer) {
    drupal_set_message(t('Unknown conumser.'), 'error');
    return;
  }
  
  return confirm_form(
    array(
      'consumer' => array(
        '#type' => 'value',
        '#value' => $id,
      ),
    ),
    t('Are you sure you want to consumer %consumer?', array('%consumer' => $consumer->name)),
    'culturefeed/serviceconsumers',
    t('This action cannot be undone.'),
    t('Remove consumer'),
    t('Cancel')
  );
}

function culturefeed_ui_page_serviceconsumers_revoke_form_submit($form, &$form_state) {
  $id = $form_state['values']['consumer'];
  
  try {
    $cf_uid = DrupalCultureFeed::getLoggedInUserId();
    DrupalCultureFeed::revokeUserServiceConsumer($cf_uid, $id);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    return;
  }

  $form_state['redirect'] = 'culturefeed/serviceconsumers';
}