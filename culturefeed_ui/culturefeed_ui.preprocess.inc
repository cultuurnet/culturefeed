<?php

/**
 * Preprocess variables for culturefeed-ui-profile-edit-form.
 *
 * @param array $vars
 *   The variables.
 */
function culturefeed_ui_preprocess_culturefeed_ui_profile_edit_form(&$vars) {

  $form = $vars['form'];
  $vars['rendered_form'] = drupal_render_children($form);

}

/**
 * Preprocess variables for culturefeed-ui-account-edit-form.
 *
 * @param array $vars
 *   The variables.
 */
function culturefeed_ui_preprocess_culturefeed_ui_account_edit_form(&$vars) {

  $form = $vars['form'];
  $vars['rendered_form'] = drupal_render_children($form);

}

/**
 * Preprocess variables for culturefeed-ui-profile-sections.
 *
 * @param array $vars
 *   The variables.
 */
function culturefeed_ui_preprocess_culturefeed_ui_profile_section(&$vars) {

  if (isset($vars['classes'])) {
    $vars['classes_array'] = array_merge($vars['classes_array'], $vars['classes']);
  }

}

/**
 * Preprocess variables for culturefeed-ui-profile.tpl.php.
 *
 * @see culturefeed-ui-profile.tpl.php
 */
function template_preprocess_culturefeed_ui_profile(&$variables) {

  $account = $variables['user'];

  // Title.
  $variables['title'] = t('My UiTiD');

  // nick
  $variables['nick'] = check_plain($account->nick);

  // name
  $name = array();

  if (!empty($account->givenName)) {
    $name[] = $account->givenName;
  }
  if (!empty($account->familyName)) {
    $name[] = $account->familyName;
  }

  $variables['name'] = !empty($name) ? implode(' ', $name) : '';

  // gender
  $variables['gender'] = '';
  if (!empty($account->gender)) {
    $variables['gender'] = $account->gender == CultureFeed_User::GENDER_MALE ? t('Male') : t('Female');
  }

  // dob
  $variables['dob'] = !empty($account->dob) ? format_date($account->dob, 'custom', 'j/n/Y') : '';

  // age
  $variables['age'] = '';
  if (!empty($account->dob)) {
    $time_zone  = new DateTimeZone('Europe/Brussels');
    $variables['age'] = DateTime::createFromFormat('d/m/Y', $variables['dob'], $time_zone)
      ->diff(new DateTime('now', $time_zone))
      ->y;
  }

  // bio
  $variables['bio'] = !empty($account->bio) ? check_plain($account->bio) : '';

  // picture
  $variables['picture'] = !empty($account->depiction) ? theme('image', array('path' => $account->depiction . '?maxwidth=100')) : '';

  // city
  $variables['city'] = !empty($account->city) ? check_plain($account->city) : '';

  // profile
  $variables['profile_edit_link'] = '';
  if (DrupalCultureFeed::getLoggedInUserId() == $account->id) {
    $variables['profile_edit_link'] = l(t('Edit'), 'culturefeed/profile/edit', array('attributes' => array('class' => array('profile-edit-link'))));
  }

  $variables['memberships'] = $account->pageMemberships;

  $variables['following'] = array();
  if (!empty($account->following)) {
    foreach ($account->following as $following) {
      $variables['following'][] = culturefeed_search_detail_l('page', $following->page->getId(), $following->page->getName());
    }
  }

}

/**
 * Preprocess variables for culturefeed-ui-mbox-confirmation-reminder.tpl.php.
 *
 * @see culturefeed-ui-mbox-confirmation-reminder.tpl.php
 */
function template_preprocess_culturefeed_ui_mbox_confirmation_reminder(&$variables) {
  $variables['mbox'] = check_plain($variables['account']->mbox);

  $variables['resend_link'] = l(t('Resend email confirmation'), 'culturefeed/mboxverify/resend', array('query' => drupal_get_destination()));
  $variables['update_link'] = l(t('Change Email address'), 'culturefeed/account/edit');
  $variables['info_link'] = l(t('More info about UiTiD'), 'http://www.uitinvlaanderen.be/over-uitid');
}

/**
 * Preprocess variables for culturefeed-ui-profile-menu-item.tpl.php.
 *
 * @see culturefeed-ui-profile-menu-item.tpl.php
 */
function template_preprocess_culturefeed_ui_profile_menu_item(&$variables) {
  $variables['link'] = l($variables['title'], $variables['url']);
}

/**
 * Preprocess variables for culturefeed-ui-online-account.tpl.php.
 *
 * @see culturefeed-ui-online-account.tpl.php
 */
function template_preprocess_culturefeed_ui_online_account(&$variables) {
  $account = $variables['account'];

  $variables['type'] = $variables['account_type'];

  $variables['name'] = !empty($account->accountName) ? check_plain($account->accountName) : '';
  $variables['nick'] = !empty($account->accountNick) ? check_plain($account->accountNick) : '';
  $variables['depiction'] = !empty($account->accountDepiction) ? check_plain($account->accountDepiction) : '';

  $variables['picture'] = !empty($account->accountDepiction) ? theme('image', array('path' => $account->accountDepiction . '?maxwidth=50&maxheight=50&crop=auto')) : '';
}

/**
 * Preprocess variables for culturefeed-ui-ranked-user-summary.tpl.php.
 *
 * @see culturefeed-ui-ranked-user-summary.tpl.php
 */
function template_preprocess_culturefeed_ui_ranked_user_summary(&$variables) {
  $user = $variables['user'];
  $user instanceof CultureFeed_User;

  template_preprocess_culturefeed_ui_user_summary($variables);
  $variables['sortValue'] = $user->sortValue;
}

/**
 * Preprocess variables for culturefeed-ui-ranked-user-summary.tpl.php.
 *
 * @see culturefeed-ui-user-summary.tpl.php
 */
function template_preprocess_culturefeed_ui_user_summary(&$variables) {
  $user = $variables['user'];
  $user instanceof CultureFeed_User;

  $picture = theme('image', array('path' => $user->depiction . '?maxwidth=50&maxheight=50&crop=auto', 'alt' => $user->nick));
  $url = 'user/' . $variables['uid'];
  $variables['picture'] = l($picture, $url, array('html' => TRUE, 'attributes' => array('title' => $user->nick)));
  $variables['nick'] = l($user->nick, $url);
}

/**
 * Preprocess variables for culturefeed-ui-anonymous-profile-box.tpl.php.
 *
 * @see culturefeed-ui-anonymous-profile-box.tpl.php
 */
function template_preprocess_culturefeed_ui_anonymous_profile_box(&$variables) {

  $variables['link_login'] = l('<span>'.t('Log in').'</span>', 'culturefeed/oauth/connect', array('html' => TRUE,'attributes' => array('class' => array('culturefeedconnect'), 'rel' => 'nofollow'), 'query' => drupal_get_destination()));
  $variables['link_register'] = l(t('Sign up'), 'culturefeed/oauth/connect/register', array('attributes' => array('class' => array('culturefeedconnect'), 'rel' => 'nofollow'), 'query' => drupal_get_destination()));
  $query = drupal_get_destination();
  $query['via'] = 'facebook';
  $variables['link_login_facebook'] = l('Facebook', 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect connect-facebook'), 'rel' => 'nofollow'), 'query' => $query));
  uasort($variables['main_items'], 'drupal_sort_weight');
}

/**
 * Preprocess variables for culturefeed-ui-profile-box.tpl.php.
 *
 * @see culturefeed-ui-profile-box.tpl.php
 */
function template_preprocess_culturefeed_ui_profile_box(&$variables) {

  $picture = $variables['account']->depiction . '?maxwidth=20&maxheight=20&crop=auto';
  $variables['picture'] = theme('image', array('path' => $picture));

  $variables['nick'] = $variables['profile_name']['nick'];
  $variables['link'] = $variables['profile_name']['link'];

  uasort($variables['main_items'], 'drupal_sort_weight');

  uasort($variables['dropdown_items'], 'drupal_sort_weight');
  foreach ($variables['dropdown_items'] as &$item) {
    if (isset($dropdown_items['children'])) {
      uasort($item, 'drupal_sort_weight');
    }
  }

  $variables['logout'] = l(t('Log out'), 'user/logout');

}

/**
 * Preprocess variables for culturefeed-ui-most-active-user.tpl.php.
 *
 * @see culturefeed-ui-most-active-user.tpl.php
 */
function template_preprocess_culturefeed_ui_most_active_user(&$variables) {
  $account = $variables['account'];
  $account instanceof CultureFeed_User;

  $url = 'user/' . culturefeed_get_uid_for_cf_uid($account->id, $account->nick);
  $picture = theme('image', array('path' => $account->depiction . '?maxwidth=93&maxheight=93&crop=auto'));

  $variables['picture'] = l($picture, $url, array('html' => TRUE));

  switch ($variables['sort']) {
    case CultureFeed_SearchUsersQuery::SORT_NUMBEROFACTIVITIES:
      $type_text = t('actions</strong> done');
      break;
    case CultureFeed_SearchUsersQuery::SORT_NUMBEROFLIKES:
      $type_text = t('pages</strong> liked');
      break;
    case CultureFeed_SearchUsersQuery::SORT_NUMBEROFSHARESFACEBOOK:
      $type_text = t('pagess</strong> shared on Facebook');
      break;
    case CultureFeed_SearchUsersQuery::SORT_NUMBEROFSHARESTWITTER:
      $type_text = t('pages</strong> shared on Twitter');
      break;
    case CultureFeed_SearchUsersQuery::SORT_NUMBEROFATTENDS:
      $type_text = t('events</strong> to attend');
      break;
    case CultureFeed_SearchUsersQuery::SORT_NUMBEROFACTIVEACTIVITIES:
      $type_text = t('actions</strong> done');
      break;
  }

  $replace = array(
    '!nick' => l($account->nick, $url),
    '!sort_value' => $account->sortValue,
    '!type' => $type_text,
    '!uitslover' => l(t('Most active user'), 'culturefeed/users/mostactive'),
  );

  $variables['text'] = strtr(t('!nick did <strong>!sort_value !type and is the !uitslover of the week. Congratulations!'), $replace);
}

/**
 * Preprocess variables for culturefeed-ui-related-events.tpl.php.
 *
 * @see culturefeed-ui-related-events.tpl.php
 */
function template_preprocess_culturefeed_ui_service_consumer_summary(&$variables) {
  $consumer = $variables['consumer'];
  $consumer instanceof CultureFeed_Consumer;

  $variables['logo'] = isset($consumer->logo) ? theme('image' , array('path' => $consumer->logo . '?maxwidth=100&maxheight=100')) : '';
  $variables['name'] = isset($consumer->name) ? check_plain($consumer->name) : '';
  $variables['description'] = isset($consumer->description) ? check_plain($consumer->description) : '';
  $variables['creation_date'] = t('Connected to') . ' ' . format_date($consumer->creationDate, 'custom', 'j F Y');
  $variables['domain'] = isset($consumer->domain) ? check_plain($consumer->domain) : '';
}

/**
 * Preprocess variables for culturefeed-ui-my-uitid.tpl.php.
 */
function template_preprocess_culturefeed_ui_my_uitid(&$variables) {
  $account = $variables['account'];

  // contextual links
  $contextual_links = array();

  $contextual_links[] = l(t('Edit profile'), 'culturefeed/profile/edit');

  if (module_exists('culturefeed_social')) {
    $contextual_links[] = l(t('User history'), 'culturefeed/activities');
  }

  if (module_exists('culturefeed_messages')) {
    $contextual_links[] = l(t('My messages'), 'culturefeed/messages');
  }

  if (module_exists('culturefeed_pages')) {
    $contextual_links[] = l(t('My pages'), 'culturefeed/pages');
  }

  $contextual_links[] = l(t('Edit account'), 'culturefeed/account/edit');

  $item_list = array(
    '#theme' => 'item_list',
    '#items' => $contextual_links,
  );
  $variables['contextual_links_array'] = $contextual_links;
  $variables['contextual_links'] = drupal_render($item_list);

  // nick
  $drupal_uid = culturefeed_get_uid_for_cf_uid($variables['account']->id, $variables['account']->nick);
  $url_profile = 'user/' . $drupal_uid;
  $variables['nick'] = l($account->nick, $url_profile);

  $variables['view_own_page'] = FALSE;
  if ($GLOBALS['user']->uid == $drupal_uid) {
    $variables['view_own_page'] = TRUE;
  }

  // name
  $name = array();

  if (!empty($account->givenName)) {
    $name[] = $account->givenName;
  }
  if (!empty($account->familyName)) {
    $name[] = $account->familyName;
  }

  $variables['name'] = !empty($name) ? l(implode(' ', $name), $url_profile) : '';

  // gender
  $variables['gender'] = '';
  if (!empty($account->gender)) {
    $variables['gender'] = $account->gender == CultureFeed_User::GENDER_MALE ? 'Man' : 'Vrouw';
  }

  // dob
  if (!empty($account->dob)) {
    $year = date('Y', $account->dob);
    $age = (date("md", date("U", $account->dob)) > date("md") ? ((date("Y") - $year) - 1) : (date("Y") - $year));
    $variables['age'] = $age;
  }
  else {
    $variables['age'] = '';
  }

  // picture
  $variables['picture'] = !empty($account->depiction) ? theme('image', array('path' => $account->depiction . '?width=50&height=50&crop=auto')) : '';

  // city
  $variables['city'] = !empty($account->city) ? check_plain($account->city) : '';

  // profile
  if ($account->lifestyleProfile) {

    // lifestyleProfile name
    switch ($account->lifestyleProfile) {
      case "AZ":
        $variables['profile_name'] = t('Action searcher');
          break;
      case "AO":
        $variables['profile_name'] = t('Leisure searcher');
          break;
      case "ONT":
        $variables['profile_name'] = t('Discoverer');
          break;
      case "FP":
        $variables['profile_name'] = t('Connoisseur');
          break;
    }
  }

  // facebook toggle
  $variables['facebook_privacy_toggle'] = NULL;
  $facebook = _culturefeed_ui_facebook_privacy_toggle();
  if ($facebook) {
    $item_list = array(
      '#theme' => 'item_list',
      '#items' => array(
        array(
          'data' => $facebook,
          'id' => 'onlineaccount-facebook',
        ),
      ),
    );
    $facebook_privacy_toggle = array(
      '#markup' => drupal_render($item_list),
    );
    $variables['facebook_privacy_toggle'] = drupal_render($facebook_privacy_toggle);
  }

  if (module_exists('culturefeed_pages')) {

    $memberships = $account->pageMemberships;
    if (!empty($memberships)) {
      $variables['memberships'] = array();
      foreach ($memberships as $membership) {
        $variables['memberships'][] = culturefeed_search_detail_l('page', $membership->page->getId(), $membership->page->getName());
      }
    }

    $following = $account->following;
    if (!empty($following)) {
      $variables['following'] = array();
      foreach ($following as $following_page) {
        $variables['following'][] = culturefeed_search_detail_l('page', $following_page->page->getId(), $following_page->page->getName());
      }
    }

  }

}

/**
 * Preprocess variables for culturefeed-ui-connect.tpl.php.
 *
 * @see culturefeed-ui-connect.tpl.php
 */
function template_preprocess_culturefeed_ui_connect(&$variables) {

  $query['destination'] = isset($variables['url']) ? $variables['url'] : $_GET['q'];
  $query['via'] = 'facebook';
  $variables['link_facebook'] = l('Facebook', 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect connect-facebook'), 'rel' => 'nofollow'), 'query' => $query));

  $query['via'] = 'twitter';
  $variables['link_twitter'] = l('Twitter', 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect connect-twitter'), 'rel' => 'nofollow'), 'query' => $query));

  $query['via'] = 'google';
  $variables['link_google'] = l('Google', 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect connect-google'), 'rel' => 'nofollow'), 'query' => $query));

  unset($query['via']);
  $variables['link_email'] = l(t('your email'), 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect connect-email'), 'rel' => 'nofollow'), 'query' => $query));

}

/**
 * Preprocess variables for culturefeed-ui-connect-hover.tpl.php.
 *
 * @see culturefeed-ui-connect-hover.tpl.php
 */
function template_preprocess_culturefeed_ui_connect_hover(&$variables) {
  template_preprocess_culturefeed_ui_connect($variables);
}

/**
 * Preprocess variables for culturefeed-ui-something-for-you.tpl.php.
 *
 * @see culturefeed-ui-something-for-you.tpl.php
 */
function template_preprocess_culturefeed_ui_something_for_you(&$variables) {
  template_preprocess_culturefeed_ui_connect($variables);
}

/**
 * Preprocess variables for culturefeed_ui_profile_menu
 */
function theme_culturefeed_ui_profile_shortcuts() {

  $build = array(
    '#attributes' => array(
      'class' => array('culturefeed-profile-shortcuts'),
    ),
    '#theme' => 'links',
  );

  $build['#links'] = array(
    'personal_data' => array(
      'title' => t('Personal data'),
      'href' => 'culturefeed/profile/edit',
    ),
    'login_data' => array(
      'title' => t('Login data'),
      'href' => 'culturefeed/account/edit',
    ),
    'privacy_settings' => array(
      'title' => t('Privacy settings'),
      'href' => 'culturefeed/profile/privacy',
    ),
  );

  return drupal_render($build);

}
