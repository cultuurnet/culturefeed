<?php
/**
 * @file
 * Page callbacks for the calendar module.
 */


/**
 * Page callback to show the calendar page.
 */
function culturefeed_calendar_page_my_activities() {

  $output = '';

  $activities = culturefeed_calendar_get_user_activities();
  $activities_by_type = _culturefeed_calendar_sort_user_activities_by_type($activities);
  $authenticated = DrupalCultureFeed::isCultureFeedUser();

  // Add a block for months-scroller with months.
  $months = array_map("_culturefeed_calendar_months_get_nav_months", range(1,12));
  $output .= theme('culturefeed_calendar_nav_months', array('months' => $months));

  // Print the 'LIKED' activities.
  if (isset($activities_by_type[CultureFeed_Activity::TYPE_LIKE])) {
    $output .= theme('culturefeed_calendar_activities_liked',
      array('activities' => $activities_by_type[CultureFeed_Activity::TYPE_LIKE]));
  }
  // Print the 'IK GA' activities.
  if (isset($activities_by_type[CultureFeed_Activity::TYPE_IK_GA])) {
    $output .= theme('culturefeed_calendar_activities_going',
      array('activities' => $activities_by_type[CultureFeed_Activity::TYPE_IK_GA], 'month_names' => $months));
  }

  // Print the 'I’m going' activities.
  if (!$authenticated) {
    $form = drupal_get_form('culturefeed_calendar_save_cookie_form');
    $output .= drupal_render($form);
  }

  return $output;
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

  //Get activities from cookie
  $activities = array();
  if(isset($_COOKIE['Drupal_visitor_calendar'])) {
    $activities = unserialize($_COOKIE['Drupal_visitor_calendar']);
  }

  foreach ($activities as $key => $activity) {
    $activity->userId = $user_id;
    DrupalCultureFeed::createActivity($activity);

  }
  user_cookie_delete('calendar');

  drupal_goto('culturefeed/calendar');
}

/**
 * Page callback to add items to the calendar
 */
function culturefeed_calendar_add_to_calendar(CultuurNet\Search\ActivityStatsExtendedEntity $event, $request_type = 'ajax') {

  $params = _culturefeed_calendar_get_event_params($event);
  $params['validate'] = 'culturefeed_calendar_add_to_calendar_form_validate';
  $params['submit'] = 'culturefeed_calendar_add_to_calendar_form_submit';
  $params['title'] = t('Add !title to your OutCalendar', array('!title' => $params['node_title']));
  $params['operation'] = 'add';

  // If no form should be shown, we can create the activity.
  if ($params['get_form']) {
    $form = drupal_get_form('culturefeed_calendar_form', $params);
  }
  else {
    culturefeed_calendar_add_activity($params['node_id'], $params['node_title'], $params['dates'][0]['date']);
  }

  if ($request_type == 'ajax') {

    $commands = array();
    if (!empty($form)) {
      $commands[] = culturefeed_ajax_command_modal('#calendar-form', drupal_render($form));
    }
    else {
      $url = culturefeed_search_detail_url('event', $event->getId(), $event->getTitle(culturefeed_get_preferred_language()));
      $url .= '?added_to_calendar=true';
      $commands[] = culturefeed_ajax_command_goto($url);
    }

    print ajax_render($commands);
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
 * Bla
 */
function culturefeed_calendar_edit_calendar_event($activity, $request_type = 'ajax') {

  $event = culturefeed_agenda_event_load($activity->nodeId);
  $params = _culturefeed_calendar_get_event_params($event);
  $params['validate'] = 'culturefeed_calendar_add_to_calendar_form_validate';
  $params['submit'] = 'culturefeed_calendar_edit_calendar_event_form_submit';
  $params['title'] = t('Change !title in your OutCalendar', array('!title' => $params['node_title']));
  $params['activity'] = $activity;
  $params['operation'] = 'add';

  $form = drupal_get_form('culturefeed_calendar_form', $params);

  if ($request_type == 'ajax') {

    $commands = array();
    if ($params['get_form']) {
      $commands[] = culturefeed_ajax_command_modal('#calendar-form', drupal_render($form));
    }
    else {
      $url = 'culturefeed/calendar';
      $commands[] = culturefeed_ajax_command_goto($url);
    }

    print ajax_render($commands);

    exit;
  }
  else {

    if ($params['get_form']) {
      return $form;
    }
    else {
      drupal_goto('culturefeed/calendar');
    }

  }

}

/**
 * Form to choose dates before adding an event to the calendar.
 */
function culturefeed_calendar_form($form, &$form_state, $params) {

  drupal_set_title($params['title']);

  $form['info'] = array(
    '#type' => 'markup',
    '#markup' => '<label>' . t($params['node_title'] . t(' runs ') . $params['calendar_summary']  . '.') . '</label>',
  );

  if (($params['type'] == 'period') || ($params['type'] == 'permanent')) {

    $form['date'] = array(
      '#title' => t('When do you want to go?'),
      '#type' => 'date_popup',
      '#date_format' => 'd-m-Y H:i',
      '#date_year_range' => '-0:+1',
      '#default_value' => '',
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
      $dates[$date['date']] = date('d-m-Y', strtotime($date_formatted)) . $time_string;
    }

    $form['date'] = array(
      '#title' => t('When do you want to go?'),
      '#type' => 'radios',
      '#options' => $dates,
      '#default_value' => 0,
    );
  }

  $form['errors'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="add-to-calendar-ajax"></div>'
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Add"),
    '#name' => 'add',
    '#submit' => array($params['submit']),
    '#validate' => array($params['validate']),
    '#ajax' => array(
        'callback' => 'culturefeed_calendar_form_ajax',
        'wrapper' => 'calendar-form',
       ),
  );

  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t("Cancel"),
    '#name' => 'cancel',
    '#submit' => array($params['submit']),
  );

  $form_state['params'] = $params;

  return $form;
}

/**
 * Ajax callback for the calendar form.
 */
function culturefeed_calendar_form_ajax($form, $form_state) {

  // When editing, activity is passed; after edit, return to the calendar page.
  if (isset($form_state['params']['activity'])) {
    $url = '/culturefeed/calendar';
  }
  else {
    $commands = array();
    $commands[] = culturefeed_ajax_command_modal('#calendar-form', drupal_render($form));
    $commands[] = ajax_command_prepend('#add-to-calendar-ajax', theme('status_messages'));

    print ajax_render($commands);
    exit;
  }
}

/**
 * Validate the add to calendar form.
 */
function culturefeed_calendar_add_to_calendar_form_validate(&$form, &$form_state) {

  if ($form_state['params']['type'] == 'period' && !empty($form_state['values']['date'])) {
    $date = strtotime($form_state['values']['date']);
    $date_from = strtotime($form_state['params']['date_from']);
    $date_to = strtotime($form_state['params']['date_to']);
    if (($date < $date_from) || ($date > $date_to)) {
      form_set_error('date', t('Enter a date between ') . date('d-m-Y H:i', $date_from) . t(' and ') . date('d-m-Y H:i', $date_to) . '.');
    }
  }
}

/**
 * Submit the add to calendar form.
 */
function culturefeed_calendar_add_to_calendar_form_submit($form, &$form_state) {

  $node_title = $form_state['params']['node_title'];
  $node_id = $form_state['params']['node_id'];

  $date = $form_state['values']['date'];
  if ($form_state['triggering_element']['#name'] == 'add') {
    culturefeed_calendar_add_activity($node_id, $node_title, $date);
  }
  elseif ($form_state['triggering_element']['#name'] == 'cancel') {
    drupal_set_message(t('The event was not added to your calendar.'));
    drupal_goto('agenda/e/' . $node_title . '/' . $node_id);
  }
}

/**
 * Form to choose dates before adding an event to the calendar.
 */
function culturefeed_calendar_edit_calendar_event_form($form, &$form_state, $params) {

  drupal_set_title(t('Change the date for !title in your OutCalendar', array('!title' => $params['node_title'])));

  $form['info'] = array(
    '#type' => 'markup',
    '#markup' => '<label>' . t($params['node_title'] . t(' runs ') . $params['calendar_summary']  . '.') . '</label>',
  );

  if (($params['type'] == 'period') || ($params['type'] == 'permanent')) {

    $form['date'] = array(
      '#title' => t('When do you want to go?'),
      '#type' => 'date_popup',
      '#date_format' => 'd-m-Y H:i',
      '#date_year_range' => '-0:+1',
      '#default_value' => '',
    );
  }
  elseif ($params['type'] == 'timestamps') {

    foreach ($params['dates'] as $key => $date) {

      $time_string = "";
      if (!empty($date['start'])) {
        $time_string .= t(" from ") . $date['start'];
        if (!empty($date['end'])) {
          $time_string .= t(" to ") . $date['end'];
        }
      }
      $date_formatted = date('d-m-Y', strtotime($date['date']));
      $dates[$date['date']] = date('d-m-Y', strtotime($date_formatted)) . $time_string;
    }

    $form['date'] = array(
      '#title' => t('When do you want to go?'),
      '#type' => 'radios',
      '#options' => $dates,
      '#default_value' => 0,
    );
  }

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Move event"),
    '#name' => 'edit',
    '#submit' => array($params['submit']),
    '#validate' => array($params['validate']),
    '#ajax' => array(
      'callback' => 'culturefeed_calendar_edit_calendar_event_form_ajax',
      'wrapper' => 'calendar-form',
     ),
  );

  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t("Cancel"),
    '#name' => 'cancel',
    '#submit' => array($params['submit']),
  );

  $form_state['params'] = $params;

  return $form;
}

/**
 *
 * @param type $form
 * @param type $form_state
 */
function culturefeed_calendar_edit_calendar_event_form_submit($form, $form_state) {

 if ($form_state['triggering_element']['#name'] == 'cancel') {
    drupal_set_message(t('Your calendar event was not changed.'));
    return;
  }

  $activity = $form_state['params']['activity'];
  $date = $form_state['values']['date'];

  // Not numeric id = activity on service.
  $success = TRUE;
  if (!is_numeric($activity->id) != 0) {
    try {

      DrupalCultureFeed::deleteActivity($activity->id);

      $new_activity = $activity;
      $new_activity->value = strtotime($date);

      DrupalCultureFeed::createActivity($new_activity);
    }
    catch (Exception $e) {
      $success = FALSE;
      watchdog_exception('culturefeed_calendar', $e);
    }
  }
  // No uuid = anonymous user. Update in cookie.
  else {
    culturefeed_calendar_update_calendar_event_cookie($activity, $date);
  }

  if ($success) {
    drupal_set_message(t('Your calendar event was updated.'));
  }
  else {
    drupal_set_message(t('An error occured while saving your calendar'), 'error');
  }

  drupal_goto('/culturefeed/calendar');

}

/**
 *
 * @param type $form
 * @param type $form_state
 */
function culturefeed_calendar_plan_calendar_event_form_submit($form, $form_state) {

  $node_title = $form_state['params']['node_title'];
  $node_id = $form_state['params']['node_id'];
  $activity_id = $form_state['params']['activity_id'];

  $date = $form_state['values']['date'];

  if ($form_state['triggering_element']['#name'] == 'add') {
    try {
      $activity = DrupalCultureFeed::loadActivity($activity_id);

      DrupalCultureFeed::deleteActivity($activity_id);
      $activity->value = strtotime($date);
      $activity->type = CultureFeed_Activity::TYPE_IK_GA;

      DrupalCultureFeed::createActivity($activity);
      drupal_goto('agenda/e/' . $node_title . '/' . $node_id);
    }
    catch (Exception $e) {
      $success = FALSE;
      watchdog_exception('culturefeed_ui', $e);
    }
  }
  elseif ($form_state['triggering_element']['#name'] == 'cancel') {
    drupal_set_message(t('The event was not added to your calendar.'));
    drupal_goto('agenda/e/' . $node_title . '/' . $node_id);
  }

}

/**
 * Helper function that get form parameters for edit/add/delete activity-forms.
 *
 * @param type $event
 * @param type $activity_id
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
 * Helper function to sort user activities by type.
 */
function _culturefeed_calendar_sort_user_activities_by_type($activities) {

  $activities_by_type = array();

  foreach($activities->objects as $activity) {
    $activities_by_type[$activity->type][] = $activity;
  }

  return $activities_by_type;

}

/**
 * Get twelve months names, starting with current month.
 */
function _culturefeed_calendar_months_get_nav_months($x) {

  $time = mktime(NULL, NULL, NULL, (int)date("n") + ($x), NULL, NULL);

  return format_date($time, 'custom', 'F');

}