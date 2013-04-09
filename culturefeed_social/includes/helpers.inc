<?php
/**
 * @file
 * Helper functions for culturefeed social.
 */

/**
 * Get the actvitity filter options.
 */
function culturefeed_social_get_activity_filter_options($active_only = FALSE) {

  $options = array(
    CultureFeed_Activity::TYPE_LIKE => 'Likes',
    CultureFeed_Activity::TYPE_RECOMMEND => 'Aanraden',
    CultureFeed_Activity::TYPE_FOLLOW => 'Pagina volgen',
    CultureFeed_Activity::TYPE_COMMENT => 'Beoordelingen',
  );

  // Return all possible filters.
  if (!$active_only) {
    return $options;
  }

  // Only active filters
  $activity_filters = variable_get('culturefeed_social_activity_filters', array());

  $active_options = array();
  foreach ($activity_filters as $activity_filter) {
    if ($activity_filter) {
      $active_options[$activity_filter] = $options[$activity_filter];
    }
  }

  return $active_options;

}