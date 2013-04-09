<?php
/**
 * @file
 * Theming / preprocess functions for culturefeed_social.
 */

/**
 * Theme the user activity list.
 */
function theme_culturefeed_social_user_activity_list(&$variables) {

  $activities = $variables['activities'];
  $items = array();

  $accounts = culturefeed_get_uids_for_activities($activities->objects);

  foreach ($activities->objects as $activity) {
    if (!isset($accounts[$activity->userId])) {
      continue;
    }

    $items[] = theme('culturefeed_social_user_activity_summary', array('activity' => $activity, 'uid' => $accounts[$activity->userId]));
  }

  return theme('item_list', array('items' => $items, 'attributes' => array('class' => $variables['class'])));

}

/**
 * Preprocess function for the user activity summary.
 * @see culturefeed-social-user-activity-summary.tpl.php
 */
function culturefeed_social_preprocess_culturefeed_social_user_activity_summary(&$variables) {

  $activity = $variables['activity'];

  $picture = theme('image', array('path' => $activity->depiction . '?maxwidth=50&maxheight=50&crop=auto'));
  $url = 'user/' . $variables['uid'];

  $variables['picture'] = l($picture, $url, array('html' => TRUE));
  $variables['date'] = format_interval(REQUEST_TIME - $activity->creationDate) . ' geleden.';

  $node = '';
  if ($activity->contentType == CultureFeed_Activity::CONTENT_TYPE_PAGE) {
    $node = l($activity->nodeTitle, 'node/' . $activity->nodeId);
  }
  else {
    $node = culturefeed_search_detail_l($activity->contentType, $activity->nodeId, $activity->nodeTitle);
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

  $variables['text'] = rtrim(strtr('!nick !type_prefix !node !type_suffix', $replace), ' ') . '.';

}