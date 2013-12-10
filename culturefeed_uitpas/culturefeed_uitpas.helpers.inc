<?php

/**
 * @file
 * Helper functions for Culturefeed uitpas.
 */

/**
 * Returns a Culturefeed uitpas event object.
 *
 * @param CultuurNet\Search\ActivityStatsExtendedEntity $item
 *   A Cultuurnet event item.
 *
 * @return CultureFeed_Uitpas_Event_CultureEvent
 *   A Culturefeed event.
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

/**
 * Returns a Culturefeed uitpas event object.
 *
 * @return string
 *   An UiTPAS location.
 */
function culturefeed_uitpas_get_uitpas_location() {

  $location = '';
  $locations = variable_get('culturefeed_uitpas_locations');

  $uitpas_user = culturefeed_uitpas_get_uitpas_user();

  // Get the user's location if available.
  if ($uitpas_user->passholder && $uitpas_user->passholder->city) {
    $location = strtolower($uitpas_user->passholder->city);
  }

  // Cookie is only for anonymous users.
  elseif (isset($_COOKIE['uitpas_location'])) {
    $location = $_COOKIE['uitpas_location'];
  }

  // Set location through a dropdown.
  if (isset($_GET['uitpas_location'])) {
    $location = $_GET['uitpas_location'];
  }

  if (!isset($locations[$location])) {
    $location = 'all';
  }

  setcookie('uitpas_location', $location, 0, '/');

  return $location;

}

/**
 * Returns a culterfeed uitpas user object.
 */
function culturefeed_uitpas_get_uitpas_user() {

  $uitpas_user = new stdClass();
  $uitpas_user->user = NULL;
  $uitpas_user->passholder = NULL;
  $uitpas_user->card_system = NULL;

  try {

    if (DrupalCultureFeed::isCultureFeedUser()) {

      $uitpas_user->user = DrupalCultureFeed::getLoggedInUser();
      $uitpas_user->cf = DrupalCultureFeed::getLoggedInUserInstance();

      if ($uitpas_user->user) {

        $uitpas_user->passholder = $uitpas_user->cf->uitpas()->getPassholderByUser($uitpas_user->user->id);
        if ($uitpas_user->passholder) {

          $card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');
          if ($card_system_id && isset($uitpas_user->passholder->cardSystemSpecific[$card_system_id])) {
            $uitpas_user->card_system = $uitpas_user->passholder->cardSystemSpecific[$card_system_id];
          }

        }

      }

    }

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_uitpas_get_uitpas_user', $e);
  }

  return $uitpas_user;
}
