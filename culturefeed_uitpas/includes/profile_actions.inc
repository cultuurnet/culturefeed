<?php

/**
 * @file
 * Helper functions for UiTPAS profile actions.
 */

/**
 * Returns profile actions.
 */
function culturefeed_uitpas_profile_actions_get() {

  $activities = array();
  $uitpas_user = culturefeed_uitpas_get_uitpas_user();

  if ($uitpas_user->user && $uitpas_user->passholder) {

    $activities = array();

    // 2 queries are needed: one for positive points and one for negative
    // points.  The queries also loop to get all results.
    $base_query = new CultureFeed_SearchActivitiesQuery();
    $base_query->userId = $uitpas_user->user->id;
    $base_query->start = 0;
    $base_query->max = 10;
    $base_query->private = TRUE;

    try {

      foreach (array('positive', 'negative') as $activity_points_filter) {

        $query = clone($base_query);
        $query->filterOnActivityPoints = $activity_points_filter;

        do {

          $result_set = DrupalCultureFeed::searchActivities($query);
          $activities = array_merge($activities, $result_set->objects);
          $query->start += $query->max;

        } while ($query->start < $result_set->total);

      }

    }
    catch (Exception $e) {
      watchdog('culturefeed_uitpas_activities', $e);
    }

  }

  // Sort by creation date.
  usort($activities, function($a, $b) {
    /* @var \CultureFeed_Activity $a */
    /* @var \CultureFeed_Activity $b */
    return $b->creationDate - $a->creationDate;
  });

  // Only include last year.
  $activities = array_values(array_filter($activities, function ($activity) {
    /* @var \CultureFeed_Activity $activity */
    return $activity->creationDate > strtotime("-1 year", time());
  }));

  return array(
    '#theme' => 'culturefeed_uitpas_profile_actions',
    '#activities' => $activities,
  );

}

/**
 * Title callback for the profile actions page.
 */
function culturefeed_uitpas_profile_actions_title() {

  return t('Points history');

}
