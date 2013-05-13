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

  $url = culturefeed_social_activity_object_path($activity->contentType, $activity->nodeId, $activity->nodeTitle);
  $link = l($activity->nodeTitle, $url);

  $type_prefix = '';
  $type_suffix = '';

  $config = culturefeed_social_activity_get_config($activity->type);
  // If no config available, show default text
  if ($config) {
    $type_prefix = $config->viewPrefix;
    $type_suffix = $config->viewSuffix;
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
 * Preprocess function for the recommendation list.
 * @see culturefeed-social-recommendation-list.tpl.php
 */
function culturefeed_social_preprocess_culturefeed_social_recommendation_list(&$variables) {

  // Always include the javascript for ajax behavior.
  drupal_add_library('system', 'drupal.ajax');

  $object = $variables['object'];
  $activities = $variables['activities'];
  $accounts = $variables['accounts'];

  $variables['url'] = culturefeed_social_detail_path($object->getType(), $object->getId(), $object->getTitle($GLOBALS['language']->language));

  $list = array();
  foreach ($activities as $activity) {
    $list[] = theme('culturefeed_social_recommendation_list_item', array(
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
 * Preprocess function for the recommendation list item.
 * @see culturefeed-social-recommendation-list-item.tpl.php
 */
function culturefeed_social_preprocess_culturefeed_social_recommendation_list_item(&$variables) {

  $object = $variables['object'];
  $activity = $variables['activity'];
  $accounts = $variables['accounts'];
  $destination = drupal_get_destination();
  $cf_user = DrupalCultureFeed::getLoggedInUser();

  // Variables for one list item.
  $picture = theme('image', array('path' => $activity->depiction . '?maxwidth=50&maxheight=50&crop=auto'));
  $url = 'user/' . $variables['uid'];
  $variables['picture'] = l($picture, $url, array('html' => TRUE));
  $variables['date'] = format_date($activity->creationDate, 'small');
  $variables['author'] = format_string('!username van !active_page', array(
      '!username' => $activity->nick, '!active_page' => $activity->onBehalfOfName));
  $variables['content'] = check_plain($activity->value);
  $variables['activity_id'] = $activity->id;

  // The list of child activities if available.
  $variables['list'] = array();
  if (!empty($variables['child_activities'])) {
    // The subitems.
    $list = array();
    foreach ($variables['child_activities'] as $child_activity) {
     $list[] = theme('culturefeed_social_recommendation_list_item', array(
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

  $variables['react_form'] = '';
  $variables['react_link'] = '';
  $variables['delete_link'] = '';
  $variables['abuse_link'] = '';

  if ($variables['level'] == 0) {

    if ($cf_user->id == $activity->userId || culturefeed_pages_is_user_admin_of_page($object->getId())) {
      $variables['delete_link'] = l('Verwijder reactie | X', 'culturefeed/activity/delete/' . $activity->id . '/nojs', array(
        'attributes' => array('class' => array('remove-link', 'use-ajax')),
        'query' => $destination,
      ));
    }
    $variables['react_link'] = l('Reageer op deze beoordeling', $_GET['q'], array('attributes' => array('class' => array('recommend-subform-toggle'))));
    $sub_form = culturefeed_social_recommendation_actionbox($object, $activity);
    $variables['react_form'] = '<div id="recommendation-subform-' . $activity->id . '" class="recommendation-subform">' . drupal_render($sub_form) . '</div>';

  }
  else {

    if ($cf_user->id == $activity->userId || culturefeed_pages_is_user_admin_of_page($object->getId())) {
      $variables['delete_link'] = l('X', 'culturefeed/activity/delete/' . $activity->id . '/nojs', array(
        'attributes' => array('class' => array('remove-link', 'use-ajax')),
        'query' => $destination,
      ));
    }

  }

  if (module_exists('culturefeed_messages')) {

    $variables['abuse_link'] = l('Rapporteer als ongepast', $_GET['q'], array('attributes' => array('class' => array('recommend-abuse-toggle'))));
    $abuse_form = drupal_get_form('culturefeed_social_report_abuse_form', $activity, culturefeed_search_get_active_object());
    $variables['abuse_form'] = '<div class="recommendation-abuse-form">' . drupal_render($abuse_form) . '</div>';

  }

}

/**
 * Preprocess the variables for the user notifications page.
 * @see culturefeedsocial-user-notifications-page.tpl.php
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