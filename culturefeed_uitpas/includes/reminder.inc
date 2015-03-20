<?php
/**
 * @file
 */

/**
 * Toggles the UiTPAS activation reminder message on and off with a cookie.
 */
function culturefeed_uitpas_toggle_reminder() {
  // Drupal has functions to set cookies (see user_cookie_save() later on), but
  // apparently not to retrieve them. So use $_COOKIE directly.
  $current_state = isset($_COOKIE['Drupal_visitor_culturefeed_uitpas_reminder']) ? $_COOKIE['Drupal_visitor_culturefeed_uitpas_reminder'] : 'on';

  if ($current_state == 'on') {
    $new_state = 'off';
  }
  else {
    $new_state = 'on';
  }

  $values = array(
    'culturefeed_uitpas_reminder' => $new_state,
  );

  user_cookie_save($values);

  $referrer = $_SERVER['HTTP_REFERER'];

  if (!$referrer) {
    $referrer = '<front>';
  }
  drupal_goto($referrer);
}
