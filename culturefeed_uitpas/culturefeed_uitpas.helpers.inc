<?php

/**
 * @file
 * Helper functions for Culturefeed uitpas.
 */

/**
 * Returns a culterfeed uitpas user object.
 */
function culturefeed_uitpas_get_uitpas_user() {

  $uitpas_user = new stdClass();
  $uitpas_user->user = NULL;
  $uitpas_user->passholder = NULL;

  try {
    if (DrupalCultureFeed::isCultureFeedUser()) {
      $uitpas_user->user = DrupalCultureFeed::getLoggedInUser();
      $uitpas_user->cf = DrupalCultureFeed::getLoggedInUserInstance();

      if ($uitpas_user->user) {
        $uitpas_user->passholder = $uitpas_user->cf->uitpas()->getPassholderByUser($uitpas_user->user->id);
      }
    }
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_uitpas_get_uitpas_user', $e);
  }

  return $uitpas_user;
}

/**
 * Returns highlighted promotions.
 */
function culturefeed_uitpas_get_promotions_highlight() {

  $promotions = array();
  try {

    $cf = DrupalCultureFeed::getConsumerInstance();
    $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
    $query->sort = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::SORT_CASHING_PERIOD_END;
    $query->max = variable_get('culturefeed_uitpas_promotions_highlight_max', 5);
    $query->cashingPeriodBegin = time();
    $query->unexpired = TRUE;
    $result = $cf->uitpas()->getPromotionPoints($query);
    if ($result->total) {
      $promotions = $result->objects;
    }

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_uitpas_block_promotions_highlight', $e);
  }

  return $promotions;

}

/**
 * Returns recent actions.
 */
function culturefeed_uitpas_get_recent_actions() {

  $actions = array();
  try {

    $cf = DrupalCultureFeed::getConsumerInstance();
    $query = new CultureFeed_SearchActivitiesQuery();
    $query->type = CultureFeed_Activity::TYPE_UITPAS;
    $query->private = FALSE;
    $query->max = variable_get('culturefeed_uitpas_recent_actions_max', 5);
    $result = $cf->searchActivities($query);
    if ($result->total) {
      $actions = $result->objects;
    }

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_uitpas_block_recent_actions', $e);
  }

  return $actions;

}
