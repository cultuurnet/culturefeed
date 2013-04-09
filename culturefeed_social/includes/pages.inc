<?php
/**
 * @file
 * Page callbacks for culturefeed social.
 */

/**
 * Page callback to get the activities for current requested page.
 */
function culturefeed_social_page_ajax_get_activities() {

  $query = new CultureFeed_SearchActivitiesQuery();
  $query->max = CULTUREFEED_SOCIAL_BLOCK_USER_ACTIVITIES_COUNT;
  $query->type = array(
    CultureFeed_Activity::TYPE_LIKE,
    CultureFeed_Activity::TYPE_FACEBOOK,
    CultureFeed_Activity::TYPE_TWITTER,
    CultureFeed_Activity::TYPE_IK_GA,
  );

  if (isset($_GET['culturefeed_uid'])) {
    $query->userId = $_GET['culturefeed_uid'];
  }

  $query->private = FALSE;

  try {
    $activities = DrupalCultureFeed::searchActivities($query);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    return;
  }

  $accounts = culturefeed_get_uids_for_activities($activities->objects);

  $results = array();
  foreach ($activities->objects as $object) {
    $results[] = $items[] = theme('culturefeed_social_user_activity_summary', array('activity' => $activity, 'uid' => $accounts[$activity->userId]));
  }

}