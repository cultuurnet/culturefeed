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
 */
function culturefeed_social_get_activity_filter_options($active_only = FALSE) {

  $options = array(
    CultureFeed_Activity::TYPE_LIKE => 'Likes',
    CultureFeed_Activity::TYPE_RECOMMEND => 'Aanraden',
    CultureFeed_Activity::TYPE_FOLLOW => 'Pagina volgen',
    CultureFeed_Activity::TYPE_COMMENT => 'Beoordelingen',
  );

  // Return all possible filters.
  if (!$active_only) {
    return $options;
  }

  // Only active filters
  $activity_filters = variable_get('culturefeed_social_activity_filters', array());
  // If no settings yet, return all.
  if (!$activity_filters) {
    return $options;
  }

  $active_options = array();
  foreach ($activity_filters as $activity_filter) {
    if ($activity_filter) {
      $active_options[$activity_filter] = $options[$activity_filter];
    }
  }

  return $active_options;

}

/**
 * Extracted from culturefeed_ui_block_users_activity_view, should be made more generic.
 *
 * @see culturefeed_social_activity_recommend_link()
 */
function culturefeed_social_activity_recommend_link_advanced($extendedEntity) {
  return culturefeed_social_activity_recommend_link($extendedEntity, TRUE);
}

/**
 * Extracted from culturefeed_ui_block_users_activity_view, should be made more generic.
 *
 * @param $activityStatsExtendedEntity
 *   The extended entity of the content to create a link for.
 * @param Boolean $advanced
 *   Indicates whether the link is advanced (with extra possibilities).
 * @return string
 * @todo Try to  make it work for all activity types.
 */
function culturefeed_social_activity_recommend_link($extendedEntity, $advanced = FALSE) {

  $itemEntity = $extendedEntity->getEntity();
  $id = $extendedEntity->getId();
  $current_query = drupal_get_query_parameters();
  $path = current_path();

  $identifier = drupal_html_id('activity-element');
  $identifier_wrapper = drupal_html_id('activity-wrapper');

  $config = culturefeed_social_activity_get_config(CultureFeed_Activity::TYPE_RECOMMEND);
  $activity_content_type = culturefeed_get_content_type($extendedEntity->getType());

  $user_did_this = FALSE;
  if (DrupalCultureFeed::isCultureFeedUser()) {
    try {
      $activity_node_id = culturefeed_social_get_activity_node_id($activity_content_type, $extendedEntity);
      $user_did_this = DrupalCultureFeed::userDidActivity(CultureFeed_Activity::TYPE_RECOMMEND, $activity_node_id, $activity_content_type);
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_social', $e);
    }
  }

  $action = $user_did_this ? 'undo' : 'do';

  if ($advanced) {
    $action_url = 'culturefeed/' . $action . '/' . $config->type . '/' . $activity_content_type . '/' . urlencode($id) . '/advanced';
  }
  else {
    $action_url = 'culturefeed/' . $action . '/' . $config->type . '/' . $activity_content_type . '/' . urlencode($id) . '/noredirect';
  }

  $markup = array();

  $markup[$identifier] = array(
    '#id' => $identifier,
    '#type' => 'link',
    '#href' => $action_url,
    '#attributes' => array(
      'class' => array($action . '-link', $action . '-' . $config->type . '-link'),
    ),
    '#query' => array(
      'destination' => url($path, array('query' => $current_query)),
    ),
    '#prefix' => '<div id="' . $identifier_wrapper . '" class="' . $action . '-link-wrapper">',
    '#suffix' => '</div>',
    '#ajax' => array(
      'wrapper' => $identifier_wrapper,
    ),
  );

  if (!$user_did_this) {

    $markup[$identifier]['#title'] = (0 == $extendedEntity->getActivityCount('recommend')) ? $config->titleDoFirst : $config->titleDo;

    if (user_is_anonymous()) {
      $markup['anon'] = array(
        '#type' => 'markup',
        '#markup' => theme('culturefeed_ui_connect_hover', array('url' => 'culturefeed/do/' . $config->type . '/' . $activity_content_type . '/' . urlencode($id)))
      );
    }
  }
  else {
    $markup[$identifier]['#title'] = $config->subjectUndo;
  }

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

      // @todo: this cant be right.
      $object = culturefeed_pages_page_load($id);
      $activity->nodeTitle = $object->getEntity()->getDetails()->getDetailByLanguage($GLOBALS['language']->language)->getTitle();
      $activity->nodeId = culturefeed_search_detail_url('page', $id, $object->getName(), array('absolute' => TRUE));
      $activity->path = culturefeed_search_detail_path('page', $id, $object->getName());

      break;

    // Normal Cdb items.
    case $activity::CONTENT_TYPE_ACTOR:
    case $activity::CONTENT_TYPE_EVENT:
    case $activity::CONTENT_TYPE_PRODUCTION:

      $object = culturefeed_search_item_load($id);
      $activity->nodeTitle = $object->getEntity()->getDetails()->getDetailByLanguage($GLOBALS['language']->language)->getTitle();
      $activity->nodeId = $id;
      $activity->path = culturefeed_search_detail_path($object->getType(), $id, $activity->nodeTitle);

      break;

      // Please add your new types here.
      default:
        return FALSE;

  }
dsm($activity);
  try {
    $id = DrupalCultureFeed::createActivity($activity);
    $activity->id = $id;
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
 * @return misc boolean false / CultureFeed_Activity on success
 */
function _culturefeed_social_undo($activity_type, $type, $id) {

  $user_id = DrupalCultureFeed::getLoggedInUserId();

  try {
    DrupalCultureFeed::deleteActivities($user_id, $node_id, $type, $activity_type);

    return TRUE;
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_social', $e);

    return FALSE;
  }
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
    return 'node/' . $node_id;
  }
  if ($type == CultureFeed_Activity::CONTENT_TYPE_BOOK) {
    return 'book/' . $id;
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

  if ($type == CultureFeed_Activity::CONTENT_TYPE_NODE || $type == CultureFeed_Activity::CONTENT_TYPE_BOOK) {
    return $id;
  }
  else {
    return culturefeed_search_detail_path($type, $id, $title);
  }

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
 * @param ActivityStatsExtendedEntity $extendedEntity
 */
function culturefeed_social_get_activity_node_id($activity_content_type, $extendedEntity) {

  switch ($activity_content_type) {

    case CultureFeed_Activity::CONTENT_TYPE_ACTOR:
    case CultureFeed_Activity::CONTENT_TYPE_PRODUCTION:
    case CultureFeed_Activity::CONTENT_TYPE_EVENT:
      $nodeId = $extendedEntity->getId();
      break;

      case CultureFeed_Activity::CONTENT_TYPE_BOOK:
        $path = culturefeed_social_detail_path($activity_content_type, $extendedEntity->getId(), $extendedEntity->getTitle($GLOBALS['language']->language));
        $nodeId = url($path, array('absolute' => TRUE));
        break;

    case CultureFeed_Activity::CONTENT_TYPE_NODE:
      $nodeId = url('node/' . $extendedEntity->getId(), array('absolute' => TRUE));
      break;

    case CultureFeed_Activity::CONTENT_TYPE_CULTUREFEED_PAGE:
      $nodeId = url(culturefeed_social_detail_path('page', $extendedEntity->getId(), $object->getName()), array('absolute' => TRUE));
      break;
  }

  return $nodeId;

}

/**
 * Helper function to create a list of activities of the type COMMENT
 * 
 * @param misc $object (Culturefeed_Page, Bibnet_Book, ...
 * @param CultureFeed_ResultSet $activities
 */
function culturefeed_social_recommendation_list($object, CultureFeed_ResultSet $activities) {

  if ($activities->total < 0) {
    return array();
  }
  
  $activity_list = array();
  $total = 0;
  foreach ($activities->objects as $activity) {
    if (empty($activity->parentActivity)) {
      $activity_list[$activity->id] = $activity;
      $total++;
    }
  }

  foreach ($activities->objects as $activity) {
    if (!empty($activity->parentActivity) && isset($activity_list[$activity->parentActivity])) {
      $activity_list[$activity->parentActivity]->childActivities[$activity->id] = $activity;
    }
  }
  
  $accounts = culturefeed_get_uids_for_activities($activities->objects);

  return array(
    '#type' => 'markup',
    '#markup' => theme('culturefeed_social_recommendation_list', array(
      'object' => $object, 
      'activities' => $activity_list, 
      'total' => $total,
      'accounts' => $accounts,
    )),
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'culturefeed_social') . '/js/culturefeed-social.js'),
    )
  );
  
}

