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

  try {

    if (DrupalCultureFeed::isCultureFeedUser()) {

      $uitpas_user->user = DrupalCultureFeed::getLoggedInUser();
      $uitpas_user->cf = DrupalCultureFeed::getLoggedInUserInstance();
      try {
        $uitpas_user->passholder = $uitpas_user->cf->uitpas()->getPassholderByUser($uitpas_user->user->id);
      }
      catch (Exception $e) {
        $uitpas_user->passholder = FALSE;
      }

    }

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_uitpas_helper_uitpas_user', $e);
  }

  return $uitpas_user;

}
