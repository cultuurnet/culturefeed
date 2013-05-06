<?php
/**
 * @file
 * Theming / preprocess functions for culturefeed_social.
 */

/**
 * Preprocess the user activity list.
 * @see culturefeed-social-user-activity-list.tpl.php
 */
function culturefeed_social_preprocess_culturefeed_social_user_activity_list(&$variables) {

  $activities = $variables['activities'];
  $items = array();

  $accounts = culturefeed_get_uids_for_activities($activities->objects);

  foreach ($activities->objects as $activity) {
    if (!isset($accounts[$activity->userId])) {
      continue;
    }

    $items[] = theme('culturefeed_social_user_activity_summary', array('activity' => $activity, 'uid' => $accounts[$activity->userId]));
  }

  $variables['items'] = $items;

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

  $url = culturefeed_social_detail_path($activity->contentType, $activity->nodeId, $activity->nodeTitle);
  $link = l($activity->nodeTitle, $url);

  $type_prefix = '';
  $type_suffix = '';

  switch ($activity->type) {
    case $activity::TYPE_VIEW:
      $type_prefix = 'heeft';
      $type_suffix = 'bekeken';
      break;
    case $activity::TYPE_DETAIL:
      $type_prefix = 'heeft';
      $type_suffix = 'bekeken';
      break;
    case $activity::TYPE_LIKE:
      $type_prefix = 'vindt';
      $type_suffix = 'leuk';
      break;
    case $activity::TYPE_MAIL:
      $type_prefix = 'heeft';
      $type_suffix = 'gemaild';
      break;
    case $activity::TYPE_PRINT:
      $type_prefix = 'heeft';
      $type_suffix = 'afgedrukt';
      break;
    case $activity::TYPE_FACEBOOK:
      $type_prefix = 'heeft';
      $type_suffix = 'via Facebook gedeeld';
      break;
    case $activity::TYPE_TWITTER:
      $type_prefix = 'heeft';
      $type_suffix = 'via Twitter gedeeld';
      break;
    case $activity::TYPE_IK_GA:
      $type_prefix = 'gaat naar';
      $type_suffix = '';
      break;
    case $activity::TYPE_RECOMMEND:
      $type_prefix = 'heeft';
      $type_suffix = 'aangeraden';
      break;
    case $activity::TYPE_FOLLOW:
      $type_prefix = 'volgt';
      break;
    case $activity::TYPE_COMMENT:
      $type_prefix = 'heeft over';
      $type_suffix = 'een beoordeling geschreven';
    break;

    case $activity::TYPE_PAGE_MEMBER:
      $type_prefix = 'werd lid van';
    break;

    case $activity::TYPE_PAGE_ADMIN:
      $type_prefix = 'werd administrator van';
    break;

  }

  $replace = array(
    '!nick' => l($activity->nick, $url),
    '!type_prefix' => $type_prefix,
    '!type_suffix' => $type_suffix,
    '!link' => $link,
  );

  // If onbehalf is known, show it only when culturefeed pages is enabled.
  if (!empty($activity->onBehalfOf) && !empty($activity->onBehalfOfName) && module_exists('culturefeed_pages')) {
    $replace['!nick'] .= ' van ' . culturefeed_search_detail_l('page', $activity->onBehalfOf, $activity->onBehalfOfName);
  }

  $variables['text'] = rtrim(strtr('!nick !type_prefix !link !type_suffix', $replace), ' ') . '.';

}

/**
 * Preprocess the variables for the user notifications page.
 * @see culturefeedsocial-user-notifications-page.tpl.php
 */
function culturefeed_social_preprocess_culturefeed_social_user_notifications_page(&$variables) {

  $notifications = array();
  dsm($variables);
  // Split the notification resultset in 2 resultsets.
  foreach ($variables['notifications']->objects as $notification) {

    if (!isset($notifications[$notification->status])) {
      $notifications[$notification->status] = new CultureFeed_ResultSet(0, array());
    }

    $notifications[$notification->status]->total++;
    $notifications[$notification->status]->objects[] = $notification;

  }

  // Render the 2 lists.
  $variables['new_notifications_count'] = 0;
  if (isset($notifications[Culturefeed::NOTIFICATION_TYPE_NEW])) {
    $variables['new_notifications_count'] = $notifications[Culturefeed::NOTIFICATION_TYPE_NEW]->total;
    $variables['new_notifications'] = theme('culturefeed_social_user_activity_list', array('activities' => $notifications[Culturefeed::NOTIFICATION_TYPE_NEW]));
  }

  $variables['read_notifications_count'] = 0;
  if (isset($notifications[Culturefeed::NOTIFICATION_TYPE_READ])) {
    $variables['read_notifications_count'] = $notifications[Culturefeed::NOTIFICATION_TYPE_READ]->total;
    $variables['read_notifications'] = theme('culturefeed_social_user_activity_list', array('activities' => $notifications[Culturefeed::NOTIFICATION_TYPE_READ]));
  }

}