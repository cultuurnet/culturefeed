<?php


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