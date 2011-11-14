<?php

function culturefeed_ui_page_recommendations($zip) {
  $recommendations = culturefeed_ui_recommendations_list(100);
  
  return array(
    'filter' => drupal_get_form('culturefeed_ui_recommendations_filter_form'),
    'recommendations' => $recommendations,
  );
}

function culturefeed_ui_recommendations_evaluate($evaluation, $id = NULL, $cdbid = NULL, $type = 'ajax') {
  switch ($evaluation) {
    case 'unlike':
      _culturefeed_ui_undo(CultureFeed_Activity::TYPE_LIKE, CultureFeed_Activity::CONTENT_TYPE_EVENT, $cdbid);
      
      try {
        DrupalCultureFeed::evaluateRecommendation($id, CultureFeed::RECOMMENDATION_EVALUATION_NEGATIVE);
      }
      catch (Exception $e) {
        watchdog_exception('culturefeed_ui', $e);
      }
      
      break;
      
    case 'negative':

      try {
        DrupalCultureFeed::evaluateRecommendation($id, CultureFeed::RECOMMENDATION_EVALUATION_POSITIVE);
      }
      catch (Exception $e) {
        watchdog_exception('culturefeed_ui', $e);
      }
      break;
      
    case 'positive':
    
      try {
        DrupalCultureFeed::evaluateRecommendation($id, CultureFeed::RECOMMENDATION_EVALUATION_POSITIVE);
      }
      catch (Exception $e) {
        watchdog_exception('culturefeed_ui', $e);
      }
      
      _culturefeed_ui_do(CultureFeed_Activity::TYPE_LIKE, CultureFeed_Activity::CONTENT_TYPE_EVENT, $cdbid);
      
      break;
  }
  
  
  if ($type == 'ajax') {
    $commands = array();

    switch ($evaluation) {
      case 'unlike':
      
      case 'negative':
        $commands[] = ajax_command_remove('#recommendation-' . $id);
        break;
        
      case 'positive':
        $commands[] = ajax_command_remove('#recommendation-negative-' . $id);
        
        $unlike = array(
          '#type' => 'link',
          '#title' => 'Ik vind dit niet meer leuk',
          '#href' => 'culturefeed/recommendations/unlike/' . $id . '/' . $cdbid . '/nojs',
          '#attributes' => array('id' => 'recommendation-unlike-' . $recommendation->id),
          '#options' => array('query' => drupal_get_destination()),
          '#ajax' => array(
            'wrapper' => 'recommendation-' . $recommendation->id,
          ),
        );
        
        $commands[] = ajax_command_replace('#recommendation-positive-' . $id, drupal_render($unlike));
        
        break;
    }

    $page = array('#type' => 'ajax', '#commands' => $commands);
    ajax_deliver($page);
  }
  else {
    drupal_goto();
  }
}

function culturefeed_ui_page_do($activity_type, $content_type, $node_id, $redirect = TRUE) {
  $error = _culturefeed_ui_do($activity_type, $content_type, $node_id);
  
  if ($redirect) {
    if ($error) {
      drupal_set_message('Er is een fout opgetreden bij deze actie.', 'error');
    }
    
    culturefeed_ui_activity_based_goto($content_type, $node_id);
  }
  else {
    if ($error) {
      $status = 'ERROR';
    }
    else {
      $status = 'OK';  
    }
    
    drupal_json_output(array('status' => $status));
  }
}

function culturefeed_ui_page_undo($activity_type, $content_type, $node_id) {
  _culturefeed_ui_undo($activity_type, $content_type, $node_id);
  
  if ($error) {
    drupal_set_message('Actie kon niet ongedaan gemaakt worden.');
  }
  
  culturefeed_ui_activity_based_goto($content_type, $node_id);
}

function culturefeed_ui_page_mboxverify_resend() {
  try {
    $cf_account = DrupalCultureFeed::getLoggedInUser();
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    drupal_set_message('Er is een fout opgetreden. Probeer later opnieuw.', 'error');
    
    drupal_goto('<front>');
  }
  
  if (!$cf_account->mboxVerified) {
    try {
      DrupalCultureFeed::resendMboxConfirmationForUser($cf_account->id);
      drupal_set_message('De bevestigingsmail werd met succes verzonden.');
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_ui', $e);
      drupal_set_message('De bevestigingsmail kon niet verstuur worden.', 'error');
    }
  }
  else {
    drupal_set_message('Het e-mailadres verbonden aan deze account werd reeds bevestigd.', 'error');
  }
  
  drupal_goto('<front>');
}

function culturefeed_ui_page_users_activity($type, $content_type, $id) {
  if (!in_array($content_type, array('actor', 'event', 'production', 'page'))) {
    drupal_not_found();
  }
  
  if ($content_type == 'page') {
    $node = node_load($id);
    $page_title = $node->title;
    $page_link = array('path' => 'node/' . $node->nid);
  }
  else {
    $request = array('action' => 'detail', 'type' => $content_type, 'query' => array('cdbid' => $id));
    $object = cnapi_get($request);

    $page_title = $object['detail']['nl']['title'];
    
    $page_link = cnapi_url_dp2dua(array($content_type => $id, 'title' => $page_title));
  }
  
  switch ($type) {
    case CultureFeed_Activity::TYPE_LIKE:
      $title = sprintf('Zijn vonden "%s" tof.', truncate_utf8($page_title, 20, TRUE, TRUE));
      break;
    case CultureFeed_Activity::TYPE_IK_GA:
      $title = sprintf('Zijn gaan naar "%s".', truncate_utf8($page_title, 20, TRUE, TRUE));
      break;
  }
  
  if ($title) {
    drupal_set_title($title);
  }
  
  $pagelength = 20;
  $page = isset($_GET['page']) ? $_GET['page'] : 0;
  
  $query = new CultureFeed_SearchActivitiesQuery();
  $query->type = $type;
  $query->nodeId = $id;

	$activity_content_type = culturefeed_activity_content_type_from_cnapi_type($content_type);

  try {
    $users = DrupalCultureFeed::searchActivityUsers($id, $type, $activity_content_type);
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
      '#markup' => l('Terug naar ' . $page_title, $page_link['path']),
    ),
    array(
      '#theme' => 'item_list',
      '#items' => $items,
      '#attributes' => array('class' => 'user-list'),
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
      '#attributes' => array('class' => 'user-activity-list'),
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
      '#attributes' => array('class' => 'user-list'),
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
    '#path' => $cf_account->depiction . '?maxwidth=50&maxheight=50&crop=auto',
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
    '#value' => 'Opslaan',
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
    drupal_set_message('Er is een fout opgetreden bij het bewaren van jouw gegevens.', 'error');
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
      drupal_set_message('Er is een fout opgetreden bij het opladen van jouw afbeelding.', 'error');
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
    drupal_set_message('Er is een fout opgetreden bij het bewaren van jouw privacy instellingen.', 'error');
    watchdog_exception('culturefeed_ui', $e);
    return;
  }

  drupal_set_message(t('Updates saved.'));
}

function culturefeed_ui_page_account_edit() {
  return array(
    'account' => drupal_get_form('culturefeed_ui_account_edit_form'),
    'online_accounts_title' => array(
      '#markup' => '<h2>Gekoppelde accounts</h2>'
    ),
    'online_accounts' => drupal_get_form('culturefeed_ui_online_accounts_form'),
    'consumers' => array(
      '#prefix' => '<div id="manage-consumers"><h2>Gekoppelde toepassingen</h2>',
      '#markup' => 'Beheer alle ' . l('websites en toepassingen', 'culturefeed/serviceconsumers') . ' die momenteel gebruik maken van je UiTid.',
      '#suffix' => '</div>',
    )
  );
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
        '#title' => 'Ik geef toestemming aan UiTid om mijn acties op ' . variable_get('site_name', 'Drupal') .  ' automatisch te publiceren op ' . $account_type . '.' . l('Gebruiksvoorwarden', 'node/2512'),
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

  try {
    $cf_account = DrupalCultureFeed::getLoggedInUser();
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    drupal_set_message('Er is een fout opgetreden bij het laden van het jouw account. Probeer het later opnieuw.');
    return;
  }

  $form['nick'] = array(
    '#type' => 'textfield',
    '#title' => 'Gebruikersnaam',
    '#disabled' => TRUE,
    '#value' => $cf_account->nick,
  );

  $form['mbox'] = array(
    '#type' => 'textfield',
    '#title' => 'E-mailadres',
    '#default_value' => $cf_account->mbox,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Opslaan',
  );
  
  $destination = drupal_get_destination();
  $destination = url($destination['destination'], array('absolute' => TRUE, 'query' => array('closepopup' => 'true')));
  
  $url = DrupalCultureFeed::getUrlChangePassword(DrupalCultureFeed::getLoggedInUserId(), $destination);

  $options = array('attributes' => array('class' => array('culturefeedconnect')), 'query' => drupal_get_destination());

  $form['change_password'] = array(
    '#prefix' => '<div id="change-password">',
    '#markup' => l('Paswoord wijzigen', $url, $options),
    '#suffix' => '</div>',
  );

  return $form;
}

function culturefeed_ui_account_edit_form_validate($form, &$form_state) {
  if (!valid_email_address($form_state['values']['mbox'])) {
    form_set_error('mbox', 'Ongeldig e-mailadres.');
  }
}

function culturefeed_ui_account_edit_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $mbox = $values['mbox'];

  $cf_account = new CultureFeed_User();

  $cf_account->id          = DrupalCultureFeed::getLoggedInUserId();
  $cf_account->mbox        = $mbox;

  DrupalCultureFeed::updateUser($cf_account);

  drupal_set_message('Jouw aanpassingen zijn bewaard.');
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
    drupal_set_message('Er is een fout opgetreden.', 'error');
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
    drupal_set_message('Er is een fout opgetreden : ongekend kanaal.', 'error');
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