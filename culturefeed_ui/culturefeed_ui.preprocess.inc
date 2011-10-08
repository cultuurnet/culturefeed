<?php

/**
 * Preprocess variables for cnapi-ui-activity-summary.tpl.php.
 *
 * @see culturefeed-ui-activity-summary.tpl.php
 */
function template_preprocess_culturefeed_ui_profile_menu_item(&$variables) {
  $variables['link'] = l($variables['title'], $variables['url']);
}

/**
 * Preprocess variables for cnapi-ui-activity-summary.tpl.php.
 *
 * @see culturefeed-ui-activity-summary.tpl.php
 */
function template_preprocess_culturefeed_ui_activity_summary(&$variables) {
  $activity = $variables['activity'];
  $activity instanceof CultureFeed_Activity;
  
  $picture = theme('image', array('path' => $activity->depiction));
  $url = 'user/' . culturefeed_user_get($activity->userId, $activity->nick)->uid;
  
  $variables['picture'] = l($picture, $url, array('html' => TRUE));
  $variables['date'] = format_interval(REQUEST_TIME - $activity->creationDate);
  
  $node = '';
  if ($variables['node']) {
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
      $type_suffix = 'via Twitter gedeeeld';
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
 * Preprocess variables for cnapi-ui-user-summarys.tpl.php.
 *
 * @see culturefeed-ui-user-summary.tpl.php
 */
function template_preprocess_culturefeed_ui_user_summary(&$variables) {
  $user = $variables['user'];
  $user instanceof CultureFeed_User;
  
  $picture = theme('image', array('path' => $user->depiction));
  $url = 'user/' . culturefeed_user_get($user->id, $user->nick)->uid;
  $variables['picture'] = l($picture, $url, array('html' => TRUE));
  $variables['nick'] = l($user->nick, $url);
}

/**
 * Preprocess variables for cnapi-ui-profile-box.tpl.php.
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
 * Preprocess variables for cnapi-ui-most-active-user.tpl.php.
 *
 * @see culturefeed-ui-most-active-user.tpl.php
 */
function template_preprocess_culturefeed_ui_most_active_user(&$variables) {
  $account = $variables['account'];
  $account instanceof CultureFeed_User;
  
  $url = 'user/' . culturefeed_user_get($account->id, $account->nick)->uid;
  $picture = theme('image', array('path' => $account->depiction));
  
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