<?php

function culturefeed_ui_activity_types() {
  return array(
    CultureFeed_Activity::TYPE_LIKE     => 'Vind ik leuk',
    CultureFeed_Activity::TYPE_IK_GA    => 'Ga hier naartoe',
    CultureFeed_Activity::TYPE_FACEBOOK => 'Gedeeld op Facebook',
    CultureFeed_Activity::TYPE_TWITTER  => 'Gedeeld op Twitter',
  );
}

function culturefeed_ui_activity_context($cnapi_types = array(), $node_types = array()) {
  $node = menu_get_object();
  $request = cnapi_ui_get_active_request();

  if ($request) {
    $type = reset(array_intersect($cnapi_types, array_keys($request)));
    if ($type) {
      return array(
        'node_id' => $request[$type],
        'activity_content_type' => culturefeed_activity_content_type_from_cnapi_type($type),
      );
    }
  }
  elseif ($node) {
    if (in_array($node->type, $node_types)) {
      return array(
        'node_id' => $node->nid,
        'activity_content_type' => culturefeed_activity_content_type_from_cnapi_type($node->type),
      );
    }
  }
  
  return FALSE;
}

function culturefeed_ui_recommendations_list($total, $zip = NULL) {
  $query = new CultureFeed_RecommendationsQuery();
  $query->max = $total;

  if ($zip) {
    $query->zip = $zip;
  }

  try {
    $recommendations = DrupalCultureFeed::getRecommendationsForUser(DrupalCultureFeed::getLoggedInUserId(), $query);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    return FALSE;
  }

  $items = array();

  foreach ($recommendations as $recommendation) {
    $id = $recommendation->id;
    $cdbid = $recommendation->itemid;

    $positive = array(
      '#type' => 'link',
      '#title' => 'Vind ik leuk',
      '#href' => 'culturefeed/recommendations/positive/' . $id . '/' . $cdbid . '/nojs',
      '#attributes' => array('id' => 'recommendation-positive-' . $id, 'class' => 'like-link'),
      '#options' => array('query' => drupal_get_destination()),
      '#ajax' => array(
        'wrapper' => 'recommendation-' . $id,
      ),
    );

    $negative = array(
      '#type' => 'link',
      '#title' => 'Verwijderen',
      '#href' => 'culturefeed/recommendations/negative/' . $id . '/' . $cdbid . '/nojs',
      '#attributes' => array('id' => 'recommendation-negative-' . $id, 'class' => 'remove-link'),
      '#options' => array('query' => drupal_get_destination()),
      '#ajax' => array(
        'wrapper' => 'recommendation-' . $id,
      ),
    );

    $links = array('like' => drupal_render($positive), 'remove' => drupal_render($negative));
    $item = theme('culturefeed_ui_event_recommendation', array('event' => $recommendation, 'links' => $links, 'heading_level' => 3));

    $items[] = array(
      'data' => $item,
      'id' => 'recommendation-' . $id,
    );
  }

  return $items;
}

function _culturefeed_ui_undo($activity_type, $content_type, $node_id) {
  $user_id = DrupalCultureFeed::getLoggedInUserId();
  $content_type = culturefeed_activity_content_type_from_cnapi_type($content_type);

  $error = FALSE;

  try {
    DrupalCultureFeed::deleteActivities($user_id, $node_id, $content_type, $activity_type);
  }
  catch (Exception $e) {
    $error = TRUE;
    watchdog_exception('culturefeed_ui', $e);
  }

  cache_clear_all('*', 'cache_page', TRUE);

  return $error;
}

function _culturefeed_ui_do($activity_type, $type, $id) {
  $activity = new CultureFeed_Activity();
  $activity->contentType = culturefeed_activity_content_type_from_cnapi_type($type);
  $activity->type = $activity_type;
  $activity->nodeId = $id;
  $activity->userId = DrupalCultureFeed::getLoggedInUserId();

  if ($type == 'page') {
    $node = node_load($id);
    $activity->nodeTitle = $node->title;
  }
  else {
    $object = cnapi_get(array('action' => 'detail', 'type' => $type, 'query' => array('cdbid' => $id)));
    $activity->nodeTitle = $object['detail']['nl']['title'];
  }

  $error = FALSE;

  try {
    DrupalCultureFeed::createActivity($activity);
  }
  catch (Exception $e) {
    if ($e->getCode() != 421) {
      $error = TRUE;
      watchdog_exception('culturefeed_ui', $e);
    }
  }

  cache_clear_all('*', 'cache_page', TRUE);

  return $error;
}

function culturefeed_ui_activity_based_goto($content_type, $node_id) {
  if ($content_type == 'page') {
    drupal_goto('node/' . $node_id);
  }
  else {
    $object = cnapi_get(array('action' => 'detail', 'type' => $content_type, 'query' => array('cdbid' => $node_id)));

    $request = array($content_type => $node_id, 'title' => $object['detail']['nl']['title']);

    cnapi_ui_goto($request);
  }
}

function culturefeed_ui_activities_list($activities) {
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

function culturefeed_ui_users_list($users) {
  $items = array();

  $accounts = culturefeed_get_uids_for_users($users->objects);

  foreach ($users->objects as $user) {
    if (!isset($accounts[$user->id])) {
      continue;
    }

    $items[] = theme('culturefeed_ui_user_summary', array('user' => $user, 'uid' => $accounts[$user->id]));
  }

  return $items;
}