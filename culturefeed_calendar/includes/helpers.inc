<?php

/**
 * Helper functions for calendar.
 */

/**
 * Get the users activities.
 */
function culturefeed_calendar_get_user_activities($use_search = FALSE, $user_id = '') {

  $activities = &drupal_static(__FUNCTION__, NULL);

  if ($activities != NULL) {
    return $activities;
  }

  $activities = new stdClass();
  $activities->objects = array();
  $activities->total = 0;

  $uid = !empty($user_id) ? $user_id : DrupalCultureFeed::getLoggedInUserId();

  if ($uid) {

    // Only search for activities with type_like or type_ik_ga.
    $activity_options = array(
      CultureFeed_Activity::TYPE_LIKE,
      CultureFeed_Activity::TYPE_IK_GA,
    );

    $query = new CultureFeed_SearchActivitiesQuery();
    $query->max = 500;
    $query->type = $activity_options;
    $query->contentType = 'event';
    $query->userId = $uid;

    // Search on the service for attends or likes of this user.
    if ($use_search) {

      $service = culturefeed_get_search_service();
      $parameters = array();
      $parameters[] = new CultuurNet\Search\Parameter\Query('*:*');
      $parameters[] = new CultuurNet\Search\Parameter\Group();
      $parameters[] = new CultuurNet\Search\Parameter\Rows(500);
      $parameters[] = new CultuurNet\Search\Parameter\FilterQuery('type:event');
      $parameters[] = new CultuurNet\Search\Parameter\FilterQuery('attend_users:' . $uid . ' OR like_users:' . $uid);

      $result = $service->search($parameters);
      // No likes / attends for this user on future events, don't do an activity search.
      if ($result->getTotalCount() == 0) {
        return $activities;
      }

      $items = $result->getItems();
      $content_ids = array();
      foreach ($items as $item) {
        $content_ids[] = $item->getId();
      }

      // Only search on found events. This way, we only see events in the future.
      $query->nodeId = $content_ids;

    }

    try {
      $activities = DrupalCultureFeed::searchActivities($query);
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_calendar', $e);
    }

  }
  else {

    if (isset($_COOKIE['Drupal_visitor_calendar'])) {
      $activities->objects = json_decode($_COOKIE['Drupal_visitor_calendar']);
      $activities->total = count($activities->objects);
    }
  }

  return $activities;
}

/**
 * Render the buttons to go to the calendar page or add item form.
 */
function culturefeed_calendar_render_calendar_buttons($item) {

  $authenticated = DrupalCultureFeed::isCultureFeedUser();
  $id = $item->getId();
  $locations = array('content', 'footer');
  $actions = array('view', 'add');
  $buttons = array();
  $button = array();

  $button['item_id'] = $id;

  foreach ($locations as $location) {

    $button['location'] = $location;

    // Show the correct button for authenticated users.
    if ($authenticated) {

      $ids_to_check = array();
      $activities = culturefeed_calendar_get_user_activities();
      if (isset($activities->objects)) {
        foreach($activities->objects as $key => $activity) {
          $ids_to_check[] = $activity->nodeId;
        }
      }

      if (in_array($id, $ids_to_check)) {
        $action = 'view';
      }
      else {
        $action = 'add';
      }

      $button['action'] = $action;
      $buttons[$location][$action] = theme('culturefeed_calendar_button', array('button' => $button, 'authenticated' => $authenticated));
    }
    // Show both buttons for anonymous users. JS will show the correct one.
    else {
      foreach ($actions as $action) {
        $button['action'] = $action;
        $buttons[$location][$action] = theme('culturefeed_calendar_button', array('button' => $button, 'authenticated' => $authenticated));
      }
    }
  }
  return $buttons;
}

/**
 * Helper function to add an activity to the calendar.
 */
function culturefeed_calendar_add_activity($node_id, $node_title, $date) {

  $authenticated = DrupalCultureFeed::isCultureFeedUser();

  if ($authenticated) {
    $activity = new CultureFeed_Activity();
    if (!empty($date)) {
      $selection = new stdClass();
      $selection->calendar_date = strtotime($date);
      $activity->value = json_encode($selection);
      $activity->type = CultureFeed_Activity::TYPE_IK_GA;
    }
    else {
      $activity->type = CultureFeed_Activity::TYPE_LIKE;
    }

    $activity->contentType = 'event';
    $activity->nodeId = $node_id;
    $activity->nodeTitle = $node_title;
    $activity->userId = DrupalCultureFeed::getLoggedInUserId();

    DrupalCultureFeed::createActivity($activity);
  }
  else {
    $activity = new stdClass();

    if (!empty($date)) {
      $selection = new stdClass();
      $selection->calendar_date = strtotime($date);
      $activity->value = json_encode($selection);
      $activity->type = CultureFeed_Activity::TYPE_IK_GA;
    }
    else {
      $activity->type = CultureFeed_Activity::TYPE_LIKE;
    }
    $activity->nodeId = $node_id;

    $calendar = array();

    if (isset($_COOKIE['Drupal_visitor_calendar'])) {
      $calendar = json_decode($_COOKIE['Drupal_visitor_calendar']);
    }

    $calendar[] = $activity;
    end($calendar);
    $last_key = key($calendar);
    $calendar[$last_key]->id = $last_key;

    $values = array(
      'calendar' => json_encode($calendar),
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

    $calendar = json_decode($_COOKIE['Drupal_visitor_calendar']);
    if (isset($calendar[$activity->id])) {
      $selection = new stdClass();
      $selection->calendar_date = strtotime($date);
      $calendar[$activity->id]->value = json_encode($selection);
      $calendar[$activity->id]->type = CultureFeed_Activity::TYPE_IK_GA;
    }
    else {
      return FALSE;
    }

    $values = array(
      'calendar' => json_encode($calendar),
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

    $calendar = json_decode($_COOKIE['Drupal_visitor_calendar']);
    if (isset($calendar[$activity->id])) {
      unset($calendar[$activity->id]);
    }
    else {
      return FALSE;
    }

    $values = array(
      'calendar' => json_encode($calendar),
    );

    user_cookie_save($values);

    return TRUE;
  }

  return FALSE;
}

/**
 * Get the calendar date that is selected for an activity.
 */
function culturefeed_calendar_get_selected_date($activity) {

  if (isset($activity->value)) {
    $calendar_info = json_decode($activity->value);
    if ($calendar_info && !empty($calendar_info->calendar_date)) {
      return $calendar_info->calendar_date;
    }
  }

}

/**
 * Helper function to sort activities by date.
 */
function culturefeed_calendar_sort_by_date( $a, $b ) {

    return json_decode($a->value)->calendar_date - json_decode($b->value)->calendar_date;
}
