<?php
/**
 * @file
 * Helper functions for culturefeed social.
 */

/**
 * Get configuration properties for a action.
 * @param Integer $activity_type
 *   The activity type to get the configuration for.
 *   E.g. CultureFeed_Activity::TYPE_RECOMMEND, CultureFeed_Activity::TYPE_LIKE, ...
 *
 * @return CultureFeedActivityConfigBase
 */
function culturefeed_social_activity_get_config($activity_type) {
  return CultureFeedActivityConfigBase::loadByType($activity_type);
}

/**
 * Get the actvitity filter options.
 * @return Array of possible activity options.
 *   key = activity id
 *   value = activity config
 */
function culturefeed_social_get_activity_filter_options($active_only = FALSE) {

  $types = array();

  // Do a full query to get all available types.
  $query = new CultureFeed_SearchActivitiesQuery();
  $query->userId = DrupalCultureFeed::getLoggedInUserId();
  $query->private = TRUE;
  try {
    $activities = DrupalCultureFeed::searchActivities($query);
    foreach ($activities->objects as $activity) {

      if (!in_array($activity->type, array_keys($types))) {
        $activity_config = CultureFeedActivityConfigBase::loadByType($activity->type);
        if ($activity_config) {
          $types[$activity_config->type] = $activity_config;
        }
      }

    }

  }
  catch (Exception $e) {
    watchdog('culturefeed_social_get_activity_options', $e);
    return array();
  }

  return $types;

}

/**
 * Show an advanced do / undo link for a given action, on a given entity.
 *
 * @see culturefeed_social_activity_link()
 */
function culturefeed_social_activity_link_advanced($activity_type, $extended_entity, $use_ajax = TRUE) {
  return culturefeed_social_activity_link($activity_type, $extended_entity, TRUE, FALSE, '', '', $use_ajax);
}

/**
 * Show a link to do / undo a given activity on the given entity.
 *
 * @param $activity_type
 *   Activity type to perform (Example: CultureFeed_Activity::TYPE_RECOMMEND)
 * @param $activityStatsExtendedEntity
 *   The extended entity of the content to create a link for.
 * @param Boolean $advanced
 *   Indicates whether the link is advanced (with extra possibilities).
 * @param Boolean $add_count
 *   Add the current activity count to the link or not.
 * @param string $title
 *   Title you want to use to override the default config title.
 * @param string $undo_title
 *   Undo title you want to use to override the default config title.
 * @param $use_ajax
 *   Use ajax for the link or not.
 * @return string
 */
function culturefeed_social_activity_link($activity_type, $extended_entity, $advanced = FALSE, $add_count = FALSE, $title = '', $title_undo = '', $use_ajax = TRUE) {

  if (method_exists($extended_entity, 'getId')) {
    $id = $extended_entity->getId();
    $activity_content_type = culturefeed_get_content_type($extended_entity->getType());
    $activity_node_id = culturefeed_social_get_activity_node_id($activity_content_type, $extended_entity);
    $activity_count = $extended_entity->getActivityCount(CultureFeed_Activity::getNameById($activity_type));
  }
  else {
    $id = $extended_entity->id;
    $activity_node_id = $id;
    $activity_content_type = culturefeed_get_content_type(get_class($extended_entity));
    $activity_count = DrupalCultureFeed::activityGetCount($activity_type, $activity_node_id, $activity_content_type);
  }

  $current_query = drupal_get_query_parameters();
  $path = current_path();

  $identifier = drupal_html_id('activity-element-' . $activity_type);
  $identifier_wrapper = drupal_html_id('activity-wrapper-' . $activity_type);

  $config = culturefeed_social_activity_get_config($activity_type);
  $user_did_this = FALSE;

  // Anonymous user: redirect him to landing page.
  if (!DrupalCultureFeed::isCultureFeedUser()) {
    $action = 'do';
    $action_url = 'authenticated';
    $destination = 'culturefeed/do/' . $config->type . '/' . $activity_content_type . '/' . urlencode($id) . '/redirect';
    $use_ajax = FALSE;
  }
  else {

    // If undo is not allowed. The action is always do.
    if ($config->undoAllowed) {
      try {
        $user_did_this = DrupalCultureFeed::userDidActivity($activity_type, $activity_node_id, $activity_content_type);
      }
      catch (Exception $e) {
        watchdog_exception('culturefeed_social', $e);
      }
    }

    $action = $user_did_this ? 'undo' : 'do';

    if (!$use_ajax) {
      $action_url = 'culturefeed/' . $action . '/' . $config->type . '/' . $activity_content_type . '/' . urlencode($id) . '/redirect';
    }
    elseif ($advanced) {
      $action_url = 'culturefeed/' . $action . '/' . $config->type . '/' . $activity_content_type . '/' . urlencode($id) . '/advanced';
    }
    else {
      $action_url = 'culturefeed/' . $action . '/' . $config->type . '/' . $activity_content_type . '/' . urlencode($id) . '/noredirect';
    }

    $destination = url($path, array('query' => $current_query));
  }

  $markup = array();

  $markup[$identifier] = array(
    '#id' => $identifier,
    '#type' => 'link',
    '#href' => $action_url,
    '#attributes' => array(
      'class' => array($action . '-link', $action . '-' . $config->type . '-link'),
    ),
    '#options' => array(
      'query' => array(
        'destination' => $destination,
        'add_count' => $add_count,
      ),
    ),
    '#prefix' => '<div id="' . $identifier_wrapper . '" class="' . $action . '-link-wrapper">',
    '#suffix' => '</div>',
  );

  // If ajax is requested, add ajax action.
  if ($use_ajax) {
    $markup[$identifier]['#ajax'] = array(
      'wrapper' => $identifier_wrapper,
    );
  }

  if (!empty($title)) {
    $markup[$identifier]['#options']['query']['title'] = $title;
    $config->titleDoFirst = $title;
    $config->titleDo = $title;
  }

  if (!empty($title_undo)) {
    $markup[$identifier]['#options']['query']['title_undo'] = $title_undo;
    $config->subjectUndo = $title_undo;
  }

  if (!$user_did_this) {
    $markup[$identifier]['#title'] = (0 == $activity_count) ? $config->titleDoFirst : $config->titleDo;
  }
  else {
    $markup[$identifier]['#title'] = $config->subjectUndo;
  }

  if ($activity_count && $add_count || isset($_GET['add_count'])) {
    $markup[$identifier]['#title'] .= ' (' . $activity_count . ')';
  }

  drupal_alter('culturefeed_social_activity_link', $markup, $activity_type, $extended_entity);

  return drupal_render($markup);
}

/**
 *
 * @param String $activity_type
 * @param String $type
 * @param Integer $id
 * @return misc boolean false / CultureFeed_Activity on success
 */
function _culturefeed_social_do($activity_type, $type, $id) {

  $activity = new CultureFeed_Activity();
  $activity->contentType = $type;
  $activity->type = $activity_type;
  $activity->userId = DrupalCultureFeed::getLoggedInUserId();

  if (module_exists('culturefeed_pages')) {
    $active_page = culturefeed_pages_get_active_page_id();
    if ($active_page) {
      $activity->onBehalfOf = $active_page;
    }
  }

  switch ($type) {

    // Drupal nodes.
    case $activity::CONTENT_TYPE_NODE:

      $node = node_load($id);
      $activity->nodeTitle = $node->title;
      $activity->nodeId = url('node/' . $node->nid, array('absolute' => TRUE));
      $activity->path = 'node/' . $node->nid;

      break;

    // Bibnet books.
    case $activity::CONTENT_TYPE_BOOK:

      $object = bibnet_book_load($id);
      $activity->nodeTitle = bibnet_books_detail_page_title($object);
      $activity->nodeId = url($object->getEntity()->getUri(), array('absolute' => TRUE));
      $activity->path = culturefeed_social_detail_path($object->getType(), $id, $activity->nodeTitle);

      break;

    // Culturefeed pages.
    case $activity::CONTENT_TYPE_CULTUREFEED_PAGE:

      $object = culturefeed_pages_page_load($id);
      $activity->nodeTitle = $object->getEntity()->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language())->getTitle();
      $activity->nodeId = culturefeed_search_detail_url('page', $id, $object->getName(), array('absolute' => TRUE));
      $activity->path = culturefeed_search_detail_path('page', $id, $object->getName());

      break;

    // Normal Cdb items.
    case $activity::CONTENT_TYPE_ACTOR:
    case $activity::CONTENT_TYPE_EVENT:
    case $activity::CONTENT_TYPE_PRODUCTION:

      $object = culturefeed_search_item_load($id, $type);
      $activity->nodeTitle = $object->getEntity()->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language())->getTitle();
      $activity->nodeId = $id;
      $activity->path = culturefeed_search_detail_path($object->getType(), $id, $activity->nodeTitle);

      break;

    case $activity::CONTENT_TYPE_ACTIVITY:

      try {
        $query = new CultureFeed_SearchActivitiesQuery();
        $query->activityId = $id;
        $activities = DrupalCultureFeed::searchActivities($query);
        if ($activities->total == 0) {
          return FALSE;
        }

        $activity->nodeTitle = $activities->objects[0]->nodeTitle;
        $activity->nodeId = $id;

      }
      catch (Exception $e) {
        watchdog_exception('culturefeed_social', $e);
        return FALSE;
      }

    break;

    // Generic content.
    case $activity::CONTENT_TYPE_CONTENT:

      $genericObject = culturefeed_userpoints_promotion_load($id);
      $activity->nodeTitle = $genericObject->title;
      $activity->nodeId = url('userpoints/promotion/' . $genericObject->id, array('absolute' => TRUE));
      $activity->path = 'userpoints/promotion/' . $genericObject->id;

      break;

      // Please add your new types here.
      default:
        return FALSE;

  }

  try {
    $activitiy_result = DrupalCultureFeed::createActivity($activity);
    $activity->id = $activitiy_result->id;
    return $activity;
  }
  catch (Exception $e) {
    if ($e->getCode() != 421) {
      watchdog_exception('culturefeed_social', $e);
      return FALSE;
    }
    return TRUE;
  }

}

/**
 *
 * @param String $activity_type
 * @param String $type
 * @param Integer $id
 * @return array Deleted activity results.
 */
function _culturefeed_social_undo($activity_type, $type, $node_id) {

  $user_id = DrupalCultureFeed::getLoggedInUserId();

  try {
    return DrupalCultureFeed::deleteActivities($user_id, $node_id, $type, $activity_type);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_social', $e);
  }
  //return array();
}

/**
 * Helper function to create a path to the item
 * @param String $type
 * @param String $id
 * @param String $title
 * @return string
 */
function culturefeed_social_detail_path($type, $id, $title = '') {

  if ($type == CultureFeed_Activity::CONTENT_TYPE_NODE) {
    return 'node/' . $id;
  }
  if ($type == CultureFeed_Activity::CONTENT_TYPE_CONTENT) {
    return 'userpoints/promotion/' . $id;
  }
  if ($type == CultureFeed_Activity::CONTENT_TYPE_BOOK) {
    return 'book/' . urlencode($id);
  }
  else {
    return culturefeed_search_detail_path($type, $id, $title);
  }

}

/**
 * Helper function to create a path to the detail of an activity object
 * @param String $type
 * @param String $id
 * @param String $title
 * @return string
 */
function culturefeed_social_activity_object_path($type, $id, $title = '') {

  if ($type == CultureFeed_Activity::CONTENT_TYPE_NODE
    || $type == CultureFeed_Activity::CONTENT_TYPE_BOOK
    || $type == CultureFeed_Activity::CONTENT_TYPE_CONTENT) {
    return $id;
  }
  else {
    return culturefeed_search_detail_path($type, $id, $title);
  }

}

/**
 * Helper function to load the correct main object.
 *
 * @param CultureFeed_Activity $activity
 *   The activity to load the main object of.
 */
function culturefeed_social_detail_object(CultureFeed_Activity $activity) {

  $object = NULL;

  switch ($activity->contentType) {

    case CultureFeed_Activity::CONTENT_TYPE_NODE:

      // Get the node url of the full url. We need it to get the nid.
      global $base_url;
      $path = str_replace($base_url . '/', '', $activity->nodeId);
      $node_path = drupal_get_normal_path($path);

      $object = node_load(arg(1, $node_path));
      if (!$object) {
        return NULL;
      }

      return new DrupalNodeExtendedEntity($object);

      break;

    case CultureFeed_Activity::CONTENT_TYPE_BOOK:
      $object = bibnet_book_load(urldecode($activity->nodeId));
      break;

    case CultureFeed_Activity::CONTENT_TYPE_CONTENT:
      $object = culturefeed_userpoints_promotion_load($activity->nodeId);
      break;

    // Activity type. Load the activity to get the original object.
    case CultureFeed_Activity::CONTENT_TYPE_ACTIVITY:
      try {
        $query = new CultureFeed_SearchActivitiesQuery();
        $query->activityId = $activity->nodeId;
        $query->private = FALSE;
        $activities = DrupalCultureFeed::searchActivities($query);
        if ($activities->total > 0) {
          $parent_activity = $activities->objects[0];
          $object = culturefeed_social_detail_object($parent_activity);
        }
      }
      catch (Exception $e) {
        watchdog_exception('culturefeed_social', $e);
      }
      break;

    case CultureFeed_Activity::CONTENT_TYPE_EVENT:
    case CultureFeed_Activity::CONTENT_TYPE_ACTOR:
    case CultureFeed_Activity::CONTENT_TYPE_PRODUCTION:
    default:
      $object = culturefeed_search_item_load($activity->nodeId, $activity->contentType);
      break;

  }

  return $object;

}

/**
 * Helper function to create a drupal goto based on activities.
 * @param Culturefeed_Activity $activity
 * @param string $content_type
 * @param string $node_id
 * @param string $title
 */
function culturefeed_social_goto($activity, $content_type, $node_id, $title = "redirect") {
  drupal_goto(culturefeed_social_detail_path($content_type, $node_id, $title));
}

/**
 * Helper to get the nodeId used for activity.
 * @param String $activity_content_type
 * @param ActivityStatsExtendedEntity $extended_entity
 */
function culturefeed_social_get_activity_node_id($activity_content_type, $extended_entity) {

  $node_id = 0;
  switch ($activity_content_type) {

    case CultureFeed_Activity::CONTENT_TYPE_ACTOR:
    case CultureFeed_Activity::CONTENT_TYPE_PRODUCTION:
    case CultureFeed_Activity::CONTENT_TYPE_EVENT:
      $node_id = $extended_entity->getId();
    break;

    case CultureFeed_Activity::CONTENT_TYPE_BOOK:
      $path = culturefeed_social_detail_path($activity_content_type, $extended_entity->getId(), $extended_entity->getTitle(culturefeed_search_get_preferred_language()));
      $node_id = url($path, array('absolute' => TRUE));
    break;

    case CultureFeed_Activity::CONTENT_TYPE_CONTENT:
      $path = culturefeed_social_detail_path($activity_content_type, $extended_entity->getId(), $extended_entity->getTitle(culturefeed_search_get_preferred_language()));
      $node_id = url($path, array('absolute' => TRUE));
    break;

    case CultureFeed_Activity::CONTENT_TYPE_NODE:
      $node_id = url('node/' . $extended_entity->getId(), array('absolute' => TRUE));
      break;

    case CultureFeed_Activity::CONTENT_TYPE_CULTUREFEED_PAGE:
      $node_id = url(culturefeed_social_detail_path('page', $extended_entity->getId(), $object->getName()), array('absolute' => TRUE));
    break;

  }

  return $node_id;

}

/**
 * Get the total activity count for the given content.
 * @param $activity_type
 *   Type of activity to get the count for.
 * @param $content_type
 *   Type of content to get the count for.
 * @param mixed $node_ids
 *   Node ID's to get the count for. Can be array or string.
 */
function culturefeed_social_get_activity_count($activity_type, $content_type, $node_ids) {

  try {
    $query = new CultureFeed_SearchActivitiesQuery();
    $query->contentType = $content_type;
    $query->nodeId = $node_ids;
    $query->type = $activity_type;
    $activities = DrupalCultureFeed::searchActivities($query);

    return $activities->total;
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_social', $e);
  }

  return 0;

}

/**
 * Get a list of themed activity summary's.
 */
function culturefeed_social_activities_list($activities) {
  $items = array();

  $accounts = culturefeed_get_uids_for_activities($activities->objects);

  foreach ($activities->objects as $activity) {
    if (!isset($accounts[$activity->userId])) {
      continue;
    }

    $items[] = theme('culturefeed_ui_activity_summary', array('activity' => $activity, 'uid' => $accounts[$activity->userId]));
  }

  return $items;
}

/**
 * Helper function to create a list of activities of the type COMMENT
 *
 * @param misc $object (Culturefeed_Page, Bibnet_Book, ...
 * @param CultureFeed_ResultSet $activities
 */
function culturefeed_social_comment_list($object, CultureFeed_ResultSet $activities) {

  if ($activities->total < 0) {
    return array();
  }

  $activity_list = array();
  $child_activities = array();
  $total = 0;
  foreach ($activities->objects as $activity) {
    if ($activity->contentType != CultureFeed_Activity::CONTENT_TYPE_ACTIVITY) {
      $activity_list[$activity->id] = $activity;
      $total++;
    }
    else {
      $child_activities[] = $activity;
    }
  }

  // Most recent activities are on top in result object. But for comments, it should be on bottom.
  $activity_list = array_reverse($activity_list);
  $child_activities = array_reverse($child_activities);

  // Place child activities under the parent.
  foreach ($child_activities as $activity) {
    if (isset($activity_list[$activity->nodeId])) {
      $activity_list[$activity->nodeId]->childActivities[$activity->id] = $activity;
    }
  }

  $accounts = culturefeed_get_uids_for_activities($activities->objects);

  return array(
    '#theme' => 'culturefeed_social_comment_list',
    '#object' => $object,
    '#activities' => $activity_list,
    '#total' => $total,
    '#accounts' => $accounts,
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'culturefeed_social') . '/js/culturefeed-social.js'),
    )
  );

}

/**
 * Create a form or login link.
 * @param misc $object
 * @param CultureFeed_Activity $activity
 */
function culturefeed_social_comment_actionbox($object, $activity = NULL) {

  if (culturefeed_is_culturefeed_user()) {
    $form_id = ($activity) ? 'culturefeed_social_comment_form_' . $activity->id : 'culturefeed_social_comment_form';
    return drupal_get_form($form_id, $object, $activity);
  }
  else {
    return array(
      '#theme' => 'culturefeed_social_login_required_message',
      '#activity_type' => CultureFeed_Activity::TYPE_COMMENT,
    );
  }

}

/**
 * Prepare all the social activity stats for this user.
 */
function culturefeed_social_warmup_activities_cache($items) {

  // Do an activity search on all found nodeIds.
  $nodeIds = array();
  foreach ($items as $item) {
    $activity_content_type = culturefeed_get_content_type($item->getType());
    $nodeIds[] = culturefeed_social_get_activity_node_id($activity_content_type, $item);
  }

  $userDidActivity = &drupal_static('userDidActivity', array());

  // Get a list of all activities from this user, on the content to show.
  $userActivities = array();
  try {

    $userId = DrupalCultureFeed::getLoggedInUserId();

    $query = new CultureFeed_SearchActivitiesQuery();
    $query->nodeId = $nodeIds;
    $query->userId = $userId;

    if ($userId) {
      $query->private = TRUE;
    }

    $activities = DrupalCultureFeed::searchActivities($query);
    foreach ($activities->objects as $activity) {
      $userActivities[$activity->nodeId][$activity->contentType][] = $activity;
    }
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_search_ui', $e);
  }

  // Fill up cache for following content types.
  $contentTypes = array(
    CultureFeed_Activity::CONTENT_TYPE_EVENT,
    CultureFeed_Activity::CONTENT_TYPE_PRODUCTION,
  );

  // Fill up the $userDidActivity variable. This is used in DrupalCulturefeed::userDidActivity().
  foreach ($nodeIds as $nodeId) {
    foreach ($contentTypes as $contentType) {
      // If user did this activitiy. Place it in the correct array.
      if (isset($userActivities[$nodeId][$contentType])) {
        $activities = new CultureFeed_ResultSet(count($userActivities[$nodeId][$contentType]), $userActivities[$nodeId][$contentType]);
      }
      // Otherwise create an empty result set.
      else {
        $activities = new CultureFeed_ResultSet(0, array());
      }
      $userDidActivity[$nodeId][$contentType][$userId] = $activities;
    }
  }
}
