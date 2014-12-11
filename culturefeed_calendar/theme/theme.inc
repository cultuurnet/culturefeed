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

  drupal_add_library('system', 'drupal.ajax');
  drupal_add_library('system', 'jquery.cookie');

  if ($variables['button']['action'] == 'view') {

    $variables['button']['class'] = 'view-calendar';
    $variables['button']['path'] = 'culturefeed/calendar';
    if ($variables['button']['location'] == 'content') {
      $variables['button']['description'] = t('This activity is added to your OuTcalendar.');
      $variables['button']['text'] = t('View OuTcalendar');
    }
    elseif ($variables['button']['location'] == 'footer') {
      $variables['button']['text'] = t('To your OuTcalendar');
    }

  }
  elseif ($variables['button']['action'] == 'add') {

    $variables['button']['class'] = 'add-to-calendar';
    $variables['button']['path'] = 'culturefeed/calendar/add/' . $variables['button']['item_id'] . '/nojs';
    if ($variables['button']['location'] == 'content') {
      $variables['button']['text'] = t('Add to my OuTcalendar');
    }
    elseif ($variables['button']['location'] == 'footer') {
      $variables['button']['text'] = t('Add');
    }

  }

}


/**
 * Theme the total activities profile box item.
 */
function theme_culturefeed_calendar_total_activities_profile_box_item($variables) {
  return l(format_plural($variables['total'], '@count unsaved activity', '@count unsaved activities'), 'culturefeed/calendar');
}

function culturefeed_calendar_preprocess_culturefeed_calendar_activities_going(&$variables) {

  $activities = $variables['activities'];
  $months = $variables['month_names'];

  $variables['months'] = array_fill_keys(array_map(function($element){return $element['full_month'];}, $months), array());

  foreach ($activities as $activity) {
    if ($selected_date = culturefeed_calendar_get_selected_date($activity)) {
      $month = format_date($selected_date, 'custom', 'F');
      $variables['months'][$month][] = $activity;
    }
  }
}

/**
 * Preprocess activity mini summary.
 */
function culturefeed_calendar_preprocess_culturefeed_calendar_activity_mini(&$variables) {

  $activity = $variables['activity'];
  $node_id = $activity->nodeId;

  $activity_id = $activity->id;
  $item = culturefeed_search_item_load($node_id, 'event');
  $variables['item'] = $item;

  module_load_include('inc', 'culturefeed_agenda', 'theme/theme');
  $function = 'culturefeed_agenda_preprocess_culturefeed_event_summary';
  if (function_exists($function)) {
    $function($variables);
  }

  // Plan link.
  $variables['edit_link']['url'] = '/culturefeed/calendar/edit/'. $activity_id . '/nojs';
  $variables['edit_link']['text'] = t('Inplannen');
  // Remove link.
  $variables['delete_link']['url'] = '/culturefeed/calendar/delete/'. $activity_id . '/nojs';
  $variables['delete_link']['text'] = t('Verwijderen');

}

/**
 * Preprocess activity summary.
 *
 * @param type $variables
 */
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
  if ($selected_date = culturefeed_calendar_get_selected_date($activity)) {
    $variables['date'] = format_date($selected_date, 'custom', 'l j F Y H:i');
  }

  $activity_id = $activity->id;
  $node_id = $activity->nodeId;

  // Edit link.
  $variables['edit_link']['url'] = '/culturefeed/calendar/edit/'. $activity_id . '/nojs';
  $variables['edit_link']['text'] = t('Verplaatsen');
  $variables['edit_link']['show'] = culturefeed_calendar_event_has_multiple_dates($variables['calendar']);
  // Remove link.
  $variables['delete_link']['url'] = '/culturefeed/calendar/delete/'. $activity_id . '/nojs';
  $variables['delete_link']['text'] = t('Verwijderen');

}

/**
 *
 * @param type $variables
 */
function culturefeed_calendar_preprocess_culturefeed_calendar_button_hover(&$variables) {
  drupal_add_css(drupal_get_path('module', 'culturefeed_calendar') . '/css/culturefeed_calendar.css');
  $variables['url'] = 'culturefeed/oauth/connect';
  $variables['options'] = array(
    'query' => array(
      'destination' => 'culturefeed/calendar/add-cookie-to-calendar',
    ),
  );
}


