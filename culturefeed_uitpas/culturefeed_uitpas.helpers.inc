<?php

/**
 * @file
 * Helper functions for Culturefeed uitpas.
 */

/**
 * Get the name of the card system for display purposes.
 *
 * This is a legacy wrapper: cards in the HELA card system were called 'UiTPAS',
 * so we need to display 'UiTPAS' for them instead of 'HELA'.
 *
 * @param CultureFeed_Uitpas_CardSystem $cardsystem
 *   The card system
 *
 * @return string
 *   The name of the card system
 */
function culturefeed_uitpas_cardsystem_name(CultureFeed_Uitpas_CardSystem $cardsystem) {
  if ($cardsystem->name == 'HELA') {
    return 'UiTPAS';
  }

  return $cardsystem->name;
}

/**
 * Returns the card systems used in this site.
 *
 * @return \CultureFeed_Uitpas_CardSystem[]
 *   The card systems.
 */
function culturefeed_uitpas_get_card_systems() {

  $cf = DrupalCultureFeed::getConsumerInstance();
  $default_card_system = variable_get('culturefeed_uitpas_cardsystem_id');
  $active_card_systems = $cf->uitpas()->getCardSystems('permanent');
  $selected_card_systems = array_filter(array_values(variable_get('culturefeed_uitpas_cardsystem_ids', array())));
  $card_systems = array();

  foreach ($active_card_systems as $card_system) {
    if (count($selected_card_systems) && in_array($card_system->id, $selected_card_systems)) {
      $card_systems[] = $card_system;
    }
    // if no card system is selected, we use default card system
    else if ($card_system->id == $default_card_system) {    
      $card_systems[] = $card_system;
    }
  }

  return $card_systems;

}

/**
 * Check if site supports multiple cardsystems.
 *
 * @return bool
 *   Boolean.
 */
function culturefeed_uitpas_multiple_cardsystems() {

  $card_systems = culturefeed_uitpas_get_card_systems();

  if (count($card_systems) > 1) {
    return TRUE;
  }

}


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
    $cache_prefix = 'culturefeed:uitpas:cultureevent:';
    
    if ($cache = cache_get($cache_prefix . $cdbid, 'cache_culturefeed')) {
      $event = $cache->data;
    }
    else {
  
      $query = new CultureFeed_Uitpas_Event_Query_SearchEventsOptions();
      $query->cdbid = $cdbid;
  
      $cf = DrupalCultureFeed::getConsumerInstance();
      $result = $cf->uitpas()->searchEvents($query);
  
      if ($result->total && !empty($result->objects[0])) {
        $event = $result->objects[0];
      }
      cache_set($cache_prefix . $cdbid, $event, 'cache_culturefeed', REQUEST_TIME + CULTUREFEED_CACHE_EXPIRES);
    }

  }

  return $event;

}

/**
 * Get the card systems as options for the profile pages.
 *
 * @return array
 *   The options.
 */
function culturefeed_uitpas_get_user_card_systems_as_options($other_regions = TRUE) {

  // Get the card systems for the passholder.
  $options = array();
  $uitpas_user = culturefeed_uitpas_get_uitpas_user();
  /* @var \CultureFeed_Uitpas_Passholder $passholder */
  $passholder = $uitpas_user->passholder;
  foreach ($passholder->cardSystemSpecific as $card_system_specific) {
    $card_system = $card_system_specific->cardSystem;
    $options[$card_system->id] = $card_system->name;
  }

  $multiple_card_systems = culturefeed_uitpas_multiple_cardsystems();

  // If multiple cards systems are enabled, get the remaining card systems.
  if ($multiple_card_systems && $other_regions) {
    $card_systems = culturefeed_uitpas_get_card_systems();
    array_walk($card_systems, function (&$card_system) {
      /* @var CultureFeed_Uitpas_Cardsystem $card_system */
      $card_system = $card_system->id;
    });
    $card_systems = array_diff($card_systems, array_keys($options));
    $options['other-card-systems+' . implode('+', $card_systems)] = t('Other regions');
  }

  return $options;

}

/**
 * Returns a Culturefeed uitpas location.
 *
 * @return string
 *   An UiTPAS location.
 */
function culturefeed_uitpas_get_uitpas_location() {

  $location = '';
  $locations = variable_get('culturefeed_uitpas_locations', array());
  if (!$locations) {
    $locations = array();
  }

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

  if (!in_array($location, $locations)) {
    $location = 'all';
  }

  setcookie('uitpas_location', $location, 0, '/');

  return $location;

}

/**
 * Returns a culterfeed uitpas user object.
 */
function culturefeed_uitpas_get_uitpas_user() {
  $uitpas_user = &drupal_static(__FUNCTION__);

  if (!$uitpas_user) {

    $uitpas_user = new stdClass();
    $uitpas_user->user = NULL;
    $uitpas_user->passholder = NULL;
    $uitpas_user->card_system = NULL;

    try {

      if (DrupalCultureFeed::isCultureFeedUser()) {

        $uitpas_user->user = DrupalCultureFeed::getLoggedInUser();
        $uitpas_user->cf = DrupalCultureFeed::getLoggedInUserInstance();

        if ($uitpas_user->user) {
        
          $cache_prefix = 'culturefeed:uitpas:passholder:';
          
          if ($cache = cache_get($cache_prefix . $uitpas_user->user->id, 'cache_culturefeed')) {
            $uitpas_user = $cache->data;
          }
          else {
          
            try {
              $uitpas_user->passholder = $uitpas_user->cf->uitpas()->getPassholderByUser($uitpas_user->user->id);
              if ($uitpas_user->passholder) {
    
                $card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');
                if ($card_system_id && isset($uitpas_user->passholder->cardSystemSpecific[$card_system_id])) {
                  $uitpas_user->card_system = $uitpas_user->passholder->cardSystemSpecific[$card_system_id];
                }
                else {
                  $uitpas_user->card_system = reset($uitpas_user->passholder->cardSystemSpecific);
                }
              }
            }
            catch (Exception $e) {
              if ($e->error_code == 'UNKNOWN_PASSHOLDER_UID') {
                $uitpas_user->passholder = null;
              }
            }
            cache_set($cache_prefix . $uitpas_user->user->id, $uitpas_user, 'cache_culturefeed', REQUEST_TIME + CULTUREFEED_CACHE_EXPIRES);
          }

        }

      }

    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_uitpas_get_uitpas_user', $e);
    }
  }

  return $uitpas_user;
}

/**
 * Check if the current user is a UiTPAS passholder and registered it online
 *
 * @return bool
 *   Boolean.
 */
function culturefeed_uitpas_is_passholder() {
  $uitpas_user = culturefeed_uitpas_get_uitpas_user();

  return NULL !== $uitpas_user->passholder;
}

/**
 * Check if the current user has already bought a card, but hasn't registered it online
 *
 * @return bool
 *   Boolean.
 */

function culturefeed_uitpas_not_yet_registered() {
  if(!culturefeed_uitpas_is_passholder() && culturefeed_uitpas_email_is_passholder()) {
    return true;
  }
}

/**
 * Check if the email is a UiTPAS passholder
 *
 * @return bool
 *   Boolean.
 */

function culturefeed_uitpas_email_is_passholder() {
  try {
    $uitpas_user = culturefeed_uitpas_get_uitpas_user();

    if ($uitpas_user->user && $uitpas_user->user->mbox) {
    
      $cache_prefix = 'culturefeed:uitpas:passholders:';
    
      if ($cache = cache_get($cache_prefix . $uitpas_user->user->mbox, 'cache_culturefeed')) {
        $passholders = $cache->data;
      }
      else {
        $query = new CultureFeed_Uitpas_Passholder_Query_SearchPassholdersOptions();
        $query->email = $uitpas_user->user->mbox;
        $passholders = $uitpas_user->cf->uitpas()->searchPassholders($query);
        cache_set($cache_prefix . $uitpas_user->user->mbox, $passholders, 'cache_culturefeed', REQUEST_TIME + CULTUREFEED_CACHE_EXPIRES);
      }
      if ($passholders->total > 0) {
        return true;
      }
    }
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_uitpas_not_yet_registered', $e);
  }
}
