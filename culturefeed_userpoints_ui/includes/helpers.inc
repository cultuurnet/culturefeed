<?php
/**
 * @file
 * Helper functions to calculate and handle user points.
 */

function culturefeed_userpoints_ui_real_points($points) {
  
  $exchange = variable_get('culturefeed_userpoints_ui_exchange_ratio');
  if (!empty($exchange)) {
    $points = round($points / $exchange);
  }
  
  return $points;
  
}