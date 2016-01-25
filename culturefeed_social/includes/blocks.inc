<?php
/**
 * @file
 * Block callbacks for the social module.
 */

/**
 * Show the recent activities as block.
 */
function culturefeed_social_block_recent_activities() {

  $query = new CultureFeed_SearchActivitiesQuery();
  $query->max = variable_get('culturefeed_social_recent_activities_total_items', CULTUREFEED_SOCIAL_RECENT_ACTIVITIES_COUNT);
  $query->type = array_keys(culturefeed_social_get_activity_filter_options(TRUE));
  $query->consumer = variable_get('culturefeed_api_application_key', '');

  try {
    $activities = DrupalCultureFeed::searchActivities($query);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_social', $e);
    return;
  }

  if ($activities->total == 0) {
    return;
  }

  $content = array();
  $content['activities'] = array(
    '#theme' => 'culturefeed_social_user_activity_list',
    '#activities' => $activities,
    '#class' => 'site-activity-list',
    '#read_more_text' => t('All actions'),
    '#read_more_url' => 'culturefeed/activities/recent',
  );

  $block['subject'] = t('Recent actions');
  $block['content'] = $content;

  return $block;

}

/**
 * Show the users recent activities.
 */
function culturefeed_social_block_user_activities($account) {

  if (empty($account->culturefeed_uid)) {
    return;
  }

  $active_types = culturefeed_social_get_activity_filter_options(TRUE);
  $query = new CultureFeed_SearchActivitiesQuery();
  $query->max = CULTUREFEED_SOCIAL_BLOCK_USER_ACTIVITIES_COUNT;
  $query->type = array_keys($active_types);
  $query->userId = $account->culturefeed_uid;
  $query->private = FALSE;
  $query->consumer = variable_get('culturefeed_api_application_key', '');

  try {
    $activities = DrupalCultureFeed::searchActivities($query);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_social', $e);
    return;
  }

  if (empty($activities->objects)) {
    $content['activities'] = array(
      '#markup' => '<p class="no-actions-yet">' . t('This user didn\'t perform any actions yet') . '</p>'
    );
  }
  else {

    if (!empty($active_types) && $activities->total > 20) {
      $content['filter'] = drupal_get_form('culturefeed_social_user_activity_filter_form', 'all', TRUE, $account);
    }

    if ($account->culturefeed_uid == DrupalCultureFeed::getLoggedInUserId()) {

      $content['edit'] = array(
        '#markup' => '<p class="pull-right">' . l('<i class="icon-pencil"></i> ' . t('Manage your actions'), 'culturefeed/activities', array('html' => TRUE, 'attributes' => array('class' => array('manage-activities-link')))) . '</p><div class="clearfix"></div>',
      );

      $facebook = _culturefeed_ui_facebook_privacy_toggle();
      if ($facebook) {
        $item_list = array(
          '#theme' => 'item_list',
          '#attributes' => array(
            'id' => 'facebook-privacy-toggle',
            ),
          '#items' => array(
            array(
              'data' => $facebook,
              'id' => 'onlineaccount-facebook',
            ),
          ),
        );
        $content['facebook'] = array(
          '#markup' => drupal_render($item_list),
        );
      }

    }


    $content['activities'] = array(
      '#theme' => 'culturefeed_social_user_activity_list',
      '#activities' => $activities,
      '#class' => 'user-activity-list',
      '#attached' => array('js' => array(drupal_get_path('module', 'culturefeed_social') . '/js/activities.js')),
    );

    if ($activities->total > count($activities->objects)) {
      $content['activities']['#read_more_url'] = url('culturefeed/ajax/user/activities', array('query' => array('culturefeed_uid' => $account->culturefeed_uid)));
      $content['activities']['#read_more_text'] = t('Read previous messages');
    }

  }

  $block['subject'] = t('Recent actions');
  $block['content'] = $content;

  return $block;

}

/**
 * Block callback to render the comments on an item.
 * @param misc $object
 * @return array
 **/
function culturefeed_social_block_comments($object) {

  if (!$object || !method_exists($object, 'getType') || arg(4) == 'edit' || arg(4) == 'tags') {
    return;
  }

  // When you want to show comments on a node. The field_comments_enabled should be checked.
  if ($object instanceof DrupalNodeExtendedEntity) {
    $status = field_get_items('node', $object->getEntity(), 'field_comments_enabled');
    if (!$status || !$status[0]['value']) {
      return;
    }
  }

  $content_type = culturefeed_get_content_type($object->getType());

  $query = new CultureFeed_SearchActivitiesQuery();
  $query->type = CultureFeed_Activity::TYPE_COMMENT;
  //$query->contentType = $content_type;
  $query->nodeId = culturefeed_social_get_activity_node_id($content_type, $object);
  $query->depth = 2;

  try {
    $activities = DrupalCultureFeed::searchActivities($query);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_social', $e);
    return;
  }

  $content = array();
  $content['comment_list'] = culturefeed_social_comment_list($object, $activities);
  $content['comment_form'] = culturefeed_social_comment_actionbox($object);

  $block['subject'] = t('Comments');
  $block['content'] = $content;

  return $block;

}

/**
 * Show a block with the latest notifications for this user.
 */
function culturefeed_social_block_my_notifications() {

  if (!culturefeed_is_culturefeed_user()) {
    return;
  }

  $block = array();
  $block['subject'] = t('My notifications');
  $block['content'] = array('#markup' => '<div class="no-results">' . t('You have no notifications'). '</div>');

  try {

    $params = array(
      'max' => CULTUREFEED_SOCIAL_BLOCK_NOTIFICATIONS_COUNT,
      'dateFrom' => date('Y-m-d', strtotime('-1 year')),
    );

    $notifications = DrupalCultureFeed::getNotifications(DrupalCultureFeed::getLoggedInUserId(), $params);
    if ($notifications->total > 0) {
      $block['content'] = array(
        '#theme' => 'culturefeed_social_user_activity_wall',
        '#activities' => $notifications,
        '#class' => 'notifications',
        '#read_more_url' => url('culturefeed/notifications'),
        '#read_more_text' => t('View all notifications'),
      );
    }

  }
  catch (Exception $e) {
    return;
  }

  return $block;

}

/**
 * Block to show a summary of my activities
 * @param stdClass $account
 */
function culturefeed_social_block_my_activity_summary($account) {

  if (empty($account->culturefeed_uid)) {
    return;
  }

  $has_pages = module_exists('culturefeed_pages');
  $has_books = module_exists('bibnet_books');
  $active_types = culturefeed_social_get_activity_filter_options(TRUE);

  $types = array();
  foreach ($active_types as $type => $activity_config) {
    $allowed_types = $activity_config->allowedTypes;
    foreach ($allowed_types as $ctype) {
      if ($ctype == 'page' && !$has_pages) {
        continue;
      }
      if ($ctype == 'book' && !$has_books) {
        continue;
      }
      $types[] = $type . '_' . $ctype;
    }
  }

  $view_own_activities = $account->uid == $GLOBALS['user']->uid ? TRUE : FALSE;
  $private = FALSE;
  try {
    $totals = DrupalCultureFeed::getTotalActivities($account->culturefeed_uid, $types, $private);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_social', $e);
    return;
  }

  $content = array();
  if ($totals) {

    $content = array(
      '#theme' => 'culturefeed_social_activity_summary',
      '#totals' => $totals,
      '#private' => $view_own_activities,
      '#account_name' => $account->name,
    );
  }

  $block['subject'] = '';
  $block['content'] = $content;

  return $block;

}

/**
 * Show the activity summary for this page.
 *
 * @page Culturefeed_Page
 */
function culturefeed_social_block_page_activity_summary($page) {

  if (empty($page) || !($page instanceOf CultureFeed_Cdb_Item_Page)) {
    return;
  }

  $has_pages = module_exists('culturefeed_pages');
  $active_types = culturefeed_social_get_activity_filter_options(TRUE);

  $types = array();
  foreach ($active_types as $type => $activity_config) {
    if (empty($activity_config)) {
      continue;
    }
    $allowed_types = $activity_config->allowedTypes;
    foreach ($allowed_types as $ctype) {
      if ($ctype == 'page' && !$has_pages) {
        continue;
      }
      if ($ctype == 'book' && !$has_pages) {
        continue;
      }
      $types[] = $type . '_' . $ctype;
    }
  }

  $private = FALSE;
  try {
    $totals = DrupalCultureFeed::getTotalPageActivities($page->getId(), $types, $private);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_social', $e);
    return;
  }

  $content = array();
  if ($totals) {

    $content = array(
      '#theme' => 'culturefeed_social_page_activity_summary',
      '#totals' => $totals,
    );
  }

  $block['subject'] = '';
  $block['content'] = $content;

  return $block;

}

/**
 * Return a block build of users that did an activity of a given type on a given object.
 * @param $object
 *   Cdb item to build the activity list for.
 * @param string $activity_type
 *   Type activity to show a list for.
 */
function culturefeed_social_user_list_block_for_activity($object, $activity_type) {

  if (!$object || !method_exists($object, 'getType')) {
    return;
  }

  $config = culturefeed_social_activity_get_config($activity_type);
  $content_type = culturefeed_get_content_type($object->getType());

  $query = new CultureFeed_SearchActivitiesQuery();
  $query->type = $activity_type;
  $query->contentType = $content_type;
  $query->consumer = variable_get('culturefeed_api_application_key', '');

  switch ($content_type) {

    case CultureFeed_Activity::CONTENT_TYPE_BOOK:
      $node_title = $object->getTitle(culturefeed_search_get_preferred_language());
      $id = $object->getId();
      $query->nodeId = url(culturefeed_social_detail_path($content_type, $id, $node_title), array('absolute' => TRUE));
      break;

    case CultureFeed_Activity::CONTENT_TYPE_EVENT:
    case CultureFeed_Activity::CONTENT_TYPE_PRODUCTION:
      $node_title = $object->getTitle(culturefeed_search_get_preferred_language());
      $id = $object->getId();
      $query->nodeId = $id;
      break;
  }

  try {
    $activities = DrupalCultureFeed::searchActivities($query);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_social', $e);
    return;
  }

  $content = array();
  $content['#theme'] = 'culturefeed_social_user_list_block_for_activity';
  $content['#activity_type'] = $activity_type;

  if ($activities->total > 0) {

    $content['#activities_total'] = $activities->total;

    $user_objects = array();
    foreach ($activities->objects as $activity_object) {
      $user_object = new stdClass();
      $user_object->nick = $activity_object->nick;
      $user_object->id = $activity_object->userId;
      $user_object->depiction = $activity_object->depiction;
      $user_objects[] = $user_object;
    }

    if (count($user_objects) > 0) {
      $content['#list'] = _culturefeed_ui_users_list($user_objects);
    }
  }

  // Advanced activity link.
  $content['#activity_link'] = culturefeed_social_activity_link_advanced($activity_type, $object, FALSE);

  $block['subject'] = $config->subject;
  $block['content'] = $content;

  return $block;
}
