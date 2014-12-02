<?php
/**
 * @file
 * Page callbacks for the calendar module.
 */


/**
 * Page callback to show the calendar page.
 */
function culturefeed_calendar_page_my_activities () {

  $activities = culturefeed_calendar_get_user_activities();

  $output = '';
  foreach ($activities->objects as $activity) {
    $output .= '<div>' . $activity->nodeTitle . '</div>';
  }

  return $output;
}

/**
 * Page callback to add items to the calendar
 */
function culturefeed_calendar_add_to_calendar($id) {

  $event = culturefeed_agenda_event_load($id);
  $calendar = $event->getEntity()->getCalendar();
  $calendar_type = get_class($calendar);
  $lang_code = $GLOBALS['language']->language;
  $title = $event->getTitle($lang_code);


  if ($calendar_type == 'CultureFeed_Cdb_Data_Calendar_PeriodList') {

  }
  elseif ($calendar_type == 'CultureFeed_Cdb_Data_Calendar_TimestampList') {
    while ($calendar->valid()) {
      $dates[] = $calendar->current()->getDate();
      $calendar->next();
    }

    if (count($dates) == 1) {
      culturefeed_calendar_add_activity($id, $title, $dates);
    }
    else {

      $params['dates'] = $dates;
      $params['id'] = $id;
      $params['title'] = $title;
      $form = drupal_get_form('culturefeed_calendar_add_to_calendar_form', $params);
      return $form;
    }
  }
}

/**
 * Helper function to add an activity to the calendar.
 */
function culturefeed_calendar_add_activity($nodeId, $nodeTitle, $value) {
  $activity = new CultureFeed_Activity();
  $activity->type = CultureFeed_Activity::TYPE_IK_GA;
  $activity->contentType = 'event';
  $activity->nodeId = $nodeId;
  $activity->nodeTitle = $nodeTitle;
  $activity->userId = DrupalCultureFeed::getLoggedInUserId();
  $activity->value = json_encode($value);
  DrupalCultureFeed::createActivity($activity);
}

/**
 * Form to choose dates before adding an event to the calendar.
 */
function culturefeed_calendar_add_to_calendar_form($form, &$form_state, $params) {
  $form['calendar_dates'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add ' . $params['title'] . ' to you OuTcalendar'),
    '#description' => t('When do you want to go?'),
    '#tree' => TRUE,
  );
  foreach ($params['dates'] as $key => $date) {
    $form['calendar_dates'][$key] = array(
      '#type' => 'checkbox',
      '#title' => $date,
      '#default_value' => 0,
    );
  }
  $form['calendar_dates']['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Add"),
  );
  $form['calendar_dates']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t("Cancel"),
  );

  return $form;
}

/**
 * Submit the add to calendar form.
 */
function culturefeed_calendar_add_to_calendar_form_submit($form, $form_state) {
  dsm($form_state, 'state');
}

