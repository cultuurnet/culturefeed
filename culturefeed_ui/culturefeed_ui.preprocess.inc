<?php

/**
 * Preprocess variables for culturefeed-ui-profile.tpl.php.
 *
 * @see culturefeed-ui-profile.tpl.php
 */
function template_preprocess_culturefeed_ui_profile(&$variables) {
  $user = $variables['user'];
  $user instanceof CultureFeed_User;
  
  // nick
  $variables['nick'] = $user->nick;
  
  // name
  $name = array();

  if (!empty($user->givenName)) {
    $name[] = $user->givenName;
  }
  if (!empty($user->familyName)) {
    $name[] = $user->familyName;
  }

  $variables['name'] = !empty($name) ? implode(' ', $name) : '';
  
  // gender
  $variables['gender'] = '';
  if (!empty($user->gender)) {
    $variables['gender'] = $user->gender == CultureFeed_User::GENDER_MALE ? 'Man' : 'Vrouw';
  }
  
  // dob
  $variables['dob'] = !empty($user->dob) ? format_date($user->dob, 'custom', 'j/n/Y') : '';
  
  // bio
  $variables['bio'] = !empty($user->bio) ? $user->bio : '';
}

/**
 * Preprocess variables for culturefeed-ui-connect.tpl.php.
 *
 * @see culturefeed-ui-connect.tpl.php
 */
function template_preprocess_culturefeed_ui_connect(&$variables) {
  $variables['link_facebook'] = l('Facebook', 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect')), 'query' => drupal_get_destination()));
  $variables['link_twitter'] = l('Twitter', 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect')), 'query' => drupal_get_destination()));
  $variables['link_google'] = l('Google', 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect')), 'query' => drupal_get_destination()));
}

/**
 * Preprocess variables for culturefeed-ui-mbox-confirmation-reminder.tpl.php.
 *
 * @see culturefeed-ui-mbox-confirmation-reminder.tpl.php
 */
function template_preprocess_culturefeed_ui_mbox_confirmation_reminder(&$variables) {
  $variables['mbox'] = $variables['account']->mbox;
  
  $variables['resend_link'] = l('Bevestiging opnieuw versturen', 'culturefeed/mboxverify/resend', array('query' => drupal_get_destination()));
  $variables['update_link'] = l('E-mail adres aanpassen', 'culturefeed/account/edit');
  $variables['info_link'] = l('Meer info over UiTid', '<front>');
}

/**
 * Preprocess variables for culturefeed-ui-activity-summary.tpl.php.
 *
 * @see culturefeed-ui-activity-summary.tpl.php
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
  $variables['picture'] = !empty($variables['depiction']) ? theme('image', array('path' => $variables['depiction'] . '?maxwidth=50&maxheight=50&crop=auto')) : '';
}

/**
 * Preprocess variables for culturefeed-ui-activity-summary.tpl.php.
 *
 * @see culturefeed-ui-activity-summary.tpl.php
 */
function template_preprocess_culturefeed_ui_activity_summary(&$variables) {
  $activity = $variables['activity'];
  $activity instanceof CultureFeed_Activity;

  $picture = theme('image', array('path' => $activity->depiction . '?maxwidth=50&maxheight=50&crop=auto'));
  $url = 'user/' . $variables['uid'];

  $variables['picture'] = l($picture, $url, array('html' => TRUE));
  $variables['date'] = format_interval(REQUEST_TIME - $activity->creationDate);

  $node = '';
  if ($variables['node'] && $activity->contentType == CultureFeed_Activity::CONTENT_TYPE_PAGE) {
    $node = l($variables['node']->title, 'node/' . $variables['node']->nid);
  }
  else {
    $node = cnapi_url_dp2dul($variables['node']['title'], array($activity->contentType => $variables['node']['cdbid'], 'title' => $variables['node']['title']));
  }

  $type_prefix = '';
  $type_suffix = '';

  switch ($activity->type) {
    case CultureFeed_Activity::TYPE_VIEW:
      $type_prefix = 'heeft';
      $type_suffix = 'bekeken';
      break;
    case CultureFeed_Activity::TYPE_DETAIL:
      $type_prefix = 'heeft';
      $type_suffix = 'bekeken';
      break;
    case CultureFeed_Activity::TYPE_LIKE:
      $type_prefix = 'vindt';
      $type_suffix = 'leuk';
      break;
    case CultureFeed_Activity::TYPE_MAIL:
      $type_prefix = 'heeft';
      $type_suffix = 'gemaild';
      break;
    case CultureFeed_Activity::TYPE_PRINT:
      $type_prefix = 'heeft';
      $type_suffix = 'afgedrukt';
      break;
    case CultureFeed_Activity::TYPE_FACEBOOK:
      $type_prefix = 'heeft';
      $type_suffix = 'via Facebook gedeeld';
      break;
    case CultureFeed_Activity::TYPE_TWITTER:
      $type_prefix = 'heeft';
      $type_suffix = 'via Twitter gedeeld';
      break;
    case CultureFeed_Activity::TYPE_IK_GA:
      $type_prefix = 'gaat naar';
      $type_suffix = '';
      break;
  }

  $replace = array(
    '!nick' => l($activity->nick, $url),
    '!type_prefix' => $type_prefix,
    '!type_suffix' => $type_suffix,
    '!node' => $node,
  );

  $variables['text'] = strtr('!nick !type_prefix !node !type_suffix.', $replace);
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

  $picture = theme('image', array('path' => $user->depiction . '?maxwidth=100&maxheight=100&crop=auto'));
  $url = 'user/' . $variables['uid'];
  $variables['picture'] = l($picture, $url, array('html' => TRUE));
  $variables['nick'] = l($user->nick, $url);
}

/**
 * Preprocess variables for culturefeed-ui-profile-box.tpl.php.
 *
 * @see culturefeed-ui-profile-box.tpl.php
 */
function template_preprocess_culturefeed_ui_profile_box(&$variables) {
  $variables['is_logged_in'] = !user_is_anonymous();

  if (!user_is_anonymous()) {
    $variables['picture'] = theme('image', array('path' => $variables['picture']));
    $variables['nick'] = l($variables['nick'], $variables['url_profile']);
    $variables['link_profile'] = l('Mijn UiTid', $variables['url_profile']);
  }

  $variables['link_logout'] = l('Uitloggen', 'user/logout');
  $variables['link_login'] = l('Inloggen', 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect')), 'query' => drupal_get_destination()));
  $variables['link_register'] = l('Registereren', 'culturefeed/oauth/connect/register', array('attributes' => array('class' => array('culturefeedconnect')), 'query' => drupal_get_destination()));
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
  $picture = theme('image', array('path' => $account->depiction . '?maxwidth=50&maxheight=50&crop=auto'));

  $variables['picture'] = l($picture, $url, array('html' => TRUE));

  switch ($variables['sort']) {
    case CultureFeed_SearchUsersQuery::SORT_NUMBEROFACTIVITIES:
      $type_text = 'gedaan';
      break;
    case CultureFeed_SearchUsersQuery::SORT_NUMBEROFLIKES:
      $type_text = 'leuk gevonden';
      break;
    case CultureFeed_SearchUsersQuery::SORT_NUMBEROFSHARESFACEBOOK:
      $type_text = 'op Facebook gedeeld';
      break;
    case CultureFeed_SearchUsersQuery::SORT_NUMBEROFSHARESTWITTER:
      $type_text = 'op Twitter gedeeld';
      break;
    case CultureFeed_SearchUsersQuery::SORT_NUMBEROFATTENDS:
      $type_text = 'bijgewoond';
      break;
    case CultureFeed_SearchUsersQuery::SORT_NUMBEROFACTIVEACTIVITIES:
      $type_text = 'gedaan';
      break;
  }

  $replace = array(
    '!nick' => l($account->nick, $url),
    '!sort_value' => $variables['sort_value'],
    '!type' => $type_text,
    '!uitslover' => l('UiTslover', 'culturefeed/users/mostactive'),
  );

  $variables['text'] = strtr('!nick heeft vorige week !sort_value activiteiten !type en is de !uitslover van de week. Proficiat !nick', $replace);
}

/**
 * Preprocess variables for culturefeed-ui-related-events.tpl.php.
 *
 * @see culturefeed-ui-related-events.tpl.php
 */
function template_preprocess_culturefeed_ui_related_events(&$variables) {
  // events
  $items = array();
  
  $i = 0;
  foreach ($variables['events'] as $event_info) {
    $theme = 'cnapi_ui_event_mini_summary';
    
    if ($i++ == 0) {
      $theme = 'cnapi_ui_event_summary';
    }
    
    $items[] = theme($theme, array('event' => $event_info, 'heading_level' => 3));
  }
  
  $variables['event_list'] = theme('item_list', array('items' => $items));
  
  // links
  $links = array();
  
  // links : headings
  foreach ($variables['headings'] as $heading) {
    $request = array('context' => 'event', 'query' => array('heading' => $heading['hid']));
    $links[] = cnapi_url_dp2dul('Meer ' . $heading['name'], $request);
  }
  
  // links : zip
  $request = array('context' => 'event', 'query' => array('zip' => $variables['zip']));
  $links[] = cnapi_url_dp2dul('Meer in postcode ' . $variables['zip'], $request);
  
  $variables['link_list'] = theme('item_list', array('items' => $links));
}

/**
 * Preprocess variables for culturefeed-ui-related-events.tpl.php.
 *
 * @see culturefeed-ui-related-events.tpl.php
 */
function template_preprocess_culturefeed_ui_service_consumer_summary(&$variables) {
  $consumer = $variables['consumer'];
  $consumer instanceof CultureFeed_Consumer;
  
  $variables['logo'] = isset($consumer->logo) ? theme('image' , array('path' => $consumer->logo . '?maxwidth=100&maxheight=100&crop=auto')) : '';
  $variables['name'] = isset($consumer->name) ? $consumer->name : '';
  $variables['description'] = isset($consumer->description) ? $consumer->description : '';
  $variables['creation_date'] = 'Gekoppeld op ' . format_date($consumer->creationDate, 'custom', 'j F Y');
  $variables['revoke_link'] = l('Verwijder', 'culturefeed/serviceconsumers/revoke/' . $consumer->id);
}