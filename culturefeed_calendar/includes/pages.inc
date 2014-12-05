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
    $output .= theme('culturefeed_calendar_activities_liked', array('activities' => $activities_by_type[CultureFeed_Activity::TYPE_LIKE]));
  }
  // Print the 'IK GA' activities.
  if (isset($activities_by_type[CultureFeed_Activity::TYPE_IK_GA])) {
    $output .= theme('culturefeed_calendar_activities_going', array('activities' => $activities_by_type[CultureFeed_Activity::TYPE_IK_GA]));
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

  if ($calendar_type == 'CultureFeed_Cdb_Data_Calendar_PeriodList') {
    $params['date_from'] = $calendar->current()->getDateFrom();
    $params['date_to'] = $calendar->current()->getDateTo();
    $params['type'] = 'period';
  }
  elseif ($calendar_type == 'CultureFeed_Cdb_Data_Calendar_TimestampList') {
    while ($calendar->valid()) {
      $dates[] = $calendar->current()->getDate();
      $calendar->next();
    }

    if (count($dates) == 1) {
      culturefeed_calendar_add_activity($node_id, $title, $dates[0]);
      return;
    }
    else {
      $params['dates'] = $dates;
      $params['type'] = 'timestamps';
    }
  }

  $form = drupal_get_form('culturefeed_calendar_add_to_calendar_form', $params);

  return $form;

  if ($request_type == 'ajax') {
    $output = drupal_render($form);
    print $output;
  }
  else {
    return $form;
  }
}

/**
 * Form to choose dates before adding an event to the calendar.
 */
function culturefeed_calendar_add_to_calendar_form($form, &$form_state, $params) {

  $form['calendar_dates'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add ') . $params['node_title'] . t(' to your OuTcalendar'),
    '#tree' => TRUE,
  );

  $form['calendar_dates']['info'] = array(
      '#type' => 'markup',
      '#markup' => '<label>' . t($params['node_title'] . t(' runs ') . $params['calendar_summary']  . '.') . '</label>',
    );

  if ($params['type'] == 'period') {

    $form['calendar_dates']['date'] = array(
      '#title' => t('When do you want to go?'),
      '#type' => 'date_popup',
      '#date_format' => 'd-m-Y',
      '#date_year_range' => '-0:+1',
      '#default_value' => '',
    );
  }
  elseif ($params['type'] == 'timestamps') {

    foreach ($params['dates'] as $key => $date) {
      $date_formatted = date('d-m-Y', strtotime($date));
      $dates[$date] = date('d-m-Y', strtotime($date_formatted));
    }

    $form['calendar_dates']['date'] = array(
      '#title' => t('When do you want to go?'),
      '#type' => 'radios',
      '#options' => $dates,
      '#default_value' => 0,
    );
  }

  $form['calendar_dates']['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Add"),
    '#name' => 'add',
    '#submit' => array('culturefeed_calendar_add_to_calendar_form_submit'),
    '#validate' => array('culturefeed_calendar_add_to_calendar_form_validate'),
  );

  $form['calendar_dates']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t("Cancel"),
    '#name' => 'cancel',
    '#submit' => array('culturefeed_calendar_add_to_calendar_form_submit'),
  );

  $form_state['params'] = $params;

  return $form;
}

/**
 * Validate the add to calendar form.
 */
function culturefeed_calendar_add_to_calendar_form_validate($form, $form_state) {

  if ($form_state['params']['type'] == 'period') {
   $date = strtotime($form_state['values']['calendar_dates']['date']);
   $date_from = strtotime($form_state['params']['date_from']);
   $date_to = strtotime($form_state['params']['date_to']);
   if (($date < $date_from) || ($date > $date_to)) {
     form_set_error("'calendar_dates']['date'", t('Enter a date between ') . date('d-m-Y', $date_from) . t(' and ') . date('d-m-Y', $date_to) . '.');
   }
  }
}
/**
 * Submit the add to calendar form.
 */
function culturefeed_calendar_add_to_calendar_form_submit($form, $form_state) {

  $node_title = $form_state['params']['node_title'];
  $node_id = $form_state['params']['node_id'];

  $date = $form_state['values']['calendar_dates']['date'];

  if ($form_state['triggering_element']['#name'] == 'add') {
    culturefeed_calendar_add_activity($node_id, $node_title, $date);
  }
  elseif ($form_state['triggering_element']['#name'] == 'cancel') {
    drupal_set_message(t('The event was not added to your calendar.'));
    drupal_goto('agenda/e/' . $node_title . '/' . $node_id);
  }
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
 * Get twleve months names, starting with current month.
 */
function _culturefeed_calendar_months_get_nav_months($x) {

  $time = mktime(NULL, NULL, NULL, (int)date("n") + ($x), NULL, NULL);

  return format_date($time, 'custom', 'F');

}