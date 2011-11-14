<?php

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

  $events = culturefeed_get_nodes_for_activities($activities->objects);
  
  $accounts = culturefeed_get_uids_for_activities($activities->objects);

  foreach ($activities->objects as $activity) {
    if (!isset($accounts[$activity->userId])) {
      continue;
    }
    
    if ($activity->contentType == CultureFeed_Activity::CONTENT_TYPE_PAGE) {
      $node = node_load($activity->nodeId); // @todo use node_load_multiple
    }
    else {
      if (!isset($events[$activity->nodeId])) {
        continue;
      }
      else {
        $node = $events[$activity->nodeId];
      }
    }

    $items[] = theme('culturefeed_ui_activity_summary', array('activity' => $activity, 'node' => $node, 'uid' => $accounts[$activity->userId]));
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