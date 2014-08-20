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
  $user_url = 'user/' . $variables['uid'];

  $variables['picture'] = l($picture, $user_url, array('html' => TRUE));

  $date = $activity->creationDate;
  $time_zone  = new DateTimeZone('Europe/Brussels');
  $last_reply = new DateTime();
  $last_reply->setTimestamp($date);
  $last_reply->setTimezone($time_zone);

  // Check if the activity happened today.
  $time_ago = $_SERVER['REQUEST_TIME'] - $date;
  if ($time_ago < (60 * 60 * 60)) {
    $variables['date'] = format_interval($time_ago) . ' ' . t('ago') . '.';
  }
  else {

    if ($date >= strtotime("today")) {
      $variables['date'] = t('Today at') . ' ' . $last_reply->format('H:i') . ' '  . t('hour');
    }
    else if ($date >= strtotime("yesterday")) {
      $variables['date'] = t('Yesterday at') . ' ' . $last_reply->format('H:i') . ' '  . t('hour');
    }
    else {
      $variables['date'] = format_date($date, 'custom', 'd M \o\m H:i') . ' '  . t('hour');
    }

  }

  // Not possible to link when it's an activity on an activity.
  if ($activity->contentType == CultureFeed_Activity::CONTENT_TYPE_ACTIVITY) {
    $link = check_plain($activity->nodeTitle);
  }
  else {
    $url = culturefeed_social_activity_object_path($activity->contentType, $activity->nodeId, $activity->nodeTitle);
    $link = l($activity->nodeTitle, $url);
  }

  $type_prefix = '';
  $type_suffix = '';

  $config = culturefeed_social_activity_get_config($activity->type);

  // If no config available, show default text
  if ($config) {
    $type_prefix = $config->viewPrefix;
    $type_suffix = $config->viewSuffix;
  }

  $variables['nick'] = l($activity->nick, $user_url);
  $variables['prefix'] = $type_prefix;
  $variables['suffix'] = $type_suffix;
  $variables['link'] = $link;

  // If onbehalf is known, show it only when culturefeed pages is enabled.
  if (!empty($activity->onBehalfOf) && !empty($activity->onBehalfOfName) && module_exists('culturefeed_pages')) {
    $variables['nick'] .= ' ' . t('from') . ' ' . culturefeed_search_detail_l('page', $activity->onBehalfOf, $activity->onBehalfOfName);
  }

}

/**
 * Preprocess function for the comment list.
 * @see culturefeed-social-comment-list.tpl.php
 */
function culturefeed_social_preprocess_culturefeed_social_comment_list(&$variables) {

  // Always include the javascript for ajax behavior.
  drupal_add_library('system', 'drupal.ajax');

  $object = $variables['object'];
  $activities = $variables['activities'];
  $accounts = $variables['accounts'];
  $variables['url'] = url(culturefeed_social_detail_path($object->getType(), $object->getId(), $object->getTitle(culturefeed_search_get_preferred_language())));

  $list = array();
  foreach ($activities as $activity) {
    $list[] = theme('culturefeed_social_comment_list_item', array(
      'activity' => $activity,
      'object' => $object,
      'uid' => $accounts[$activity->userId],
      'child_activities' => $activity->childActivities,
      'accounts' => $accounts,
      'level' => 0,
    ));
  }
  $variables['list'] = $list;

}

/**
 * Preprocess function for the comment list item.
 * @see culturefeed-social-comment-list-item.tpl.php
 */
function culturefeed_social_preprocess_culturefeed_social_comment_list_item(&$variables) {

  $object = $variables['object'];
  $activity = $variables['activity'];
  $accounts = $variables['accounts'];
  $destination = drupal_get_destination();
  $cf_user = NULL;

  if (culturefeed_is_culturefeed_user()) {
    try {
      $cf_user = DrupalCultureFeed::getLoggedInUser();
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_social', $e);
    }
  }

  // Variables for one list item.
  $picture = theme('image', array('path' => $activity->depiction . '?maxwidth=75&maxheight=75&crop=auto'));
  $author_url = 'user/' . $variables['uid'];

  $variables['picture'] = l($picture, $author_url, array('html' => TRUE));
  $variables['date'] = format_date($activity->creationDate, 'small');

  $variables['author'] = l($activity->nick, $author_url);
  if (!empty($activity->onBehalfOf) && !empty($activity->onBehalfOfName) && module_exists('culturefeed_pages')) {
    $variables['author'] .= ' ' . t('from') . ' ' . culturefeed_search_detail_l('page', $activity->onBehalfOf, $activity->onBehalfOfName);
  }

  $variables['content'] = check_plain(strip_tags($activity->value)); // CultuurNet doesn't want to see html tags converted to plain.
  $variables['content'] = str_replace("\n", "<br />", $variables['content']);
  $variables['activity_id'] = $activity->id;

  // The list of child activities if available.
  $variables['list'] = array();
  if (!empty($variables['child_activities'])) {
    // The subitems.
    $list = array();
    foreach ($variables['child_activities'] as $child_activity) {
     $list[] = theme('culturefeed_social_comment_list_item', array(
       'activity' => $child_activity,
       'object' => $object,
       'uid' => $accounts[$child_activity->userId],
       'child_activities' => array(),
       'accounts' => array(),
       'level' => 1,
     ));
    }
    $variables['list'] = $list;
  }

  $variables['comment_link'] = '';
  $variables['comment_url'] = '';
  $variables['delete_link'] = '';
  $variables['abuse_link'] = '';

  if ($cf_user) {

    $remove_path = 'culturefeed/activity/delete/' . $activity->id;
    $attributes = array(
      'class' => array('remove-link'/*, 'use-ajax'*/),
      'role' => 'button',
      'data-toggle' => 'modal',
      'data-target' => '#delete-wrapper-' . $activity->id,
      'data-remote' => url($remove_path . "/ajax", array('query' => $destination)),
    );

    if ($variables['level'] == 0) {

      if ($cf_user->id == $activity->userId || (module_exists('culturefeed_pages') && culturefeed_pages_is_user_admin_of_page($object->getId()))) {
        $variables['delete_link'] = l(t('Delete'), $remove_path . '/nojs', array(
          'attributes' => $attributes,
          'query' => $destination,
        ));
      }

      $comment_url = 'culturefeed/activity/comment/' . $activity->id;
      $attributes = array(
        'class' => array('comment-link link-icon'),
        'role' => 'button',
        'data-toggle' => 'modal',
        'data-target' => '#comment-wrapper-' . $activity->id,
        'data-remote' => url($comment_url . "/ajax", array('query' => $destination)),
      );

      $variables['comment_link'] = l(t('Reply'), $comment_url . '/nojs', array(
        'attributes' => $attributes,
        'query' => $destination,
      ));

     $variables['comment_url'] = url($comment_url . '/nojs', array('attributes' => $attributes, 'query' => $destination));

    }
    else {

      if ($cf_user->id == $activity->userId || culturefeed_pages_is_user_admin_of_page($object->getId())) {
        $variables['delete_link'] = l(t('Delete'), $remove_path . '/nojs', array(
          'attributes' => $attributes,
          'query' => $destination,
          'html' => TRUE,
        ));
      }

    }
  }

  if (module_exists('culturefeed_messages')) {

    $abuse_url = 'culturefeed/activity/report-abuse/' . $activity->id;
    $attributes = array(
      'class' => array('comment-abuse-link'),
      'role' => 'button',
      'data-toggle' => 'modal',
      'data-target' => '#abuse-wrapper-' . $activity->id,
      'data-remote' => url($abuse_url . "/ajax", array('query' => $destination)),
    );

    $variables['abuse_link'] = l(t('Report as inappropriate'), $abuse_url . '/nojs', array(
      'attributes' => $attributes,
      'query' => $destination,
    ));

  }

}

/**
 * Preprocess the variables for the user notifications page.
 * @see culturefeed-social-user-notifications-page.tpl.php
 */
function culturefeed_social_preprocess_culturefeed_social_user_notifications_page(&$variables) {

  $notifications = array();

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


/**
 * Preprocess the variables for the summary of activities of a user.
 * @see culturefeed-social-activity-summary.tpl.php
 */
function culturefeed_social_preprocess_culturefeed_social_activity_summary(&$variables) {

  $total_per_type = array();

  foreach ($variables['totals'] as $total) {

    // Conventional totals for each type/content type.
    list($activity_type, $content_type) = explode("_", $total->key);
    $variables[$content_type . '_' . $activity_type] = $total->value;

    if (!isset($total_per_type[$activity_type])) {
      $total_per_type[$activity_type] = array('activities' => 0, 'books' => 0, 'pages' => 0);
    }

    if (in_array($content_type, array('actor', 'production', 'event'))) {
      $total_per_type[$activity_type]['activities'] += $total->value;
    }
    elseif ($content_type == 'book') {
      $total_per_type[$activity_type]['books'] = $total->value;
    }
    elseif ($content_type == 'page') {
      $total_per_type[$activity_type]['pages'] = $total->value;
    }

  }

  foreach ($total_per_type as $atype => $total_per_content_type) {

    $variables['total_' . $atype] = $total_per_content_type['books'] + $total_per_content_type['pages'] + $total_per_content_type['activities'];
    $variables['books_total_' . $atype] = $total_per_content_type['books'];
    $variables['pages_total_' . $atype] = $total_per_content_type['pages'];
    $variables['activities_total_' . $atype] = $total_per_content_type['activities'];

  }

  if ($variables['private']) {
    $variables['subject'] = t('My activities');
  }
  else {
    $variables['subject'] = t('Activities of @username', array('@username' => $variables['account_name']));
  }

}

/**
 * Preprocess the variables for the summary of activities for a page.
 * @see culturefeed-social-page-activity-summary.tpl.php
 */
function culturefeed_social_preprocess_culturefeed_social_page_activity_summary(&$variables) {

  $total_per_type = array();

  foreach ($variables['totals'] as $total) {

    // Conventional totals for each type/content type.
    list($activity_type, $content_type) = explode("_", $total->key);
    $variables[$content_type . '_' . $activity_type] = $total->value;

    if (!isset($total_per_type[$activity_type])) {
      $total_per_type[$activity_type] = array('activities' => 0, 'books' => 0, 'pages' => 0);
    }

    if (in_array($content_type, array('actor', 'production', 'event'))) {
      $total_per_type[$activity_type]['activities'] += $total->value;
    }
    elseif ($content_type == 'book') {
      $total_per_type[$activity_type]['books'] = $total->value;
    }
    elseif ($content_type == 'page') {
      $total_per_type[$activity_type]['pages'] = $total->value;
    }

  }

  foreach ($total_per_type as $atype => $total_per_content_type) {

    $variables['total_' . $atype] = $total_per_content_type['books'] + $total_per_content_type['pages'] + $total_per_content_type['activities'];
    $variables['books_total_' . $atype] = $total_per_content_type['books'];
    $variables['pages_total_' . $atype] = $total_per_content_type['pages'];
    $variables['activities_total_' . $atype] = $total_per_content_type['activities'];

  }

  $variables['subject'] = t('Activities');

}

/**
 * Theme the login required message when an anonymous user sees a social action.
 */
function theme_culturefeed_social_login_required_message($variables) {

  $config = culturefeed_social_activity_get_config($variables['activity_type']);

  return '<div class="login-required">' . $config->loginRequiredMessage . '</div>';

}

/**
 * Theme the notifications profile box item.
 */
function theme_culturefeed_social_profile_box_item_notifications($variables) {
  return l(format_plural($variables['total'], '@count new notification', '@count new notifications'), 'culturefeed/notifications');
}
