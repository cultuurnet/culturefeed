<?php
/**
 * @file
 * Helper functions for culturefeed social.
 */

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