<?php

function culturefeed_ui_page_remove_account_form($form, &$form_state) {
  return confirm_form($form,
    t('Are you sure you want to delete your account?'),
    isset($_GET['destination']) ? $_GET['destination'] : 'culturefeed/account/edit',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

function culturefeed_ui_page_remove_account_form_submit($form, &$form_state) {
  $cf_uid = DrupalCultureFeed::getLoggedInUserId();

  try {
    DrupalCultureFeed::deleteUser($cf_uid);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    drupal_set_message(t('An error occurred while deleting your account.'));
    return;
  }
}

function culturefeed_ui_page_users_search($name = '') {
  return array(
    'form' => drupal_get_form('culturefeed_ui_users_search_form'),
    'results' => _culturefeed_ui_page_users_search_results($name)
  );
}

function _culturefeed_ui_page_users_search_results($name = NULL) {
  $pagelength = CULTUREFEED_UI_PAGE_USERS_SEARCH_COUNT;
  $page = isset($_GET['page']) ? $_GET['page'] : 0;

  $query = new CultureFeed_SearchUsersQuery();

  $query->max   = $pagelength;
  $query->start = $page * $pagelength;
  $query->sort  = CultureFeed_SearchUsersQuery::SORT_CREATIONDATE;
  $query->order = CultureFeed_SearchUsersQuery::SORT_ORDER_DESCENDING;

  if (!empty($name)) {
    $query->name = $name;
  }

  try {
    $users = DrupalCultureFeed::searchUsers($query);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    drupal_set_message(t('Error occurred'), 'error');
    return;
  }

  pager_default_initialize($users->total, $pagelength);

  $items = culturefeed_ui_users_list($users);

  if (empty($items)) {
    $content['users'] = array(
      '#markup' => t('No results found.'),
    );
  }
  else {
    $content['users'] = array(
      '#theme' => 'item_list',
      '#items' => $items,
      '#attributes' => array('class' => 'user-list'),
    );

    $content['pager'] = array(
      '#theme' => 'pager',
    );
  }

  return $content;
}

function culturefeed_ui_page_users_most_active() {
  $query = new CultureFeed_SearchUsersQuery();
  $query->max = CULTUREFEED_UI_PAGE_USERS_MOST_ACTIVE_COUNT;
  $query->order = CultureFeed_SearchUsersQuery::SORT_ORDER_DESCENDING;
  $query->sort = variable_get('culturefeed_ui_block_most_active_user_sort', CultureFeed_SearchUsersQuery::SORT_NUMBEROFACTIVEACTIVITIES);

  try {
    $users = DrupalCultureFeed::searchUsers($query);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    drupal_set_message(t('Error occurred'), 'error');
    return;
  }

  $items = array();

  $accounts = culturefeed_get_uids_for_users($users->objects);

  foreach ($users->objects as $user) {
    if (!isset($accounts[$user->id])) {
      drupal_set_message(t('Error occurred'), 'error');
      return;
    }

    $items[] = theme('culturefeed_ui_ranked_user_summary', array('user' => $user, 'uid' => $accounts[$user->id]));
  }

  if (empty($items)) {
    $content['users'] = array(
      '#markup' => t('No users found'),
    );
  }
  else {
    $content['users'] = array(
      '#theme' => 'item_list',
      '#items' => $items,
      '#type' => 'ul',
      '#attributes' => array('class' => 'user-activity-list'),
    );
  }

  return $content;
}

function culturefeed_ui_page_mboxverify_resend() {
  try {
    $cf_account = DrupalCultureFeed::getLoggedInUser();
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    drupal_set_message(t('Error occurred, please try again later'), 'error');
  }

  if (!empty($cf_account) && !$cf_account->mboxVerified) {
    try {
      DrupalCultureFeed::resendMboxConfirmationForUser($cf_account->id);
      drupal_set_message(t('Confirmation mail succesfully sended'));
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_ui', $e);
      drupal_set_message(t('Error occurred, please try again later'), 'error');
    }
  }
  else {
    drupal_set_message(t('Email address of this account is already confirmed.'));
  }

  drupal_goto();
}

/**
 * Page callback the for profile privacy settings.
 *
 * @return array
 *   The render array.
 */
function culturefeed_ui_page_privacy_settings() {

  try {
    $culturefeed_user = DrupalCultureFeed::getLoggedInUser();
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    drupal_set_message(t('An error occurred while loading your account, please try again later.'));
    return array();
  }

  culturefeed_ui_set_profile_breadcrumb();

  $form = drupal_get_form('culturefeed_ui_privacy_settings_form', $culturefeed_user);

  return array(
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'culturefeed_ui') . '/css/privacy_settings.css',
      ),
    ),
    '#intro' => culturefeed_ui_profile_intro(),
    '#form' => drupal_render($form),
    '#profile_shortcuts' => theme('culturefeed_ui_profile_shortcuts'),
    '#theme' => 'culturefeed_ui_privacy_settings',
  );

}

/**
 * Page callback for the account edit page.
 *
 * @return array
 *   The render array.
 */
function culturefeed_ui_page_account_edit() {

  try {
    $culturefeed_user = DrupalCultureFeed::getLoggedInUser();
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    drupal_set_message(t('An error occurred while loading your account, please try again later.'));
    return array();
  }

  culturefeed_ui_set_profile_breadcrumb();

  $connected_applications = culturefeed_ui_page_serviceconsumers();
  $form = drupal_get_form('culturefeed_ui_account_edit_form', $culturefeed_user);
  $online_accounts = culturefeed_ui_online_accounts();

  return array(
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'culturefeed_ui') . '/css/account_edit.css',
      ),
    ),
    '#connected_applications' => drupal_render($connected_applications),
    '#form' => drupal_render($form),
    '#intro' => culturefeed_ui_profile_intro(),
    '#online_accounts' => drupal_render($online_accounts),
    '#profile_shortcuts' => theme('culturefeed_ui_profile_shortcuts'),
    '#theme' => 'culturefeed_ui_page_account_edit',
  );

}

/**
 * Page callback for the profile edit page.
 *
 * @return array
 *   The render array.
 */
function culturefeed_ui_page_profile_edit() {

  try {
    $culturefeed_user = DrupalCultureFeed::getLoggedInUser();
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    drupal_set_message(t('An error occurred while loading your account, please try again later.'));
    return array();
  }

  culturefeed_ui_set_profile_breadcrumb();

  $form = drupal_get_form('culturefeed_ui_profile_edit_form', $culturefeed_user);

  $build = array(
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'culturefeed_ui') . '/css/profile_edit.css',
      ),
    ),
    '#form' => drupal_render($form),
    '#intro' => culturefeed_ui_profile_intro(),
    '#profile_shortcuts' => theme('culturefeed_ui_profile_shortcuts'),
    '#theme' => 'culturefeed_ui_page_profile_edit',
  );

  if (module_exists('culturefeed_uitpas')) {

    $uitpas_user = culturefeed_uitpas_get_uitpas_user();
    if (isset($uitpas_user->passholder)) {

      $build['#attached']['js'] = array(
        drupal_get_path('module', 'culturefeed_ui') . '/js/synchronization.js',
        array(
          'type' => 'setting',
          'data' => array(
            'culturefeed_ui_synchronization' => array(
              'url' => url('culturefeed/uitpas/synchronization'),
              'title' => t('Update personal data'),
            ),
          ),
        ),
      );
      $build['#attached']['library'] = array(
        array('system', 'ui.dialog'),
      );

    }

  }

  return $build;

}


function culturefeed_ui_online_accounts() {
  $online_account_types = array('twitter', 'facebook', 'google');

  $items = array();

  foreach ($online_account_types as $i => $online_account_type) {
    try {
      $online_account = DrupalCultureFeed::getUserOnlineAccount($online_account_type);
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_ui', $e);
      drupal_set_message(t('Error occurred'), 'error');
      return;
    }

    $data = culturefeed_ui_online_account($online_account_type, $online_account);

    $items[] = array(
      'data' => $data,
      'id' => 'onlineaccount-' . $online_account_type,
    );
  }

  return array(
    '#theme' => 'item_list',
    '#items' => $items,
    '#attributes' => array(
    'class' => 'user-accounts',
    ),
  );
}

function culturefeed_ui_page_online_account_update_privacy($online_account_type, $id, $publish, $type = 'ajax') {
  $account = new CultureFeed_OnlineAccount();
  $account->publishActivities = $publish == '1' ? TRUE : FALSE;
  $account->accountName = $id;
  $account->accountType = $online_account_type;

  $success = TRUE;

  try {
    DrupalCultureFeed::updateUserOnlineAccount(DrupalCultureFeed::getLoggedInUserId(), $account);
    $online_account = DrupalCultureFeed::getUserOnlineAccount($online_account_type);
    $online_account->publishActivities = $account->publishActivities;
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    $success = FALSE;
  }

  if ($type == 'ajax') {
    $commands = array();

    if ($success) {
      $data = culturefeed_ui_online_account($online_account_type, $online_account);
      $commands[] = ajax_command_html('#onlineaccount-' . $online_account_type, $data);
    }

    ajax_deliver(array('#type' => 'ajax', '#commands' => $commands));
  }
  else {
    if (!$success) {
      drupal_set_message(t('Error occurred'), 'error');
    }
    drupal_goto();
  }
}


function culturefeed_ui_page_online_account_toggle_facebook_privacy($online_account_type, $id, $publish, $type = 'ajax') {
  $account = new CultureFeed_OnlineAccount();
  $account->publishActivities = $publish == '1' ? TRUE : FALSE;
  $account->accountName = $id;
  $account->accountType = $online_account_type;

  $success = TRUE;

  try {
    DrupalCultureFeed::updateUserOnlineAccount(DrupalCultureFeed::getLoggedInUserId(), $account);
    $online_account = DrupalCultureFeed::getUserOnlineAccount($online_account_type);
    $online_account->publishActivities = $account->publishActivities;
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    $success = FALSE;
  }

  if ($type == 'ajax') {
    $commands = array();

    if ($success) {
      $data = _culturefeed_ui_facebook_privacy_toggle();
      $commands[] = ajax_command_html('#onlineaccount-' . $online_account_type, $data);
    }

    ajax_deliver(array('#type' => 'ajax', '#commands' => $commands));
  }
  else {
    if (!$success) {
      drupal_set_message(t('Error occurred'), 'error');
    }
    drupal_goto();
  }
}

function culturefeed_ui_page_online_account_delete($online_account_type, $id, $type = 'ajax') {
  $cf_uid = DrupalCultureFeed::getLoggedInUserId();

  $success = TRUE;

  try {
    DrupalCultureFeed::deleteUserOnlineAccount($cf_uid, $online_account_type, $id);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    $success = FALSE;
  }

  if ($type == 'ajax') {
    $commands = array();

    if ($success) {
      $data = culturefeed_ui_online_account($online_account_type);
      $commands[] = ajax_command_html('#onlineaccount-' . $online_account_type, $data);
    }

    ajax_deliver(array('#type' => 'ajax', '#commands' => $commands));
  }
  else {
    if (!$success) {
      drupal_set_message(t('Error occurred'), 'error');
    }
    drupal_goto();
  }
}

function culturefeed_ui_page_serviceconsumers() {
  try {
    $consumers = DrupalCultureFeed::getUserServiceConsumers(DrupalCultureFeed::getLoggedInUserId());
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    drupal_set_message(t('Error occurred'), 'error');
    return;
  }

  $items = array();

  foreach ($consumers as $consumer) {
    $revoke_link = '';

    if ($consumer->consumerKey != variable_get('culturefeed_api_application_key', '')) {
      $revoke_link = array(
        '#type' => 'link',
        '#title' => t('Delete'),
        '#href' => 'culturefeed/serviceconsumers/revoke/' . $consumer->id . '/nojs',
        '#attributes' => array('class' => 'revoke-link'),
        '#options' => array('query' => drupal_get_destination()),
        '#ajax' => array(),
      );

      $revoke_link = drupal_render($revoke_link);
    }

    $items[] = array(
      'data' => theme('culturefeed_ui_service_consumer_summary', array('consumer' => $consumer, 'revoke_link' => $revoke_link)),
      'id' => 'consumer-' . $consumer->id,
    );
  }

  if (empty($items)) {
    $content['consumers'] = array(
      '#markup' => t('No channels found.'),
    );
  }
  else {
    $content['consumers'] = array(
      '#theme' => 'item_list',
      '#items' => $items,
      '#attributes' => array(
        'class' => 'user-applications',
      ),
    );
  }

  return $content;
}

function culturefeed_ui_page_serviceconsumer_revoke($consumer_id, $type = 'ajax') {
  $cf_uid = DrupalCultureFeed::getLoggedInUserId();

  $success = TRUE;

  try {
    DrupalCultureFeed::revokeUserServiceConsumer($cf_uid, $consumer_id);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    $success= FALSE;
  }

  if ($type == 'ajax') {
    $commands = array();

    if ($success) {
      $commands[] = ajax_command_invoke('#consumer-' . $consumer_id, 'fadeOut', array('slow'));
    }

    ajax_deliver(array('#type' => 'ajax', '#commands' => $commands));
  }
  else {
    if (!$success) {
      drupal_set_message(t('Error occurred'), 'error');
    }
    drupal_goto();
  }
}
