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
 * Returns a Culturefeed uitpas event object.
 *
 * @param CultuurNet\Search\ActivityStatsExtendedEntity $item
 *   A Cultuurent event item.
 */
function culturefeed_uitpas_get_uitpas_event($item) {

  $event = NULL;

  if ($item) {

    $cdb_item = $item->getEntity();
    $cdbid = $cdb_item->getCdbid();

    $query = new CultureFeed_Uitpas_Event_Query_SearchEventsOptions();
    $query->cdbid = $cdbid;

    $cf = DrupalCultureFeed::getConsumerInstance();
    $result = $cf->uitpas()->searchEvents($query);

    if ($result->total) {
      $event = $result->objects[0];
    }

  }

  return $event;

}
