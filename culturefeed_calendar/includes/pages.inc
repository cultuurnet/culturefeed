<?php
/**
 * @file
 * Page callbacks for the calendar module.
 */


/**
 * Page callback to show the calendar page.
 */
function culturefeed_calendar_page_my_activities() {

  $activities = culturefeed_calendar_get_user_activities(TRUE);
  return theme('culturefeed_calendar_page', array('activities' => $activities));

}

/**
 * Form to add a button to save the cookie events to the calendar.
 */
function culturefeed_calendar_save_cookie_form($form, &$form_state) {
  $form['save_cookie'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('culturefeed_calendar_save_cookie_form_submit'),
  );

  return $form;
}

/**
 * Submit the save cookie form.
 */
function culturefeed_calendar_save_cookie_form_submit($form, &$form_state) {
  //Go to the login screen to let the user login.
  $url = 'culturefeed/oauth/connect';
  $options = array(
    'query' => array(
      'destination' => 'culturefeed/calendar/add-cookie-to-calendar',
    ),
  );
  drupal_goto($url, $options);
}

/**
 * Page callback to move the events from the cookie to the calendar.
 */
function culturefeed_calendar_add_cookie_to_calendar() {
  $user_id = DrupalCultureFeed::getLoggedInUserId();
  // Stop if we don't have a user id.
  if (empty($user_id)) {
    return;
  }

  //Get activities from cookie
  $cookie_activities = array();
  if (isset($_COOKIE['Drupal_visitor_calendar'])) {
    $cookie_activities = json_decode($_COOKIE['Drupal_visitor_calendar']);
  }

  // Get activities from calendar.
  $calendar_ids = array();
  $calendar_activities = culturefeed_calendar_get_user_activities();
  foreach ($calendar_activities->objects as $calendar_activity) {
    $calendar_ids[] = $calendar_activity->nodeId;
  }

  // Create a culturefeed activity (CultureFeed_Activity()) for each cookie activity (stdClass).
  $added = 0;
  $skipped = 0;
  foreach ($cookie_activities as $key => $cookie_activity) {
    // Only create activities that don't already exist.
    if (!in_array($cookie_activity->nodeId, $calendar_ids)) {
      $activity = new CultureFeed_Activity();
      // Add planned activity's' date.
      if (isset($cookie_activity->value)) {
        $activity->value = $cookie_activity->value;
      }
      $activity->type = $cookie_activity->type;
      $activity->contentType = 'event';
      $activity->nodeId = $cookie_activity->nodeId;
      $activity->nodeTitle = culturefeed_search_item_load($activity->nodeId, 'event')->getTitle(culturefeed_search_get_preferred_language());
      $activity->userId = $user_id;

      DrupalCultureFeed::createActivity($activity);

      $added++;
    }
    else {
      $skipped++;
    }
  }

  // Delete the cookie.
  user_cookie_delete('calendar');

  // Proceed to the calendar.
  drupal_goto('culturefeed/calendar');
}

/**
 * Page callback to add items to the calendar.
 */
function culturefeed_calendar_add_to_calendar(CultuurNet\Search\ActivityStatsExtendedEntity $event, $request_type = 'ajax') {

  $params = _culturefeed_calendar_get_event_params($event);
  $params['title'] = t('Add !title to your calendar', array('!title' => $params['node_title']));
  $params['button_text'] = t('Add');
  $params['use_ajax'] = $request_type == 'ajax';

  // If no form should be shown, we can create the activity.
  if ($params['get_form']) {
    $form = drupal_get_form('culturefeed_calendar_form', $params);
  }
  else {
    try {
      culturefeed_calendar_add_activity($params['node_id'], $params['node_title'], $params['dates'][0]['date']);
      drupal_set_message(t('The event has been added to your calendar.'));
    }
    catch (Exception $e) {
      drupal_set_message(t('There was an error while adding to your calendar'), 'error');
    }
  }

  if ($request_type == 'ajax') {

    // No form should be shown, redirect to the detailpage.
    $commands = array();
    if ($params['get_form']) {
      $commands[] = culturefeed_ajax_command_modal('#calendar-form', drupal_render($form));
    }
    else {
      $url = culturefeed_search_detail_url('event', $event->getId(), $event->getTitle(culturefeed_get_preferred_language()));
      $commands[] = culturefeed_ajax_command_goto($url);
    }

    ajax_deliver(array('#type' => 'ajax', '#commands' => $commands));
    exit;

  }
  else {
    if ($params['get_form']) {
      return $form;
    }
    else {
      drupal_goto(culturefeed_search_detail_path('event', $event->getId(), $event->getTitle(culturefeed_get_preferred_language())));
    }

  }
}

/**
 * Page callback to edit events in the calendar.
 */
function culturefeed_calendar_edit_calendar_event($activity, $request_type = 'ajax') {

  $event = culturefeed_agenda_event_load($activity->nodeId);

  $params = _culturefeed_calendar_get_event_params($event);
  $params['title'] = t('Reschedule !title in your calendar', array('!title' => $params['node_title']));
  $params['button_text'] = t('Update event');
  $params['use_ajax'] = $request_type == 'ajax';
  $params['activity'] = $activity;

  $form = drupal_get_form('culturefeed_calendar_form', $params);

  // Always show the form/modal when editing events, no need to check on $params['show_form'].
  if ($request_type == 'ajax') {
    $commands = array();
    $commands[] = culturefeed_ajax_command_modal('#calendar-form', drupal_render($form));
    print ajax_render($commands);
    exit;
  }
  else {
    return $form;
  }

}

/**
 * Form edit/add an event to the calendar.
 */
function culturefeed_calendar_form($form, &$form_state, $params) {

  // Activity is passed when editing.
  $selected_date = culturefeed_calendar_get_selected_date($params['activity']);
  $edit = !empty($selected_date) ? TRUE : FALSE;

  drupal_set_title($params['title']);
  
  // Only show possible dates for periods where calendar summary is available
  $possible_dates = '';
  if (($params['type'] == 'period') && $params['calendar_summary']) {
    $info = '<label>' . t('Possible dates') . ':' . '</label><p>' . $params['calendar_summary'] . '</p>';
  }
  
  $form['info'] = array(
    '#type' => 'markup',
    '#markup' => $possible_dates,
  );

  if (($params['type'] == 'period') || ($params['type'] == 'permanent')) {
    $form['date'] = array(
      '#title' => t('When do you want to go?'),
      '#type' => 'date_popup',
      '#date_format' => 'd-m-Y',
      '#date_year_range' => '-0:+1',
      '#default_value' => $edit ? date('Y-m-d H:i', $selected_date) : '',
    );
  }
  // timestamps.
  else {
    foreach ($params['dates'] as $key => $date) {
      $time_string = "";
      if (!empty($date['start'])) {
        $time_string .= t(" from ") . $date['start'];
        if (!empty($date['end'])) {
          $time_string .= t(" to ") . $date['end'];
        }
      }
      $date_formatted = date('d-m-Y', strtotime($date['date']));
      if(strtotime($date['date']) >= time()) {
        $dates[$date['date']] = date('d-m-Y', strtotime($date_formatted)) . $time_string;
      }
    }

    $form['date'] = array(
      '#title' => t('When do you want to go?'),
      '#type' => 'radios',
      '#options' => $dates,
      '#default_value' => $edit ? date('Y-m-d', $selected_date) : '',
    );
  }

  $form['errors'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="add-to-calendar-errors"></div>',
    '#weight' => -20,
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $submit_function = isset($params['activity']) ? 'culturefeed_calendar_edit_calendar_event_form_submit' : 'culturefeed_calendar_add_to_calendar_form_submit';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => $params['button_text'],
    '#name' => 'add',
    '#submit' => array($submit_function),
    '#validate' => array('culturefeed_calendar_form_validate'),
  );

  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t("Cancel"),
    '#name' => 'cancel',
    '#submit' => array($submit_function),
    '#limit_validation_errors' => array(),
  );

  if ($params['use_ajax']) {
    $form['actions']['submit']['#ajax'] = array(
      'callback' => 'culturefeed_calendar_form_ajax',
      'wrapper' => 'calendar-form',
    );
    $form['actions']['cancel']['#ajax'] = array(
      'callback' => 'culturefeed_calendar_form_ajax',
      'wrapper' => 'calendar-form',
    );
  }

  $form_state['params'] = $params;

  return $form;
}

/**
 * Ajax callback for the calendar form.
 */
function culturefeed_calendar_form_ajax($form, $form_state) {

  // When editing, activity is passed; after edit, return to the calendar page.
  if (isset($form_state['params']['activity'])) {
    $url = url('culturefeed/calendar');
  }
  // After adding, return to event detail.
  else {
    $url = culturefeed_search_detail_url('event', $form_state['params']['node_id'], $form_state['params']['node_title']);
  }

  $commands = array();
  if (empty($form_state['params']['validation_error'])) {
    $commands[] = culturefeed_ajax_command_goto($url);
  }
  else {
    $commands[] = ajax_command_html('#calendar-form', drupal_render($form));
    $commands[] = ajax_command_html('#add-to-calendar-errors', theme('status_messages'));
  }

  return array('#type' => 'ajax', '#commands' => $commands);

}

/**
 * Validate the add to calendar form.
 */
function culturefeed_calendar_form_validate(&$form, &$form_state) {

  $form_state['params']['validation_error'] = FALSE;

  if ($form_state['params']['type'] == 'period' && !empty($form_state['values']['date'])) {

    $date = strtotime($form_state['values']['date']);
    $date_from = strtotime($form_state['params']['date_from']);
    $date_to = strtotime($form_state['params']['date_to']);

    if ($date <= time()) {
      form_set_error('date', t('You selected a date in the past. Please select a date in the future.'));
      $form_state['params']['validation_error'] = TRUE;
    }
    // Check if the chosen date is between the start and end date of the event.
    elseif (($date < $date_from) || ($date > $date_to)) {
      form_set_error('date', t('Enter a date between ') . date('d-m-Y', $date_from) . t(' and ') . date('d-m-Y', $date_to) . '.');
      $form_state['params']['validation_error'] = TRUE;
    }
    // Check if the event is opened on the chosen day.
    elseif (!empty($form_state['params']['week_scheme'])) {
      $week_scheme = $form_state['params']['week_scheme'];
      $day = strtolower(date('l', $date));
      $day_allowed = $week_scheme->getDay($day)->getopenType() == 'closed' ? FALSE : TRUE;

      if (!$day_allowed) {
        $form_state['params']['validation_error'] = TRUE;
        form_set_error('date', t('The event is closed on ') . t($day) . '. Please chose another day.');
      }
    }
  }
}

/**
 * Submit the add to calendar form.
 */
function culturefeed_calendar_add_to_calendar_form_submit($form, &$form_state) {

  $node_title = $form_state['params']['node_title'];
  $node_id = $form_state['params']['node_id'];
  $form_state['redirect'] = 'agenda/e/' . $node_title . '/' . $node_id;

  $date = $form_state['values']['date'];
  if ($form_state['triggering_element']['#name'] == 'add') {

    try {
      culturefeed_calendar_add_activity($node_id, $node_title, $date);
      drupal_set_message(t('The event has been added to your calendar.'));
    }
    catch (Exception $e) {
      drupal_set_message(t('There was an error while adding to your calendar'), 'error');
    }

  }
}

/**
 * Submit handler for editing calendar events.
 */
function culturefeed_calendar_edit_calendar_event_form_submit($form, &$form_state) {

  if ($form_state['triggering_element']['#name'] == 'cancel') {
    $form_state['redirect'] = 'culturefeed/calendar';
    return;
  }

  $activity = $form_state['params']['activity'];
  $date = $form_state['values']['date'];

  // Not numeric id = activity on service.
  $success = TRUE;
  if (!is_numeric($activity->id) != 0) {
    try {

      DrupalCultureFeed::deleteActivity($activity->id);

      $selection = new stdClass();
      $selection->calendar_date = strtotime($date);

      $new_activity = $activity;
      $new_activity->value = json_encode($selection);
      $new_activity->type = CultureFeed_Activity::TYPE_IK_GA;

      DrupalCultureFeed::createActivity($new_activity);
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

  if (!$form_state['params']['use_ajax']) {
    $form_state['redirect'] = 'culturefeed/calendar';
  }

}

/**
 * Page callback to delete events from the calendar.
 */
function culturefeed_calendar_delete_calendar_event($activity, $request_type = 'ajax') {

  $event = culturefeed_agenda_event_load($activity->nodeId);
  $params = _culturefeed_calendar_get_event_params($event);
  $params['title'] = t('Evenement verwijderen');
  $params['activity'] = $activity;
  $params['use_ajax'] = $request_type == 'ajax';

  $form = drupal_get_form('culturefeed_calendar_delete_form', $params);

  if ($request_type == 'ajax') {
    $commands = array();
    $commands[] = culturefeed_ajax_command_modal('#calendar-form', drupal_render($form));

    print ajax_render($commands);
    exit;
  }
  else {
    return $form;
  }

}

/**
 * Form to delete a calendar event.
 */
function culturefeed_calendar_delete_form($form, &$form_state, $params) {

  drupal_set_title($params['title']);

  $form['info'] = array(
    '#type' => 'markup',
    '#markup' => '<label>' . t('Are you sure you want to delete \'!title\' from your UiTkalender?', array('!title' => $params['node_title'])) . '</label>',
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Verwijderen'),
    '#name' => 'delete',
    '#submit' => array('culturefeed_calendar_delete_form_submit'),
  );

  if ($params['use_ajax']) {
    $form['actions']['submit']['#ajax'] = array(
      'callback' => 'culturefeed_calendar_delete_form_ajax',
      'wrapper' => 'calendar-form',
    );
  }

  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t("Cancel"),
    '#name' => 'cancel',
    '#submit' => array('culturefeed_calendar_delete_form_submit'),
  );

  $form_state['params'] = $params;

  return $form;

}

/**
 * Ajax callback for the calendar delete form.
 */
function culturefeed_calendar_delete_form_ajax($form, $form_state) {

  $commands = array();
  $commands[] = culturefeed_ajax_command_goto(url('culturefeed/calendar'));

  print ajax_render($commands);
  exit;

}

/**
 * Submit handler for deleting calendar events.
 */
function culturefeed_calendar_delete_form_submit($form, &$form_state) {

  $form_state['redirect'] = 'culturefeed/calendar';

  if ($form_state['triggering_element']['#name'] == 'cancel') {
    return;
  }

  $activity = $form_state['params']['activity'];

  // Not numeric id = activity on service.
  $success = TRUE;
  if (!is_numeric($activity->id) != 0) {
    try {
      DrupalCultureFeed::deleteActivity($activity->id);
    }
    catch (Exception $e) {
      $success = FALSE;
      watchdog_exception('culturefeed_calendar', $e);
    }
  }
  // No uuid = anonymous user. Update in cookie.
  else {
    $success = culturefeed_calendar_delete_calendar_event_cookie($activity);
  }

  if ($success) {
    drupal_set_message(t('Your calendar event was deleted.'));
  }
  else {
    drupal_set_message(t('An error occured while deleting the event from your calendar'), 'error');
  }

}

/**
 * Helper function that get form parameters for edit/add/delete activity-forms.
 *
 * @param type $event
 * @return type
 */
function _culturefeed_calendar_get_event_params($event) {

  $entity = $event->getEntity();
  $node_id = $entity->getCdbId();

  $event_detail = $entity->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language());
  $calendar_summary = check_plain($event_detail->getCalendarSummary());

  $calendar = $entity->getCalendar();
  $calendar_type = get_class($calendar);

  $lang_code = $GLOBALS['language']->language;
  $title = $event->getTitle($lang_code);

  $params = array();
  $params['node_id'] = $node_id;
  $params['node_title'] = $title;
  $params['calendar_summary'] = $calendar_summary;
  $params['get_form'] = TRUE;

  if ($calendar_type == 'CultureFeed_Cdb_Data_Calendar_Permanent') {
    $params['type'] = 'permanent';
  }
  elseif ($calendar_type == 'CultureFeed_Cdb_Data_Calendar_PeriodList') {
    $params['date_from'] = $calendar->current()->getDateFrom();
    $params['date_to'] = $calendar->current()->getDateTo();
    $params['type'] = 'period';
    $params['week_scheme'] = $calendar->current()->getWeekScheme();
  }
  elseif ($calendar_type == 'CultureFeed_Cdb_Data_Calendar_TimestampList') {
    $i = 0;
    $dates = array();
    while ($calendar->valid()) {
      $dates[$i]['date'] = $calendar->current()->getDate();
      $dates[$i]['start'] = $calendar->current()->getStartTime();
      $dates[$i]['end'] = $calendar->current()->getEndTime();
      $calendar->next();
      $i++;
    }

    if (count($dates) == 1) {
      $params['get_form'] = FALSE;
      $params['dates'] = $dates;
      return $params;
    }
    else {
      $params['dates'] = $dates;
      $params['type'] = 'timestamps';
    }
  }

  return $params;
}

/**
 * Helper function that gets the current and next twelve months for nav links.
 * @return array
 *  - full month name
 *  - short month name
 *  - year
 */
function _culturefeed_calendar_get_nav_months() {

  $months = array();
  $month = date('n'); // Current month.
  for ($x = 0; $x < 12; $x++) {
    $time = mktime(0, 0, 0, $month + $x, 1);
    $item = array(
      'full_month' => format_date($time, 'custom', 'F'),
      'month' => format_date($time, 'custom', 'M'),
      'year' => format_date($time, 'custom', 'Y'),
    );
    $months[] = $item;

  }

  return $months;
}

/**
 * Form to add a button to go to share the calendar page.
 */
function culturefeed_calendar_share_calendar_button_form($form, &$form_state) {
  $form['share_calendar'] = array(
    '#type' => 'submit',
    '#value' => t('Share'),
    '#disabled' => FALSE,
  );

  return $form;
}

/**
 * Submit the share calendar button form.
 */
function culturefeed_calendar_share_calendar_button_form_submit($form, &$form_state) {

}