<?php
/**
 * @file
 * Block callbacks for the social module.
 */

function culturefeed_social_block_user_activities($account) {

  if (empty($account->culturefeed_uid)) {
    return;
  }

  $query = new CultureFeed_SearchActivitiesQuery();
  $query->max = CULTUREFEED_SOCIAL_BLOCK_USER_ACTIVITIES_COUNT;
  $query->type = array(
    CultureFeed_Activity::TYPE_LIKE,
    CultureFeed_Activity::TYPE_FACEBOOK,
    CultureFeed_Activity::TYPE_TWITTER,
    CultureFeed_Activity::TYPE_IK_GA,
  );
  $query->userId = $account->culturefeed_uid;
  $query->private = FALSE;

  try {
    $activities = DrupalCultureFeed::searchActivities($query);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    return;
  }

  if (empty($activities->objects)) {
    $content['activities'] = array(
      '#markup' => 'Deze gebruiker heeft nog geen acties gedaan.',
    );
  }
  else {


    $activity_filters = culturefeed_social_get_activity_filter_options(TRUE);
    if (!empty($activity_filters)) {
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
      $content['activities']['#pager_path'] = url('culturefeed/ajax/user/activities', array('query' => array('culturefeed_uid' => $account->culturefeed_uid)));
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

  if (!$object) {
    return;
  }

  $config = culturefeed_social_activity_get_config(CultureFeed_Activity::TYPE_RECOMMEND);
  $content_type = culturefeed_get_content_type($object->getType());

  $query = new CultureFeed_SearchActivitiesQuery();
  $query->type = CultureFeed_Activity::TYPE_RECOMMEND;
  $query->contentType = $content_type;

  $cf_user_id = DrupalCultureFeed::getLoggedInUserId();
  //$query->private = TRUE;

  switch ($content_type) {

    case CultureFeed_Activity::CONTENT_TYPE_BOOK:
      $node_title = $object->getTitle($GLOBALS['language']->language);
      $id = $object->getId();
      $query->nodeId = url(culturefeed_social_detail_path($object->getType(), $id, $node_title), array('absolute' => TRUE));
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
