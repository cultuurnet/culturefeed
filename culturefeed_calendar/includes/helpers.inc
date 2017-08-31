<?php

/**
 * Helper functions for calendar.
 */

/**
 * Get the users activities.
 */
function culturefeed_calendar_get_user_activities($use_search = FALSE, $user_id = '') {

  $user_id = !empty($user_id) ? $user_id : DrupalCultureFeed::getLoggedInUserId();

  $activities = &drupal_static(__FUNCTION__, array());
  if (isset($activities[$user_id])) {
    return $activities;
  }

  if ($user_id) {
    $activities[$user_id] = culturefeed_calendar_get_culturefeed_user_activities($use_search, $user_id);
  }
  else {
    $activities[$user_id] = culturefeed_calendar_get_anonymous_user_activities($use_search);
  }

  return $activities[$user_id];
}

/**
 * Get all the calendar activities for a culturefeed user.
 */
function culturefeed_calendar_get_culturefeed_user_activities($use_search, $user_id) {

  $activities = new stdClass();
  $activities->objects = array();
  $activities->total = 0;

  // Only search for activities with type_like or type_ik_ga.
  $activity_options = array(
    CultureFeed_Activity::TYPE_LIKE,
    CultureFeed_Activity::TYPE_IK_GA,
  );

  $query = new CultureFeed_SearchActivitiesQuery();
  $query->max = 500;
  $query->type = $activity_options;
  $query->contentType = 'event';
  $query->userId = $user_id;

  // Use search for facet support.
  if ($use_search) {

    // No search cache for the calendar page.
    global $conf;
    $conf['culturefeed_search_cache_enabled'] = FALSE;

    $service = culturefeed_get_search_service();
    $parameters[] = new CultuurNet\Search\Parameter\Group();
    $parameters[] = new CultuurNet\Search\Parameter\Rows(500);
    $parameters[] = new CultuurNet\Search\Parameter\FilterQuery('type:event');
    $parameters[] = new CultuurNet\Search\Parameter\Query('*:*');
    $parameters[] = new CultuurNet\Search\Parameter\FilterQuery('attend_users:' . $user_id . ' OR like_users:' . $user_id);

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

  // Filter activities to make sure there are no duplicates.
  $activities->objects = culturefeed_calendar_filter_duplicate_activities($activities->objects);

  return $activities;

}

/**
 * Get the calendar activities for an anonymous user.
 */
function culturefeed_calendar_get_anonymous_user_activities($use_search) {

  $activities = new stdClass();
  $activities->objects = array();
  $activities->total = 0;

  // No cookie = no activities
  if (isset($_COOKIE['Drupal_visitor_calendar'])) {

    $calendar = json_decode($_COOKIE['Drupal_visitor_calendar']);

    // Parse calendarDate back to time
    $calendar_to_time = array();
    foreach ($calendar as $calendar_item) {
      $calendar_item->calendarDate = strtotime(culturefeed_calendar_get_selected_date($calendar_item));
      $calendar_to_time[] = $calendar_item;
    }
    $calendar = $calendar_to_time;
    
    // Check if cookie is valid.
    if (is_array($calendar)) {

      // Use search for facet support.
      if ($use_search) {

        $eventids = array();
        $filterids = array();
        foreach ($calendar as $key => $event) {
          $eventids[] = $event->nodeId;
          $filterids[] = '"' . $event->nodeId . '"';
        }

        $service = culturefeed_get_search_service();
        $parameters[] = new CultuurNet\Search\Parameter\Group();
        $parameters[] = new CultuurNet\Search\Parameter\Rows(500);
        $parameters[] = new CultuurNet\Search\Parameter\FilterQuery('type:event');
        $parameters[] = new CultuurNet\Search\Parameter\Query('cdbid IN(' . implode(',', $eventids) . ')');

        try {

          $result = $service->search($parameters);
          // No future events in the cookie.
          if ($result->getTotalCount() == 0) {
            return $activities;
          }

          $activities->objects = array();
          $items = $result->getItems();
          // Set only the events that are found on search api (=future events).
          foreach ($items as $item) {
            $key = array_search($item->getId(), $eventids);
            if ($key !== FALSE) {
              unset($eventids[$key]);
              $activities->objects[] = $calendar[$key];
            }
          }

          // All remaining eventids don't exist in future, remove them of cookie.
          foreach ($eventids as $key => $id) {
            culturefeed_calendar_delete_calendar_event_cookie($calendar[$key]);
          }


        }
        catch (Exception $e) {
          watchdog_exception('culturefeed_calendar', $e);
        }

      }
      else {
        $activities->objects = $calendar;
      }

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
  $actions = array('view', 'like', 'add');
  $buttons = array();
  $button = array();

  $button['item_id'] = $id;

  $ids_to_check = array();
  $activities = NULL;
  if ($authenticated) {
    $activities = culturefeed_calendar_get_user_activities();
    if (isset($activities->objects)) {
      foreach($activities->objects as $key => $activity) {
        $ids_to_check[] = $activity->nodeId;
      }
    }
  }

  $event_status = culturefeed_calendar_get_event_status($item);

  // When event is passed or started, show only a message aside.
  if ($event_status !== CULTUREFEED_CALENDAR_EVENT_PLANNABLE) {
    $locations = array('aside');
  }
  else {
    // else show message in content and aside
    $locations = array('content', 'aside');
  }

  // locations = aside and/or content
  foreach ($locations as $location) {
    $button['location'] = $location;
    // Show the correct button for authenticated users.
    if ($authenticated) {
      if (in_array($id, $ids_to_check)) {
        $actions = array('view');
      }
      else {
        $actions = array('like', 'add');
      }

      foreach($actions as $action) {
        $button['action'] = $action;
        $buttons[$location][$action] = theme('culturefeed_calendar_button', array('button' => $button, 'authenticated' => $authenticated, 'status' => $event_status));
      }
    }
    // Show all buttons for anonymous users. JS will show the correct one.
    else {
      foreach ($actions as $action) {
        $button['action'] = $action;
        $buttons[$location][$action] = theme('culturefeed_calendar_button', array('button' => $button, 'authenticated' => $authenticated, 'status' => $event_status));
      }
    }
  }
  return $buttons;
}

/**
 * Helper function to add an activity to the calendar.
 */
function culturefeed_calendar_add_activity($node_id, $node_title, $date = '') {

  $authenticated = DrupalCultureFeed::isCultureFeedUser();

  if ($authenticated) {
    $activity = new CultureFeed_Activity();
    if (!empty($date)) {
      $activity->calendarDate = gmdate('c', strtotime($date));
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
      $activity->calendarDate = gmdate('c', strtotime($date));
      $activity->type = CultureFeed_Activity::TYPE_IK_GA;
    }
    else {
      $activity->type = CultureFeed_Activity::TYPE_LIKE;
    }
    $activity->nodeId = $node_id;

    $calendar = array();

    if (isset($_COOKIE['Drupal_visitor_calendar'])) {
      $calendar = json_decode($_COOKIE['Drupal_visitor_calendar']);
      // Invalid cookie value, reset again.
      if (!is_array($calendar)) {
        $calendar = array();
      }
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
 * Updates a calendar event in the cookie for anonymous users.
 *
 * @param CultureFeed_Activity $activity
 * @param string $date String version of the date to save.
 */
function culturefeed_calendar_update_calendar_event_cookie($activity, $date) {

  if (isset($_COOKIE['Drupal_visitor_calendar'])) {

    $calendar = json_decode($_COOKIE['Drupal_visitor_calendar']);
    if (is_array($calendar) && isset($calendar[$activity->id])) {
      $calendar[$activity->id]->calendarDate = gmdate('c', strtotime($date));;
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
    if (is_array($calendar) && isset($calendar[$activity->id])) {
      unset($calendar[$activity->id]);
    }
    else {
      return FALSE;
    }

    // Reset the keys to become numeric again.
    $calendar = array_values($calendar);
    // Also reset the ids.
    foreach ($calendar as $key => $calendar_item) {
      $calendar[$key]->id = $key;
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

  if (!empty($activity->calendarDate)) {
    return $activity->calendarDate;
  }
  else if (!empty($activity->value) ) {
    $calendar_info = json_decode($activity->value);
    if ($calendar_info && !empty($calendar_info->calendar_date)) {
      return $calendar_info->calendar_date;
    }
  }
}

/**
 * Helper function to sort activities by date.
 */
function culturefeed_calendar_sort_by_date($a, $b) {

  $calendar_date_a = 0;
  $calendar_date_b = 0;

  // Calendar date property of a exists.
  if (!empty($a->calendarDate)) {
    $calendar_date_a = $a->calendarDate;
  }
  // Calendar date a is stored in value.
  elseif (!empty($calendar_date_a->value)) {
    $json_value_a = json_decode($a->value);
    if ($json_value_a && !empty($json_value_a->calendar_date)) {
      $calendar_date_a = $json_value_a->calendar_date;
    }
  }

  // Calendar date property of b exists.
  if (!empty($b->calendarDate)) {
    $calendar_date_b = $b->calendarDate;
  }
  // Calendar date b is stored in value.
  elseif (!empty($calendar_date_b->value)) {
    $json_value_b = json_decode($b->value);
    if ($json_value_b && !empty($json_value_b->calendar_date)) {
      $calendar_date_b = $json_value_b->calendar_date;
    }
  }

  return $calendar_date_a - $calendar_date_b;

}

/**
 * Helper function to filter out duplicate activities.
 */
function culturefeed_calendar_filter_duplicate_activities($activities) {
  $filtered_activities = array();

  foreach ($activities as $key => $activity) {

    // First time this activity is found.
    if (empty($filtered_activities[$activity->nodeId])) {
      $filtered_activities[$activity->nodeId] = $activity;
    }

    // go's are more important then like's
    else if ($activity->type == CultureFeed_Activity::TYPE_IK_GA && $filtered_activities[$activity->nodeId]->type == CultureFeed_Activity::TYPE_LIKE) {
      $filtered_activities[$activity->nodeId] = $activity;
    }

    // When type is go. Activities with a date are more important.
    else if ($activity->type == CultureFeed_Activity::TYPE_IK_GA && (!empty($activity->calendarDate) || !empty($activity->value))) {
      $filtered_activities[$activity->nodeId] = $activity;
    }
  }

  return $filtered_activities;
}

/**
 * Helper function to read cookie betaCalendar (in progress)
 */
function culturefeed_calendar_enabled() {
  if (variable_get('culturefeed_calendar_beta', FALSE)) {  

    // Check if the user has opted in for the beta program
    if (isset($_COOKIE['betaCalendar']) && $_COOKIE['betaCalendar'] == 'true') {
      return TRUE;
    }
    else {
      return FALSE;
    }

  }

  return TRUE;

}

/**
 * Helper function to get the url of the calendar
 */
function culturefeed_calendar_get_url() {
  global $base_url;
  return $base_url . '/culturefeed/calendar';
}

/**
 * Helper function to check if an activity is finished (date not between now and now + 11 months)
 */
function culturefeed_calendar_activity_is_finished($activity) {
  if ($selected_date = culturefeed_calendar_get_selected_date($activity)) {

    // Only show activities between current month and 11 months in the future
    $activity_month = format_date($selected_date, 'custom', 'Ym');
    $current_month = format_date(REQUEST_TIME, 'custom', 'Ym');
    
    if ($activity_month < $current_month || $activity_month > date('Ym', strtotime("11 months"))) {
      return TRUE;
    }

  }
  else {
    return TRUE;
  }

  return FALSE;

}

/**
 * Helper function to get the event status.
 */
function culturefeed_calendar_get_event_status($item) {

  $event_status = CULTUREFEED_CALENDAR_EVENT_FINISHED;

  if ($item->getType() == 'event') {
    $event = $item->getEntity();
    if ($event instanceof CultureFeed_Cdb_Item_Event) {
      $objDateTime = new DateTime('NOW');
      $calendar = $event->getCalendar();

      if ($calendar instanceof CultureFeed_Cdb_Data_Calendar_TimestampList || $calendar instanceof CultureFeed_Cdb_Data_Calendar_PeriodList) {

        // Check if 1 of the items is still in the future.
        foreach ($calendar as $calendar_item) {

          if ($calendar_item instanceof CultureFeed_Cdb_Data_Calendar_Period) {

            $dateToStart = DateTime::createFromFormat('Y-m-d H:i:s', $calendar_item->getDateTo() . ' 00:00:00');
            $dateToEnd = DateTime::createFromFormat('Y-m-d H:i:s', $calendar_item->getDateTo() . ' 23:59:59');
            // End date is in the future, so plannable
            if ($objDateTime->getTimestamp() < $dateToStart->getTimestamp()) {
              return CULTUREFEED_CALENDAR_EVENT_PLANNABLE;
            }
            // End date is busy so event started.
            elseif ($objDateTime->getTimestamp() >= $dateToStart->getTimestamp() && $objDateTime->getTimestamp() <= $dateToEnd->getTimestamp()) {
              $event_status = CULTUREFEED_CALENDAR_EVENT_STARTED;
            }

          }
          elseif ($calendar_item instanceof CultureFeed_Cdb_Data_Calendar_Timestamp) {

            // Get start time.
            if ($calendar_item->getStartTime()) {
              $starttime = DateTime::createFromFormat('Y-m-d H:i:s', $calendar_item->getDate() . ' ' . $calendar_item->getStartTime());
            }
            else {
              $starttime = DateTime::createFromFormat('Y-m-d H:i:s', $calendar_item->getDate() . ' 00:00:00');
            }

            // Get end time.
            if ($calendar_item->getEndTime() && $calendar_item->getEndTime() !== "00:00:00") {
              $endtime = DateTime::createFromFormat('Y-m-d H:i:s', $calendar_item->getDate() . ' ' . $calendar_item->getEndTime());
            }
            else {
              $endtime = DateTime::createFromFormat('Y-m-d H:i:s', $calendar_item->getDate() . ' 23:59:59');
            }

            // Start time still to come?
            if ($starttime->getTimestamp() > $objDateTime->getTimestamp()) {
              return CULTUREFEED_CALENDAR_EVENT_PLANNABLE;
            }
            elseif ($endtime->getTimestamp() > $objDateTime->getTimestamp()) {
              $event_status = CULTUREFEED_CALENDAR_EVENT_STARTED;
            }
          }
        }
      }
      elseif ($calendar instanceof CultureFeed_Cdb_Data_Calendar_Permanent) {
        $event_status = CULTUREFEED_CALENDAR_EVENT_PLANNABLE;
      }
    }
  }

  return $event_status;
}

/**
 * Helper function to update a calendar event.
 * @param $activity
 * @param $date
 */
function culturefeed_calendar_update_calendar_event($activity, $date) {
  // Not numeric id = activity on service.
  $success = TRUE;
  if (!is_numeric($activity->id) != 0) {
    try {

      DrupalCultureFeed::deleteActivity($activity->id);

      $activity->value = '';
      $activity->calendarDate = gmdate('c', strtotime($date));
      $new_activity = $activity;
      $new_activity->type = CultureFeed_Activity::TYPE_IK_GA;

      DrupalCultureFeed::createActivity($new_activity);

      // Add a timeout of 2 second, this gives index the time to place the new activity into the search index.
      // sleep(2);

    }
    catch (Exception $e) {
      $success = FALSE;
      watchdog_exception('culturefeed_calendar', $e);
    }
  }
  // No uuid = anonymous user. Update in cookie.
  else {
    $success = culturefeed_calendar_update_calendar_event_cookie($activity, $date);
  }

  if ($success) {
    drupal_set_message(t('Your calendar event was updated.'));
  }
  else {
    drupal_set_message(t('An error occured while saving your calendar'), 'error');
  }
}

