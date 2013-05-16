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
  $query->max = CULTUREFEED_SOCIAL_RECENT_ACTIVITIES_COUNT;
  $query->type = array_keys(culturefeed_social_get_activity_filter_options(TRUE));

  try {
    $activities = DrupalCultureFeed::searchActivities($query);
  }
  catch (Exception $e) {
    dsm($e->getMessage());
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
  );

  $block['subject'] = 'Recente acties';
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

  try {
    $activities = DrupalCultureFeed::searchActivities($query);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_social', $e);
    return;
  }

  if (empty($activities->objects)) {
    $content['activities'] = array(
      '#markup' => 'Deze gebruiker heeft nog geen acties gedaan.',
    );
  }
  else {

    if (!empty($active_types)) {
      $content['filter'] = drupal_get_form('culturefeed_social_user_activity_filter_form', 'all', TRUE, $account);
    }

    if ($account->culturefeed_uid == DrupalCultureFeed::getLoggedInUserId()) {

      $content['edit'] = array(
        '#markup' => '<p class="pull-right">' . l('<i class="icon-pencil"></i> Beheer jouw acties', 'culturefeed/activities', array('html' => TRUE, 'attributes' => array('class' => array('manage-activities-link')))) . '</p><div class="clearfix"></div>',
      );

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
      $content['activities']['#read_more_text'] = 'Lees oudere berichten';
    }

  }

  $block['subject'] = 'Recente acties';
  $block['content'] = $content;

  return $block;

}

/**
 * Block callback to render the users who have recommended the page.
 * @param misc $object
 * @return array
 */
function culturefeed_social_block_recommended_by($object) {

  if (!$object || !method_exists($object, 'getType')) {
    return;
  }

  $config = culturefeed_social_activity_get_config(CultureFeed_Activity::TYPE_RECOMMEND);
  $content_type = culturefeed_get_content_type($object->getType());

  $query = new CultureFeed_SearchActivitiesQuery();
  $query->type = CultureFeed_Activity::TYPE_RECOMMEND;
  $query->contentType = $content_type;

  $cf_user_id = DrupalCultureFeed::getLoggedInUserId();

  switch ($content_type) {

    case CultureFeed_Activity::CONTENT_TYPE_BOOK:
      $node_title = $object->getTitle($GLOBALS['language']->language);
      $id = $object->getId();
      $query->nodeId = url(culturefeed_social_detail_path($content_type, $id, $node_title), array('absolute' => TRUE));
      break;

    case CultureFeed_Activity::CONTENT_TYPE_EVENT:
      $node_title = $object->getTitle($GLOBALS['language']->language);
      $id = $object->getId();
      $query->nodeId = $id;
      break;
  }

  $activities = DrupalCultureFeed::searchActivities($query);

  $content = array();

  if ($activities->total > 0) {

    $content['times'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="recommended-times">' . $activities->total . ' x</div>',
    );

    $user_objects = array();
    foreach ($activities->objects as $activity_object) {
      $user_object = new stdClass();
      $user_object->nick = $activity_object->nick;
      $user_object->id = $activity_object->userId;
      $user_object->depiction = $activity_object->depiction;
      $user_objects[] = $user_object;
    }

    if (count($user_objects) > 0) {

      $items = _culturefeed_ui_users_list($user_objects);

      $content['list'] = array(
        '#theme' => 'item_list',
        '#items' => $items,
        '#attributes' => array('class' => 'user-list'),
      );

    }

  }

  // Advanced recommend link.
  $content['recommend-advanced'] = array(
    '#type' => 'markup',
    '#markup' => culturefeed_social_activity_recommend_link_advanced($object),
  );

  $block['subject'] = $config->subject;
  $block['content'] = $content;

  return $block;

}

/**
 * Block callback to render the comments on an item.
 * @param misc $object
 * @return array
 **/
function culturefeed_social_block_comments($object) {

  if (!$object || !method_exists($object, 'getType')) {
    return;
  }

  $config = culturefeed_social_activity_get_config(CultureFeed_Activity::TYPE_COMMENT);
  $content_type = culturefeed_get_content_type($object->getType());

  $query = new CultureFeed_SearchActivitiesQuery();
  $query->type = CultureFeed_Activity::TYPE_COMMENT;
  $query->contentType = $content_type;
  $query->nodeId = culturefeed_social_get_activity_node_id($content_type, $object);

  $cf_user_id = DrupalCultureFeed::getLoggedInUserId();

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

  $block['subject'] = 'Beoordelingen';
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
  $block['subject'] = 'Mijn meldingen';
  $block['content'] = array('#markup' => '<div class="no-results">U hebt geen meldingen</div>');

  try {

    $params = array(
      'max' => CULTUREFEED_SOCIAL_BLOCK_NOTIFICATIONS_COUNT,
      'dateFrom' => date('Y-m-d', strtotime('-1 year')),
    );

    $notifications = DrupalCultureFeed::getNotifications(DrupalCultureFeed::getLoggedInUserId(), $params);
    if ($notifications->total > 0) {
      $block['content'] = array(
        '#theme' => 'culturefeed_social_user_activity_list',
        '#activities' => $notifications,
        '#class' => 'notifications',
        '#read_more_url' => url('culturefeed/notifications'),
        '#read_more_text' => 'Bekijk alle meldingen',
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
  foreach ($active_types as $type => $activityConfig) {
    $allowed_types = $activityConfig->allowedTypes;
    foreach ($allowed_types as $ctype) {
      // @todo remove
      if ($ctype == 'book') continue;
      if ($ctype == 'page' && !$has_pages) {
        continue;
      }
      if ($ctype == 'book' && !$has_books) {
        continue;
      }
      $types[] = $type . '_' . $ctype;
    }
  }

  $private = $account->culturefeed_uid == $GLOBALS['user']->uid ? TRUE : FALSE;

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
    );
  }

  $block['subject'] = '';
  $block['content'] = $content;

  return $block;

}
