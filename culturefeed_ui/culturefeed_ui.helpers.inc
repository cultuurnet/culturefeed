<?php

/**
 * Put noindex and canonical metatags for SEO on relevant culturefeed pages.
 */
function culturefeed_ui_set_canonical_and_noindex() {
  $canonical = FALSE;
  $noindex = FALSE;
  $nofollow = FALSE;

  if (arg(0) == 'culturefeed') {
    if (arg(1) == 'activities' && arg(2) == 'recent') {
      $noindex = TRUE;
    }
    if (arg(1) == 'users' && arg(2) == 'search') {
      $noindex = TRUE;
    }
  }

  if (arg(0) == 'review') {
    $noindex = TRUE;
    $nofollow = TRUE;
  }

  // Add the "noindex, follow" meta tag if needed.
  if ($noindex) {
    $element = array(
      '#tag' => 'meta',
      '#attributes' => array(
        'name' => 'robots',
        'content' => 'noindex, ' . ($nofollow ? 'nofollow' : 'follow'),
      ),
    );

    drupal_add_html_head($element, 'robots');
  }

  // Add the rel="canonical" tag if needed.
  if ($canonical) {
    $canonical['options']['absolute'] = TRUE;
    drupal_add_html_head_link(array('rel' => 'canonical', 'href' => url($canonical['path'], $canonical['options'])), TRUE);
  }
}

function culturefeed_ui_is_culturefeed_profile_page() {
  $account = menu_get_object('user');

  if (arg(0) == 'user' && is_numeric(arg(1)) && !arg(2) && DrupalCultureFeed::isCultureFeedUser($account->uid)) {
    return TRUE;
  }

  return FALSE;
}


function culturefeed_ui_online_account($online_account_type, $online_account = NULL) {
  $delete_link = '';

  if ($online_account) {
    $delete_link = array(
      '#type' => 'link',
      '#title' => t('Delete'),
      '#href' => 'culturefeed/onlineaccounts/delete/' . $online_account_type . '/' . $online_account->accountName . '/nojs',
      '#attributes' => array('class' => 'delete-link'),
      '#options' => array('query' => drupal_get_destination()),
      '#ajax' => array(),
    );

    $delete_link = drupal_render($delete_link);
  }

  $statement = t('I accept that my UiTiD actions on ') . check_plain(variable_get('site_name', 'Drupal')) . t(' will be published automatically on ') . $online_account_type . '. ' . l(t('User agreement'), 'node/2512');

  $publish_link = '';
  if ($online_account) {
    $publish_link = array(
      '#type' => 'link',
      '#title' => $online_account->publishActivities ? t('Public') : t('Private'),
      '#href' => 'culturefeed/onlineaccounts/privacy/' . $online_account_type . '/' . $online_account->accountName . '/' . ($online_account->publishActivities ? '0' : '1') . '/nojs',
      '#attributes' => array('id' => 'onlineaccount-privacy-' . $online_account->accountName, 'class' => 'privacy-link ' . ($online_account->publishActivities ? 'status-publiek' : 'status-prive'), 'title' => ($online_account->publishActivities ? t('Switch off') : t('Switch on'))),
      '#options' => array('query' => drupal_get_destination()),
      '#ajax' => array(),
    );

    $publish_link = drupal_render($publish_link) . $statement;
  }

  $redirect = url('culturefeed/account/edit', array('absolute' => TRUE));
  $connect_link = l(t('Add account'), DrupalCultureFeed::getUrlAddSocialNetwork($online_account_type, $redirect), array('attributes' => array('class' => 'culturefeedconnect')));

  $vars = array(
    'account_type'     => $online_account_type,
    'account'          => $online_account,
    'publish_link'     => $publish_link,
    'delete_link'      => $delete_link,
    'connect_link'     => $connect_link,
  );

  return theme('culturefeed_ui_online_account', $vars);
}

function culturefeed_ui_activity_types() {
  return array(
    CultureFeed_Activity::TYPE_LIKE     => t('Like'),
    CultureFeed_Activity::TYPE_IK_GA    => t('Attend'),
    CultureFeed_Activity::TYPE_FACEBOOK => t('Share on Facebook'),
    CultureFeed_Activity::TYPE_TWITTER  => t('Share on Twitter'),
  );
}

function culturefeed_ui_users_list($users) {
  return _culturefeed_ui_users_list($users->objects);
}

function _culturefeed_ui_users_list($user_objects) {
  $items = array();

  $accounts = culturefeed_get_uids_for_users($user_objects);

  foreach ($user_objects as $user) {
    if (!isset($accounts[$user->id])) {
      continue;
    }

    $items[] = theme('culturefeed_ui_user_summary', array('user' => $user, 'uid' => $accounts[$user->id]));
  }

  return $items;
}


/**
 * Helper function to build facebook privacy toggle
 */
function _culturefeed_ui_facebook_privacy_toggle() {
  try {
    $online_account = DrupalCultureFeed::getUserOnlineAccount('facebook');
    if ($online_account) {
      $current_status = array(
        '#type' => 'markup',
        '#markup' => '<span class="current-status '  . ($online_account->publishActivities ? 'status-publiek' : 'status-prive') . '"><strong> ' . ($online_account->publishActivities ? t('ON') : t('OFF')) . ' </strong></span>',
      );
      $publish_link = array(
        '#type' => 'link',
        '#title' => ($online_account->publishActivities ? t('Switch off') : t('Switch on')) . ' ',
        '#href' => 'culturefeed/toggle/privacy/facebook/' . $online_account->accountName . '/' . ($online_account->publishActivities ? '0' : '1') . '/nojs',
        '#attributes' => array('id' => 'onlineaccount-privacy-' . $online_account->accountName, 'class' => 'privacy-link ' . ($online_account->publishActivities ? 'status-publiek' : 'status-prive'), 'title' => ($online_account->publishActivities ? t('Switch off') : t('Switch on'))),
        '#options' => array('query' => drupal_get_destination()),
        '#ajax' => array(),
      );

      $publish_link = t('Post my actions to Facebook') . ' | ' . drupal_render($current_status) . ' | ' . drupal_render($publish_link);

      return theme('culturefeed_ui_online_account', array('publish_link' => $publish_link));
    }
    else {
      return NULL;
    }
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    drupal_set_message(t('Error occurred'), 'error');
    return;
  }
}

/**
 * Set the breadcrumb to the profile.
 */
function culturefeed_ui_set_profile_breadcrumb() {

  global $user;

  // Set the breadcrumb.
  $active_trail = array();

  $active_trail[] = array(
    'title' => t('Home'),
    'href' => '<front>',
    'link_path' => '',
    'localized_options' => array(),
    'type' => 0,
  );

  $active_trail[] = array(
    'title' => t('My profile'),
    'href' => 'user/' . $user->uid,
    'link_path' => '',
    'localized_options' => array(),
    'type' => 0,
  );

  $active_trail[] = array(
    'title' => $user->name,
    'href' => $_GET['q'],
    'link_path' => '',
    'localized_options' => array(),
    'type' => 0,
  );

  menu_set_active_trail($active_trail);

}

