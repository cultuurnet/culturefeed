<?php

/**
 * @file
 * Theming / preprocess functions for culturefeed_calendar.
 */

/**
 * Preprocess the calendar page.
 * @see culturefeed-calendar-page.tpl.php
 */
function culturefeed_calendar_preprocess_culturefeed_calendar_page(&$variables) {


  $loggedin_cf_uid = DrupalCultureFeed::getLoggedInUserId();

  // Check if we need to show the calendar for the logged in user or for another user.
  try {
    if (isset($variables['user_id']) && $loggedin_cf_uid !== $variables['user_id']) {
      $my_calendar = FALSE;
      $cf_user = DrupalCultureFeed::getUser($variables['user_id']);
    }
    else {
      $my_calendar = TRUE;
      $cf_user = DrupalCultureFeed::getLoggedInUser();
    }
  }
  catch (Exception $e) {
    $variables['deny_access'] = TRUE;
    return;
  }

  // Check if another persons calendar is shared.
  if (!$my_calendar && empty($cf_user->calendarId)) {
    $variables['deny_access'] = TRUE;
  }
  else {
    $variables['deny_access'] = FALSE;
  }

  $variables['sidebar'] = 'empty';

  // Top nav with month names.
  $months = _culturefeed_calendar_get_nav_months();
  $variables['nav_months'] = theme('culturefeed_calendar_nav_months', array('months' => $months));

  // Sort activities by type.
  $activities = $variables['activities'];
  $activities_by_type = array();
  if (!empty($activities->objects)) {
    foreach($activities->objects as $activity) {
      $activity_type = $activity->type;
      if (!culturefeed_calendar_get_selected_date($activity)) {
        $activity_type = CultureFeed_Activity::TYPE_LIKE;
      }
      $activities_by_type[$activity_type][] = $activity;
    }
  }

  $variables['not_yet_planned'] = '';
  $variables['planned'] = '';
  if (!empty($activities_by_type)) {
    // Print the 'LIKED' activities.
    if (isset($activities_by_type[CultureFeed_Activity::TYPE_LIKE]) && $my_calendar) {

      //Remove activities that are already started.
      foreach ($activities_by_type[CultureFeed_Activity::TYPE_LIKE] as $key => $activity_liked) {
        $event = culturefeed_agenda_event_load($activity_liked->nodeId);
        if (culturefeed_calendar_get_event_status($event) !== CULTUREFEED_CALENDAR_EVENT_PLANNABLE) {
          unset($activities_by_type[CultureFeed_Activity::TYPE_LIKE][$key]);
        }
      }

      $variables['not_yet_planned'] = theme('culturefeed_calendar_activities_liked',
        array('activities' => $activities_by_type[CultureFeed_Activity::TYPE_LIKE]));
    }
    // Print the 'IK GA' activities.
    if (isset($activities_by_type[CultureFeed_Activity::TYPE_IK_GA])) {
      $variables['planned'] = theme('culturefeed_calendar_activities_going',
        array('activities' => $activities_by_type[CultureFeed_Activity::TYPE_IK_GA], 'month_names' => $months, 'my_calendar' => $my_calendar));
    }
    // Print the save cookie button.
    if (!$loggedin_cf_uid && !empty($activities->objects) && $my_calendar) {
      $variables['login_url'] = url('culturefeed/oauth/connect', array('query' => drupal_get_destination()));
    }
  }

  if ($loggedin_cf_uid || (!$loggedin_cf_uid && !$my_calendar)) {
    // Set username.
    if (empty($cf_user->givenName)) {
      $name = $cf_user->nick;
    }
    else {
      $name = $cf_user->givenName . ' ' . $cf_user->familyName;
    }
    $variables['user_name'] = $name;
  }

  if ($loggedin_cf_uid && $my_calendar) {
    $variables['share_calendar_path'] = 'culturefeed/calendar/share';
    $variables['calendar_settings_path'] = 'culturefeed/calendar/settings';

    if ($cf_user->privacyConfig->calendarId == CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC) {
      $variables['shared'] = TRUE;
      $variables['data_content'] = theme('culturefeed_calendar_share_page');
      $variables['calendar_share_path'] = 'culturefeed/calendar/' . $cf_user->id;
      $variables['calendar_share_url'] = url($variables['calendar_share_path'], array('absolute' => TRUE));
    }
    else {
      $variables['shared'] = FALSE;
      $variables['data_content'] = theme('culturefeed_calendar_share_page');
    }
  }

  culturefeed_calendar_preprocess_culturefeed_calendar_share_page($variables);
  culturefeed_calendar_preprocess_culturefeed_calendar_metatags($variables);
}

/**
 * Preprocess the variables for the calendar buttons.
 * @see culturefeed-calendar-button.tpl.php
 */
function culturefeed_calendar_preprocess_culturefeed_calendar_button(&$variables) {

  drupal_add_library('system', 'drupal.ajax');
  drupal_add_library('system', 'jquery.cookie');

  if (!culturefeed_is_culturefeed_user()) {
    drupal_add_css(drupal_get_path('module', 'culturefeed_calendar') . '/css/culturefeed_calendar_anonymous.css');
  }

  $variables['classes_array'] = array(
    'calendar-button',
    'calendar-button-' . $variables['button']['location'],
  );
  $variables['finished'] = $variables['status'] == CULTUREFEED_CALENDAR_EVENT_FINISHED;
  $variables['started'] = $variables['status'] == CULTUREFEED_CALENDAR_EVENT_STARTED;

  if (!$variables['finished'] && $variables['button']['action'] == 'view') {

    $variables['classes_array'][] = 'btn-view-calendar';

    $variables['button']['options'] = array();
    $variables['button']['path'] = 'culturefeed/calendar';
    if ($variables['button']['location'] == 'content') {
      $variables['button']['description'] = t('This event is added to your calendar.');
      $variables['button']['text'] = t('View calendar');
    }
    elseif ($variables['button']['location'] == 'aside') {
      $variables['button']['text'] = t('to your calendar');
    }

  }
  elseif ($variables['button']['action'] == 'add') {

    $variables['classes_array'][] = 'btn-add-calendar';

    $variables['button']['options'] = array(
      'query' => drupal_get_destination(),
    );

    $variables['button']['path'] = 'culturefeed/calendar/add/' . $variables['button']['item_id'] . '/nojs';
    if ($variables['button']['location'] == 'content') {
      $variables['button']['text'] = t('Going');
    }
    elseif ($variables['button']['location'] == 'aside') {
      $variables['button']['text'] = t('Going');
    }

  }
  elseif ($variables['button']['action'] == 'like'){

    $variables['classes_array'][] = 'btn-like-calendar';

    $variables['button']['options'] = array(
      'query' => drupal_get_destination(),
    );

    $variables['button']['path'] = 'culturefeed/calendar/like/' . $variables['button']['item_id'] . '/nojs';
    if ($variables['button']['location'] == 'content') {
      $variables['button']['text'] = t('Interested');
    }
    elseif ($variables['button']['location'] == 'aside') {
      $variables['button']['text'] = t('Interested');
    }

  }

  $variables['event_id'] = $variables['button']['item_id'];
  $variables['classes'] = implode(' ', $variables['classes_array']);

}

/**
 * Theme the profile box item for calendar.
 */
function theme_culturefeed_calendar_profile_box_item($variables) {

  if (culturefeed_is_culturefeed_user()) {
    return l(t('Go to your calendar'), 'culturefeed/calendar');
  }
  else {
    return l(t('unsaved events: <span class="unread-activities">0</small>'), 'culturefeed/calendar', array('html' => TRUE));
  }

}

/**
 * Preprocess activities 'going'.
 */
function culturefeed_calendar_preprocess_culturefeed_calendar_activities_going(&$variables) {

  // Get activities and sort them by date to add them to the calendar in the right order.
  $activities = $variables['activities'];
  usort($activities, "culturefeed_calendar_sort_by_date");

  $months = $variables['month_names'];

  $variables['months'] = array_fill_keys(array_map(function($element){return $element['full_month'];}, $months), array());

  foreach ($activities as $activity) {
    if (culturefeed_calendar_activity_is_finished($activity)) {
      continue;
    }

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
  culturefeed_agenda_preprocess_culturefeed_event_summary($variables);

  // Plan link.
  $variables['edit_link']['url'] = url('culturefeed/calendar/edit/'. $activity_id . '/nojs');
  $variables['edit_link']['text'] = t('Schedule');
  // Remove link.
  $variables['delete_link']['url'] = url('culturefeed/calendar/delete/'. $activity_id . '/nojs');
  $variables['delete_link']['text'] = t('Remove');

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
    if (format_date($selected_date, 'custom', 'H:i') == '00:00') {
      $variables['date'] = format_date($selected_date, 'custom', 'l j F Y');
    }
    else {
      $variables['date'] = format_date($selected_date, 'custom', 'l j F Y H:i');
    }
  }

  $activity_id = $activity->id;

  // Edit link.
  $variables['edit_link']['url'] = url('culturefeed/calendar/edit/'. $activity_id . '/nojs');
  $variables['edit_link']['text'] = t('Reschedule');
  $variables['edit_link']['show'] = culturefeed_calendar_event_has_multiple_dates($variables['calendar']);
  // Remove link.
  $variables['delete_link']['url'] = url('culturefeed/calendar/delete/'. $activity_id . '/nojs');
  $variables['delete_link']['text'] = t('Remove');

}

/**
 * Preprocess for add button tootlip.
 *
 * @param type $variables
 */
function culturefeed_calendar_preprocess_culturefeed_calendar_button_hover(&$variables) {

  $variables['url'] = 'culturefeed/oauth/connect';
  $variables['options'] = array(
    'query' => array(
      'destination' => 'culturefeed/calendar',
    ),
  );
}

/**
 * Preprocess the share calendar page.
 *
 * @param type $variables
 */
function culturefeed_calendar_preprocess_culturefeed_calendar_share_page(&$variables) {

  try {
    $cf_user = DrupalCultureFeed::getLoggedInUser();
  }
  catch (Exception $e) {
    drupal_set_message(t('There was an error while searching the user'), 'error');
  }

  $variables['back_to_calendar_path'] =  'culturefeed/calendar';
  $variables['shared'] = FALSE;
  // Calendar is public.
  if ($cf_user && $cf_user->privacyConfig->calendarId == CultureFeed_UserPrivacyConfig::PRIVACY_PUBLIC) {

    $variables['shared'] = TRUE;
    $variables['calendar_share_path'] = 'culturefeed/calendar/' . $cf_user->id;
    $variables['calendar_share_url'] = url($variables['calendar_share_path'], array('absolute' => TRUE));

    culturefeed_search_ui_add_facebook_share();

    $variables['facebook']['url'] = 'https://www.facebook.com/sharer/sharer.php';
    $variables['facebook']['attr'] = array(
      'query' => array('u' => $variables['calendar_share_url']),
      'attributes' => array('class' => 'facebook-share'),
      );

    $variables['facebook_url'] = 'https://www.facebook.com/sharer/sharer.php?u=' . urlencode($variables['calendar_share_url']);
    $variables['messenger_url'] = 'fb-messenger://share/?link=' . urlencode($variables['calendar_share_url']);
    $variables['twitter_url'] = 'https://twitter.com/intent/tweet?text=' . urlencode($variables['calendar_share_url']);

    if (module_exists('forward')) {
      $variables['mail_url'] = url('forward', array('query' => array('path' => $variables['calendar_share_path'])));
    }

  }
}


/**
 * Helper function to create a meta description tag for calendarpages
 * @param type $variables
 */
function culturefeed_calendar_preprocess_culturefeed_calendar_metatags(&$variables) {

  $title_parts = array();
  $title_parts[] = t('Calendar');
  $title_parts[] = variable_get('site_name', '');
  $description = "";
  $og_tags = array();

  // Add meta title tag.
  drupal_set_title(implode(' | ', $title_parts), PASS_THROUGH);

  // Add meta description tag.
  drupal_add_html_head(array(
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'description',
      'content' => $description,
    ),
  ), 'meta_description');

  // Add open graph meta tags.
  $fb_app_id = variable_get('culturefeed_search_ui_fb_app_id', '');
  if (!empty($fb_app_id)) {
    $og_tags['fb:app_id'] = $fb_app_id;
  }

  $og_tags['og:site_name'] = variable_get('site_name', '');

  if($variables['shared']) {
    $og_tags['og:title'] = t('Calendar of ') . $variables['user_name'];
    $og_tags['og:url'] =  $variables['calendar_share_url'];
    $og_tags['og:description'] = t('Discover the calendar of ') . $variables['user_name'] ;
  }
  else {
    $og_tags['og:title'] = t('Make your own calendar');
    $og_tags['og:url'] = url('culturefeed/calendar', array('absolute' => TRUE));
    $og_tags['og:description'] = t('Put activities from ') . variable_get('site_name', '') . t(' in your personal calendar.');
  }

  foreach ($og_tags as $property => $content) {
   drupal_add_html_head(array(
     '#tag' => 'meta',
     '#attributes' => array(
       'property' => $property,
       'content' => $content,
     ),
   ), $property);
  }


}
