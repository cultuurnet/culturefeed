<?php

/**
 * @file
 * Theming / preprocess functions for culturefeed_calendar.
 */

/**
 * Preprocess the variables for the calendar buttons.
 * @see culturefeed-calendar-button.tpl.php
 */
function culturefeed_calendar_preprocess_culturefeed_calendar_button(&$variables) {

  if ($variables['button']['action'] == 'view') {

    $variables['button']['url'] = 'culturefeed/calendar';
    if ($variables['button']['location'] == 'content') {
      $variables['button']['description'] = t('This activity is added to your OuTcalendar.');
      $variables['button']['text'] = t('View OuTcalendar');
    }
    elseif ($variables['button']['location'] == 'footer') {
      $variables['button']['text'] = t('To your OuTcalendar');
    }

  }
  elseif ($variables['button']['action'] == 'add') {

    $variables['button']['url'] = 'culturefeed/calendar/add/' . $variables['button']['item_id'] . '/ajax';
    if ($variables['button']['location'] == 'content') {
      $variables['button']['text'] = t('Add to my OuTcalendar');
    }
    elseif ($variables['button']['location'] == 'footer') {
      $variables['button']['text'] = t('Add');
    }

  }

}

/**
 * Preprocess the variables for one of the event templates on calendar page.
 */
function culturefeed_calendar_preprocess_culturefeed_calendar_activities_liked(&$variables) {
  //haha
}

function culturefeed_calendar_preprocess_culturefeed_calendar_activities_going(&$variables) {

  $activities = $variables['activities'];
  $variables['months'] = array();

  foreach ($activities as $activity) {
    $month = format_date($activity->value, 'custom', 'F');
    $variables['months'][$month][] = $activity;
  }
}

/**
 * Preprocess the variables for one of the event templates on calendar page.
 */
function culturefeed_calendar_preprocess_culturefeed_calendar_activity_mini(&$variables) {

  $activity = $variables['activity'];
  $item = culturefeed_search_item_load($activity->nodeId, 'event');
  $variables['item'] = $item;

  module_load_include('inc', 'culturefeed_agenda', 'theme/theme');
  $function = 'culturefeed_agenda_preprocess_culturefeed_event_summary';
  if (function_exists($function)) {
    $function($variables);
  }

}


function culturefeed_calendar_preprocess_culturefeed_calendar_activity_summary(&$variables) {

  $activity = $variables['activity'];
  $item = culturefeed_search_item_load($activity->nodeId, 'event');
  $variables['item'] = $item;

  module_load_include('inc', 'culturefeed_agenda', 'theme/theme');
  $function = 'culturefeed_agenda_preprocess_culturefeed_event_summary';
  if (function_exists($function)) {
    $function($variables);
  }

  // Activity details.
  $variables['date'] = format_date($activity->value, 'custom', 'l j F Y');

  // Personal calendar buttons.
  $variables['personal_calendar']['button_footer'] = culturefeed_calendar_render_button($item, 'footer');
  $variables['personal_calendar']['button_content'] = culturefeed_calendar_render_button($item,'content');

}


