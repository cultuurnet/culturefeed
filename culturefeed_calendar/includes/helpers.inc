<?php

/**
 * Helper functions for calendar.
 */

/**
 * Get the users activities.
 */
function culturefeed_calendar_get_user_activities() {

  $uid = DrupalCultureFeed::getLoggedInUserId();

  if ($uid) {
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

    /*$service = culturefeed_get_search_service();
    $parameters = array();
    $parameters[] = new CultuurNet\Search\Parameter\Query('*:*');
    $parameters[] = new CultuurNet\Search\Parameter\FilterQuery('type:event');
    $parameters[] = new CultuurNet\Search\Parameter\FilterQuery('like_users:' . $uid);
    $parameters[] = new CultuurNet\Search\Parameter\FilterQuery('attend_users:' . $uid);

    $result = $service->search($parameters);
    */

    $query = new CultureFeed_SearchActivitiesQuery();
    $query->max = 500;
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
  }
  else {
    $activities = new stdClass();
    $activities->objects = array();
    $activities->total = 0;

    if (isset($_COOKIE['Drupal_visitor_calendar'])) {
      $activities->objects = unserialize($_COOKIE['Drupal_visitor_calendar']);
      $activities->total = count($activities->objects);
    }
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

/**
 * Helper function to add an activity to the calendar.
 */
function culturefeed_calendar_add_activity($node_id, $node_title, $date) {

  $authenticated = DrupalCultureFeed::isCultureFeedUser();
  $activity = new CultureFeed_Activity();

  if (!empty($date)) {
    $activity->value = strtotime($date);
    $activity->type = CultureFeed_Activity::TYPE_IK_GA;
  }
  else {
    $activity->type = CultureFeed_Activity::TYPE_LIKE;
  }

  $activity->contentType = 'event';
  $activity->nodeId = $node_id;
  $activity->nodeTitle = $node_title;

  if ($authenticated) {
    $activity->userId = DrupalCultureFeed::getLoggedInUserId();

    DrupalCultureFeed::createActivity($activity);
  }
  else {
    $calendar = array();

    if (isset($_COOKIE['Drupal_visitor_calendar'] )) {
      $calendar = unserialize($_COOKIE['Drupal_visitor_calendar']);
    }

    $calendar[] = $activity;
    end($calendar);
    $last_key = key($calendar);
    $calendar[$last_key]->id = $last_key;

    $values = array(
      'calendar' => serialize($calendar),
    );

    user_cookie_save($values);

  }

}

/**
 * Helper function to return the total unread activities (stored in cookie).
 */
function culturefeed_calendar_get_total_unread_activities() {
  $activities_count = 0;
  $authenticated = DrupalCultureFeed::isCultureFeedUser();

  if (!$authenticated) {
    $activities = culturefeed_calendar_get_user_activities();
    $activities_count = $activities->total;
  }

  return $activities_count;
}

/**
 * Checks wether an event has multiple dates.
 */
function culturefeed_calendar_event_has_multiple_dates($dates_array) {

  switch ($dates_array['type']) {

    case 'period':
      return ($dates_array['period_start'] == $dates_array['period_end']) ? FALSE : TRUE;

    case 'permanent':
      return TRUE;

    case 'timestamps':
      return (count($dates_array['timestamps']) > 1) ? TRUE : FALSE;

  }

}

/**
 * Updates a calendar event in the cookie for anon users.
 *
 * @param CultureFeed_Activity $activity
 * @param string $date String version of the date to save.
 */
function culturefeed_calendar_update_calendar_event_cookie($activity, $date) {

  if (isset($_COOKIE['Drupal_visitor_calendar'])) {

    $calendar = unserialize($_COOKIE['Drupal_visitor_calendar']);
    if (isset($calendar[$activity->id])) {
      $calendar[$activity->id]->value = strtotime($date);
      $calendar[$activity->id]->type = CultureFeed_Activity::TYPE_IK_GA;
    }
    else {
      return FALSE;
    }

    $values = array(
      'calendar' => serialize($calendar),
    );

    user_cookie_save($values);

    return TRUE;
  }

  return FALSE;

}

/**
 * deletes a calendar event from the cookie for anon users.
 *
 * @param CultureFeed_Activity $activity
 */
function culturefeed_calendar_delete_calendar_event_cookie($activity) {
  if (isset($_COOKIE['Drupal_visitor_calendar'])) {

    $calendar = unserialize($_COOKIE['Drupal_visitor_calendar']);
    if (isset($calendar[$activity->id])) {
      unset($calendar[$activity->id]);
    }
    else {
      return FALSE;
    }

    $values = array(
      'calendar' => serialize($calendar),
    );

    user_cookie_save($values);

    return TRUE;
  }

  return FALSE;
}