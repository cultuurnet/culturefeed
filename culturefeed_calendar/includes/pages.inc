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
 * Page callback to show a users shared calendar page.
 */
function culturefeed_calendar_page_user_activities($user_id) {

  $activities = culturefeed_calendar_get_user_activities(TRUE, $user_id);
  return theme('culturefeed_calendar_page', array('activities' => $activities, 'user_id' => $user_id));

}

/**
 * Page callback to show the calendar share page.
 */
function culturefeed_calendar_page_share_my_calendar() {

  return theme('culturefeed_calendar_share_page');

}

/**
 * Page callback to show the calendar settings page.
 */
function culturefeed_calendar_page_settings() {

  $form = drupal_get_form('culturefeed_calendar_settings_form');

  return theme('culturefeed_calendar_settings_page', array('settings_form' => $form));

}

/**
 * Page callback to like items
 */
function culturefeed_calendar_like(CultuurNet\Search\ActivityStatsExtendedEntity $event, $request_type = 'ajax') {

    $params = _culturefeed_calendar_get_event_params($event);
    $params['title'] = t('Add !title to your calendar', array('!title' => $params['node_title']));
    $params['button_text'] = t('Add');
    $params['use_ajax'] = $request_type == 'ajax';

    culturefeed_calendar_add_activity($params['node_id'], $params['node_title']);

    if ($request_type == 'ajax') {

      // No form should be shown, redirect to the detailpage.
      $commands = array();

      if (isset($_GET['destination'])) {
        $url = url($_GET['destination']);
      }
      else {
        $url = culturefeed_search_detail_url('event', $event->getId(), $event->getTitle(culturefeed_get_preferred_language()));
      }
      $commands[] = culturefeed_ajax_command_goto($url);


      ajax_deliver(array('#type' => 'ajax', '#commands' => $commands));
      exit;

    }
}


/**
 * Page callback to add items to the calendar.
 */
function culturefeed_calendar_add_to_calendar(CultuurNet\Search\ActivityStatsExtendedEntity $event, $request_type = 'ajax') {

  $params = _culturefeed_calendar_get_event_params($event);
  $params['title'] = t('When do you want to go?');
  $params['button_text'] = t('Add');
  $params['use_ajax'] = $request_type == 'ajax';

  // If no form should be shown, we can create the activity.
  if ($params['get_form']) {
    $form = drupal_get_form('culturefeed_calendar_form', $params);
  }
  else {
    try {
      if (!empty($params['dates'][0]['date'])) {
        $date = $params['dates'][0]['date'];
        if (!empty($params['dates'][0]['start'])) {
          $date .= ' ' . $params['dates'][0]['start'];
        }
      }

      culturefeed_calendar_add_activity($params['node_id'], $params['node_title'], $date);
      //drupal_set_message(t('The event has been added to your calendar.'));
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
      if (isset($_GET['destination'])) {
        $url = url($_GET['destination']);
      }
      else {
        $url = culturefeed_search_detail_url('event', $event->getId(), $event->getTitle(culturefeed_get_preferred_language()));
      }
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
  $params['title'] = t('When do you want to go?');
  $params['button_text'] = t('Update event');
  $params['use_ajax'] = $request_type == 'ajax';
  $params['activity'] = $activity;

  if ($params['get_form']) {
    $form = drupal_get_form('culturefeed_calendar_form', $params);

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
  else {

    if (!empty($params['dates'][0]['date'])) {
      $date = $params['dates'][0]['date'];
      if (!empty($params['dates'][0]['start'])) {
        $date .= ' ' . $params['dates'][0]['start'];
      }
    }

    culturefeed_calendar_update_calendar_event($activity, $date);

    if ($request_type == 'ajax') {
      $commands = array();
      $commands[] = culturefeed_ajax_command_goto(url(culturefeed_calendar_get_url()));
      ajax_deliver(array('#type' => 'ajax', '#commands' => $commands));
      exit;
    }
    else {
      drupal_goto(url(culturefeed_calendar_get_url()));
    }
  }
}

/**
 * Form edit/add an event to the calendar.
 */
function culturefeed_calendar_form($form, &$form_state, $params) {

  $submit_function = isset($params['activity']) ? 'culturefeed_calendar_edit_calendar_event_form_submit' : 'culturefeed_calendar_add_to_calendar_form_submit';

  // Activity is passed when editing.
  if (!empty($params['activity'])) {
    $selected_date = strtotime(culturefeed_calendar_get_selected_date($params['activity']));
    $edit = !empty($selected_date) ? TRUE : FALSE;
  }
  else {
    $edit = FALSE;
  }

  drupal_set_title($params['title']);

  // Only show possible dates for periods where calendar summary is available
  /*$possible_dates = '';
  if (($params['type'] == 'period') && $params['calendar_summary']) {
    $info = '<label>' . t('Possible dates') . ':' . '</label><p>' . $params['calendar_summary'] . '</p>';
  }

  $form['info'] = array(
    '#type' => 'markup',
    '#markup' => $possible_dates,
  );*/

  // periode of permanent
  if (($params['type'] == 'period') || ($params['type'] == 'permanent')) {

    $help = t('Choose a date');

    if ($params['type'] == 'period') {
      // Don't add if date is in the past.
      $timestamp = strtotime($params['date_from']);
      if ($timestamp < $_SERVER['REQUEST_TIME']) {
        $start_date = format_date($_SERVER['REQUEST_TIME'], 'custom', 'Y-m-d');
      }
      else {
        $start_date = $params['date_from'];
      }

      $help = t('Choose a date between ') . " " . format_date(strtotime($start_date), 'custom', 'd/m/Y') . " ". t('and') . " " . format_date(strtotime($params['date_to']), 'custom', 'd/m/Y') . ".";

      if($params['when_lg'] !== '') {
        $help .= " " . $params['when_lg'] ;
      }
    }
    elseif ($params['type'] == 'permanent') {
      $start_date = format_date($_SERVER['REQUEST_TIME'], 'custom', 'Y-m-d');
    }

    $form['date'] = array(
      '#title' => $help,
      '#type' => 'date_popup',
      '#date_format' => 'd/m/Y',
      '#datepicker_options' => array(
        'minDate' => format_date(strtotime($start_date), 'custom', 'd/m/Y'),
        'maxDate' => format_date(strtotime($params['date_to']), 'custom', 'd/m/Y'),
      ),
      '#default_value' => $edit ? format_date($selected_date, 'custom', 'Y-m-d H:i') : $start_date,
    );
  }
  // timestamps.
  elseif ($params['type'] == 'timestamps') {
    $dates = array();

    foreach ($params['dates'] as $key => $date) {

      $full_date = $date['date'];
      if (!empty($date['start'])) {
        $full_date .= ' ' . $date['start'];
      }

      // Don't add if date is in the past.
      $timestamp = strtotime($full_date);
      if ($timestamp < $_SERVER['REQUEST_TIME']) {
        continue;
      }

      if (!empty($date['start'])) {
        $starthour = format_date($timestamp, 'custom', 'H:i');
        if (!empty($date['end'])) {
          $endhour = format_date(strtotime($date['date'] . ' ' . $date['end']), 'custom', 'H:i');
          $dates[$date['date'] . ' ' . $starthour] = t('!date from !starthour to !endhour', array('!date' => format_date(strtotime($date['date']), 'custom', 'd/m/Y'), '!starthour' => $starthour, '!endhour' => $endhour));
        }
        else {
          $dates[$date['date']  . ' ' .  $starthour] = t('!date from !starthour', array('!date' => format_date(strtotime($date['date']), 'custom', 'd/m/Y'), '!starthour' => $starthour));
        }
      }
      else {
        $dates[$date['date']] = t('!date', array('!date' => format_date(strtotime($date['date']), 'custom', 'd/m/Y')));
      }

    }

    $form['date'] = array(
      '#title' => t('When do you want to go?'),
      '#type' => 'radios',
      '#options' => $dates,
      '#default_value' => $edit ? format_date($selected_date, 'custom', 'd/m/Y') : '',
    );
  }

  // Removed actions ($form['actions']['submit']) because that didn't work, we have to investigate
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $params['button_text'],
    '#name' => 'add',
    '#submit' => array($submit_function),
    '#validate' => array('culturefeed_calendar_form_validate'),
  );

  if (!$edit) {
    $form['no_date'] = array(
      '#type' => 'submit',
      '#value' => t("I don't know yet"),
      '#name' => 'no_idea_yet',
      '#submit' => array($submit_function),
      '#validate' => array('culturefeed_calendar_form_validate'),
      '#attributes' => array(
        'class' => array('btn', 'btn-link')
      ),
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



  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t("Cancel"),
    '#name' => 'cancel',
    '#submit' => array($submit_function),
    '#limit_validation_errors' => array(),
  );

  if ($params['use_ajax']) {
    // Removed actions ($form['actions']['submit']['#ajax']) because that didn't work, we have to investigate
    $form['submit']['#ajax'] = array(
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
  if (isset($_GET['destination'])) {
    $url = url($_GET['destination']);
  }
  elseif (isset($form_state['params']['activity'])) {
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

  // No idea yet => reset date.
  if ($form_state['clicked_button']['#name'] == 'no_idea_yet') {
    $form_state['values']['date'] = '';
  }

  if ($form_state['params']['type'] == 'period' && !empty($form_state['values']['date'])) {
    $date = strtotime($form_state['values']['date']);
    $date_from = strtotime($form_state['params']['date_from']);
    $date_to = strtotime($form_state['params']['date_to']);
    $today = mktime(0, 0, 0);

    if ($date < $today) {
      form_set_error('date', t('You selected a date in the past. Please select a date in the future.'));
      $form_state['params']['validation_error'] = TRUE;
    }
    // Check if the chosen date is between the start and end date of the event.
    elseif (($date < $date_from) || ($date > $date_to)) {
      if ($date_from < $today) {
        $date_start = $_SERVER['REQUEST_TIME'];
      }
      else {
        $date_start = $date_from;
      }

      form_set_error('date', t('Enter a date between ') . date('d/m/Y', $date_start) . t(' and ') . date('d/m/Y', $date_to) . '.');
      $form_state['params']['validation_error'] = TRUE;
    }
    // Check if the event is opened on the chosen day.
    elseif (!empty($form_state['params']['week_scheme'])) {
      $week_scheme = $form_state['params']['week_scheme'];
      $day = strtolower(date('l', $date));
      $day_allowed = $week_scheme->getDay($day)->getopenType() == 'closed' ? FALSE : TRUE;

      if (!$day_allowed) {
        $form_state['params']['validation_error'] = TRUE;
        form_set_error('date', t('The event is closed on ') . t($day) . '. ' . t('Please choose another day') . '.');
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
  $form_state['redirect'] = culturefeed_search_detail_path('event', $node_id, $node_title);

  if ($form_state['triggering_element']['#name'] == 'cancel') {
    return;
  }

  $date = $form_state['values']['date'];

  try {
    culturefeed_calendar_add_activity($node_id, $node_title, $date);
    //drupal_set_message(t('The event has been added to your calendar.'));
  }
  catch (Exception $e) {
    drupal_set_message(t('There was an error while adding to your calendar'), 'error');
  }

}

/**
 * Submit handler for editing calendar events.
 */
function culturefeed_calendar_edit_calendar_event_form_submit($form, &$form_state) {

  $form_state['redirect'] = 'culturefeed/calendar';
  if ($form_state['triggering_element']['#name'] == 'cancel' || empty($form_state['values']['date'])) {
    return;
  }

  $activity = $form_state['params']['activity'];
  $date = $form_state['values']['date'];

  culturefeed_calendar_update_calendar_event($activity, $date);
}

/**
 * Page callback to delete events from the calendar.
 */
function culturefeed_calendar_delete_calendar_event($activity, $request_type = 'ajax') {

  $event = culturefeed_agenda_event_load($activity->nodeId);
  $params = _culturefeed_calendar_get_event_params($event);
  $params['title'] = t('Remove event');
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
    '#markup' => '<label>' . t('Are you sure you want to remove \'!title\' from your calender?', array('!title' => $params['node_title'])) . '</label>',
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Remove'),
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

  $event_detail = $entity->getDetails()
    ->getDetailByLanguage(culturefeed_search_get_preferred_language());
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
  $params['type'] = '';

  if ($calendar_type == 'CultureFeed_Cdb_Data_Calendar_Permanent') {
    $params['type'] = 'permanent';
  }
  elseif ($calendar_type == 'CultureFeed_Cdb_Data_Calendar_PeriodList') {
    $params['date_from'] = $calendar->current()->getDateFrom();
    $params['date_to'] = $calendar->current()->getDateTo();
    $params['type'] = 'period';
    $params['week_scheme'] = $calendar->current()->getWeekScheme();
    // Format Calendar details to readable & themable HTML.
    $html_formatter = culturefeed_agenda_get_html_calendar_formatter();
    $variables['when_lg'] = '';
    try {
      $params['when_lg'] = $html_formatter->format($calendar, 'lg');
    }
    catch (Exception $e) {
    }

    // if date-to = today, pass modal-form and direct
    if ($calendar->current()->getDateTo() == date('Y-m-d', $_SERVER['REQUEST_TIME'])) {
      $params['dates'][]['date'] = $params['date_to'];
      $params['get_form'] = FALSE;
    }

  }
  elseif ($calendar_type == 'CultureFeed_Cdb_Data_Calendar_TimestampList') {

    $params['type'] = 'timestamps';
    $params['dates'] = array();

    $i = 0;
    $dates = array();
    while ($calendar->valid()) {
      $endtime = $calendar->current()->getEndTime();
      if ($endtime == NULL || $endtime == "00:00:00") {
        $endtime = "23:59:59";
      }
      if (DateTime::createFromFormat('Y-m-d H:i:s', $calendar->current()->getDate() . " " . $endtime)->getTimestamp() > $_SERVER['REQUEST_TIME']) {
        $dates[$i]['date'] = $calendar->current()->getDate();
        $dates[$i]['start'] = $calendar->current()->getStartTime();
        $dates[$i]['end'] = $calendar->current()->getEndTime();
      }
      $calendar->next();
      $i++;
    }

    if (count($dates) == 1) {
      /* don't show a modal */
      $params['get_form'] = FALSE;
      /* reindex dates */
      $dates = array_values($dates);
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
      'first_day' => format_date($time, 'custom', '1/m/Y'),
      'last_day' => format_date($time, 'custom', 't/m/Y'),
    );
    $months[] = $item;

  }

  return $months;
}

/**
 * Form to delete a calendar event.
 */
function culturefeed_calendar_settings_form($form, &$form_state) {

  try {
    $cf_user = DrupalCultureFeed::getLoggedInUser();
  }
  catch (Exception $ex) {
    drupal_set_message(t('There was an error while loading the user information'), 'error');
    return $form;
  }

  $form['visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Who can see your calendar'),
    '#default_value' => !empty($cf_user->privacyConfig->calendarId) ? $cf_user->privacyConfig->calendarId : CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC,
    '#options' => array(
      CultureFeed_UserPrivacyConfig::PRIVACY_PRIVATE => t('Only me'),
      CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC => t('Everyone'),
    ),
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#name' => 'save',
    '#submit' => array('culturefeed_calendar_settings_form_submit'),
  );

  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), 'culturefeed/calendar', array('attributes' => array('class' => array('btn', 'btn-link'))))
  );

  return $form;

}


/**
 * Submit handler for the calendar settings form.
 */
function culturefeed_calendar_settings_form_submit($form, &$form_state) {
  try {
    $cf_user = DrupalCultureFeed::getLoggedInUser();

    // Update the user privacy.
    $cf_user->privacyConfig->calendarId = $form_state['values']['visibility'];
    DrupalCultureFeed::updateUserPrivacy($cf_user->id, $cf_user->privacyConfig);

    // If the privacy is set to not published, unset the calenderId.
    if ($cf_user->privacyConfig->calendarId == CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC) {
      $cf_user->calendarId = $cf_user->id;
    }
    else {
      $cf_user->calendarId = '';
    }
    DrupalCultureFeed::updateUser($cf_user, array('calendarId'));

    if ($cf_user->privacyConfig->calendarId == CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC) {
      drupal_set_message(t('Changes succesfully saved.') . ' ' . l(t('Go to your calendar'), culturefeed_calendar_get_url()) .  ' ' . t('or') . ' ' . l(t('share it with your friends'), 'culturefeed/calendar/share') . '.');
    }
    else {
      drupal_set_message(t('Changes succesfully saved.') . ' ' . l(t('Go to your calendar'), culturefeed_calendar_get_url()));
    }

  }
  catch (Exception $ex) {
    drupal_set_message(t('There was an error while saving the form'), 'error');
  }


}
