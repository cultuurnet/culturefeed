<?php

/**
 * Helper functions for calendar.
 */

/**
 * Get the users activities.
 */
function culturefeed_calendar_get_user_activities() {

  // Only search for activities with type_like or type_ik_ga.
  $activity_options = array(
    CultureFeed_Activity::TYPE_LIKE,
    CultureFeed_Activity::TYPE_IK_GA,
  );

  // Load the config for the activity options.
  $options = array();
  foreach ($activity_options as $activity_type) {
    $options[$activity_type] = CultureFeedActivityConfigBase::loadByType($activity_type);
  }

  $query = new CultureFeed_SearchActivitiesQuery();
  //$query->max = 500;
  $query->type = $activity_options;
  $query->contentType = 'event';
  $query->userId = DrupalCultureFeed::getLoggedInUserId();

  try {
    $activities = DrupalCultureFeed::searchActivities($query);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_calendar', $e);
    return;
  }

  if ($activities->total == 0) {
    return;
  }

  return $activities;
}

/**
 * Render the button to go to the calendar page or add item form.
 */
function culturefeed_calendar_render_button($item, $location) {

  $id = $item->getId();
  $ids_to_check = array();
  $activities = culturefeed_calendar_get_user_activities();
  foreach($activities->objects as $key => $activity) {
    $ids_to_check[] = $activity->nodeId;
  }

  if (in_array($id, $ids_to_check)) {
    $button['action'] = 'view';
  }
  else {
    $button['action'] = 'add';
  }

  $button['location'] = $location;
  $button['item_id'] = $id;

  return theme('culturefeed_calendar_button', array('button' => $button));
}