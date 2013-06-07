<?php
/**
 * @file
 * Theming / preprocess functions for culturefeed userpoints ui.
 */


/**
 * Preprocess the culturefeed userpoints my balance block.
 * @see culturefeed-userpoints-ui-my-balance.tpl.php
 */
function culturefeed_userpoints_ui_preprocess_culturefeed_userpoints_ui_my_balance(&$variables) {

  $variables['points'] = culturefeed_userpoints_ui_real_points($variables['points']);

}

/**
 * Preprocess the culturefeed userpoints my exchange page.
 * @see culturefeed-userpoints-ui-exchange-page.tpl.php
 */
function culturefeed_userpoints_ui_preprocess_culturefeed_userpoints_ui_exchange_page(&$variables) {

  $variables['points'] = culturefeed_userpoints_ui_real_points($variables['points']);

}

/**
 * Theme the message when a user earned some points.
 */
function theme_culturefeed_userpoints_ui_points_earned_message($variables) {

  $points = culturefeed_userpoints_ui_real_points($variables['activity']->points);
  $total_points = culturefeed_userpoints_ui_real_points($variables['activity']->newTotalPoints);
  $message = 'Gefeliciteerd! Je verdiende ' . $points . ' met deze actie en hebt nu ' . $total_points . ' punten.';

  return $message;

}