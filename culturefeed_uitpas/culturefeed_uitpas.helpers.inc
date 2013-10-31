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
