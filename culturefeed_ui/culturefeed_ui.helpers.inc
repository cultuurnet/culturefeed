<?php

/**
 * Returns the user display name based on privacy settings.
 *
 * @param CultureFeed_User $culturefeed_user
 *   The CultureFeed user.
 *
 * @return string
 *   The display name.
 */
function culturefeed_ui_get_display_name(CultureFeed_User $culturefeed_user) {

  // Build the display name based on available fields
  // (set through privacy settings).
  $display = array();

  if ($culturefeed_user->givenName) {

    // Name.
    $name = $culturefeed_user->givenName;
    if ($culturefeed_user->familyName) {
      $name .= ' ' . $culturefeed_user->familyName;
    }
    $display[] = $name;

    // Age.
    module_load_include('inc', 'culturefeed_ui', 'includes/privacy_settings');
    $age = culturefeed_ui_privacy_settings_get_age($culturefeed_user);
    if ($age) {
      $display[] = $age;
    }

    // City.
    if ($culturefeed_user->city) {
      $display[] = $culturefeed_user->city;
    }

    return implode(', ', $display);

  }

  else {
    return $culturefeed_user->nick;
  }

}

/**
 * Returns the different types of preferences.
 *
 * @return array
 *   The preference types.
 */
function culturefeed_ui_get_preference_types() {

  $reflection = new ReflectionClass('CultureFeed_Activity');
  $constants = $reflection->getConstants();
  $types = array();
  foreach ($constants as $key => $value) {
    if (strpos($key, 'TYPE_') === 0) {
      $types[$key] = $value;
    }
  }
  return $types;

}

/**
 * Returns the user display name setting based on privacy settings.
 *
 * @param CultureFeed_User $culturefeed_user
 *   The CultureFeed user.
 *
 * @return string
 *   The display name.
 */
function culturefeed_ui_get_privacy_setting(CultureFeed_User $culturefeed_user = NULL) {

  if (!isset($culturefeed_user)) {
    $culturefeed_user = DrupalCultureFeed::getLoggedInUser();
  }

  module_load_include('inc', 'culturefeed_ui', 'includes/privacy_settings');

  return culturefeed_ui_privacy_settings_get_current_setting($culturefeed_user);

}

/**
 * Returns the intro message for profile, account and privacy settings pages.
 *
 * @return string
 *   The profile intro message.
 */
function culturefeed_ui_profile_intro() {

  $culturefeed_user = DrupalCultureFeed::getLoggedInUser();

  $profile_intro = t('Update your profile information here. All your user activities can be managed in your <a href="@history-url">personal history</a>.', array('@history-url' => url('culturefeed/activities')));

  return $profile_intro;

}

/**
 * Returns the drupal message after making profile changes.
 *
 * @param CultureFeed_User $culturefeed_user
 *   The CultureFeed user.
 *
 * @return string
 *   The message.
 */
function culturefeed_ui_profile_edit_message(CultureFeed_User $culturefeed_user) {

  $user_id = culturefeed_get_uid_for_cf_uid($culturefeed_user->id, $culturefeed_user->nick);

  $message = t('Changes saved successfully.') . ' ';
  $message .= t(
    "<a href=\"@profile\">View your profile</a> as shown to others.",
    array('@profile' => url('user/' . $user_id))
  );

  return $message;

}

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

/**
 * Returns the online account links.
 *
 * @param string $online_account_type
 *   The online account type.
 * @param CultureFeed_OnlineAccount $online_account
 *   The online account.
 *
 * @return string
 *   The links.
 */
function culturefeed_ui_online_account($online_account_type, $online_account = NULL) {
  $delete_link = '';

  if ($online_account) {
    $delete_link = array(
      '#type' => 'link',
      '#title' => t('Remove connection'),
      '#href' => 'culturefeed/onlineaccounts/delete/' . $online_account_type . '/' . $online_account->accountName . '/nojs',
      '#attributes' => array('class' => 'delete-link'),
      '#options' => array('query' => drupal_get_destination(), 'html' => TRUE),
      '#ajax' => array(),
    );

    $delete_link = drupal_render($delete_link);
  }

  $publish_form = '';
  if ($online_account) {
    $publish_form = drupal_get_form('culturefeed_ui_online_account_publish_activities_' . $online_account_type . '_form', $online_account_type, $online_account);
    $publish_form = drupal_render($publish_form);
  }

  $redirect = url('culturefeed/account/edit', array('absolute' => TRUE));
  $connect_link = l(t('Connect'), DrupalCultureFeed::getUrlAddSocialNetwork($online_account_type, $redirect), array('attributes' => array('class' => 'culturefeedconnect')));

  $vars = array(
    'account_type'     => $online_account_type,
    'account'          => $online_account,
    'publish_form' => $publish_form,
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

/**
 * Sets all the activity privacy preferences.
 *
 * @param CultureFeed_User $culturefeed_user
 *   The culturefeed user.
 *
 * @param boolean $private
 *   The privacy setting.
 */
function culturefeed_ui_set_all_activity_privacy_preferences(CultureFeed_User $culturefeed_user, $private) {

  $types = culturefeed_ui_get_preference_types();

  $preferences = new CultureFeed_Preferences();
  $preferences->uid = $culturefeed_user->id;
  foreach ($types as $type => $id) {
    $preference = new CultureFeed_ActivityPrivacyPreference($id, $private);
    $preferences->activityPrivacyPreferences[$id] = $preference;
  }
  DrupalCultureFeed::setUserPreferences($culturefeed_user->id, $preferences);

}


function culturefeed_ui_set_cookies_message() {
  if (!path_is_admin(current_path()) && variable_get('culturefeed_ui_cookie_bool') == TRUE) {
    $message = variable_get('site_name');
    $message .= ' ' . t('is using cookies') . '. ' . '<a href="@url">' . t('Learn more') . '</a>. ';
    $message_vars = array("@url" => url(variable_get('culturefeed_ui_cookie_path')));
    drupal_set_message(t($message, $message_vars), 'warning', FALSE);
  }
}

/**
 * Applies the privacy settings on the user view.
 *
 * @param CultureFeed_User $culturefeed_user
 *   The culturefeed user.
 * @param object $account
 *   The drupal user.
 */
function culturefeed_ui_user_view_apply_privacy_settings(CultureFeed_User $culturefeed_user, &$account) {

  if ($culturefeed_user->nick) {
    $title = $culturefeed_user->nick;
  }
  else {

    $title = t('Anonymous');
    $account->content['anonymous'] = array(
      '#markup' => '<p class="private-profile">' . t('This profile is private.') . '</p>',
    );

    culturefeed_set_noindex_metatag();

  }

  drupal_set_title($title);

}
