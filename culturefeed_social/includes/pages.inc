<?php
/**
 * @file
 * Page callbacks for culturefeed social.
 */

/**
 * Page callback to get the activities for current requested page.
 */
function culturefeed_social_page_ajax_get_activities() {

  $requested_page = isset($_GET['page']) ? $_GET['page'] : 0;

  $query = new CultureFeed_SearchActivitiesQuery();
  $query->max = CULTUREFEED_SOCIAL_BLOCK_USER_ACTIVITIES_COUNT;
  $query->start = $requested_page * CULTUREFEED_SOCIAL_BLOCK_USER_ACTIVITIES_COUNT;

  if (isset($_GET['culturefeed_uid'])) {
    $query->userId = $_GET['culturefeed_uid'];
  }

  // Filter on type.
  if (isset($_GET['type'])) {
    $query->type = array($_GET['type']);
  }
  else {
    $query->type = array(
      CultureFeed_Activity::TYPE_LIKE,
      CultureFeed_Activity::TYPE_FACEBOOK,
      CultureFeed_Activity::TYPE_TWITTER,
      CultureFeed_Activity::TYPE_IK_GA,
    );
  }

  $query->private = FALSE;

  $result = new stdClass();
  $result->results = array();
  $result->show_pager = FALSE;
  try {

    $activities = DrupalCultureFeed::searchActivities($query);
    $accounts = culturefeed_get_uids_for_activities($activities->objects);

    foreach ($activities->objects as $activity) {
      $result->results[] = theme('culturefeed_social_user_activity_summary', array('activity' => $activity, 'uid' => $accounts[$activity->userId]));
    }

    $result->show_pager = ($query->max  + $query->start) < $activities->total;

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
  }

  // If we are applying, and we have no results. Make sure we show a no results message.
  if (isset($_GET['new_filter']) && empty($result->results)) {
    $result->results[] = 'Geen activiteiten gevonden';
  }

  drupal_json_output($result);

}