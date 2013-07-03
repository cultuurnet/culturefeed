<?php
/**
 * @file
 * Helper functions for the culturefeed agenda.
 **/

/**
 * Get the current active agenda object.
 * @param $path
 *   Path to load the active object of. Defaults to current path.
 */
function culturefeed_agenda_get_active_object($path = NULL) {

  $supported_types = array(
    'e' => 'culturefeed_agenda_event',
    'p' => 'culturefeed_agenda_production',
    'a' => 'culturefeed_agenda_actor',
  );

  if (arg(0, $path) == 'agenda' && isset($supported_types[arg(1, $path)])) {
    return menu_get_object($supported_types[arg(1, $path)], 3, $path);
  }

}

/**
 * Get all the links from a given item.
 * @var CultureFeed_Cdb_Item_Base $item
 */
function _culturefeed_agenda_get_links_from_item(CultureFeed_Cdb_Item_Base $item) {

  $detail = $item->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language());

  $links = array();
  // General links.
  if ($detail) {
    $media_links = $detail->getMedia()->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_WEBRESOURCE);
    foreach ($media_links as $media_link) {
      $links[] = $media_link->getHLink();
    }
  }

  // Contact info links.
  if (method_exists($item, 'getContactInfo') && $item->getContactInfo()) {
    $contact_links = _culturefeed_agenda_get_contact_links($item->getContactInfo());
    $links += $contact_links['main'];
    $links += $contact_links['other'];
  }

  // Location links.
  if (method_exists($item, 'getLocation') && $item->getLocation() && $item->getLocation()->getActor() && $item->getLocation()->getActor()->getContactInfo()) {
    $contact_links += _culturefeed_agenda_get_contact_links($item->getLocation()->getActor()->getContactInfo());
    $links += $contact_links['main'];
    $links += $contact_links['other'];
  }

  // Organiser links.
  if (method_exists($item, 'getOrganiser') && $item->getOrganiser() && $item->getOrganiser()->getActor() && $item->getOrganiser()->getActor()->getContactInfo()) {
    $contact_links = _culturefeed_agenda_get_contact_links($item->getOrganiser()->getActor()->getContactInfo());
    $links += $contact_links['main'];
    $links += $contact_links['other'];
  }

  // Performer links.
  if ($detail && method_exists($detail, 'getPerformers') && $detail->getPerformers()) {
    $performers = $detail->getPerformers();
    foreach ($performers as $performer) {
      if ($performer->getActor() && $performer->getActor()->getContactInfo()) {
        $contact_links = _culturefeed_agenda_get_contact_links($performer->getActor()->getContactInfo());
        $links += $contact_links['main'];
        $links += $contact_links['other'];
      }
    }
  }

  // Cleaning up links.
  foreach ($links as $i => $link) {
    $links[$i] = rtrim($link, '/');
  }

  $links = array_unique($links);

  return $links;

}

/**
 * Get the list of links from a given contact info. Seperate it in main and other.
 */
function _culturefeed_agenda_get_contact_links(CultureFeed_Cdb_Data_ContactInfo $info) {

  $links = array('main' => array(), 'other' => array());
  foreach ($info->getUrls() as $url) {
    if ($url->isMain()) {
      $links['main'][] = $url->getUrl();
    }
    else {
      $links['other'][] = $url->getUrl();
    }
  }

  return $links;

}

/**
 * Get the first culturefeed page link connected with the given item.
 */
function culturefeed_agenda_get_connected_page_id_of_item(CultureFeed_Cdb_Item_Base $item) {

  $detail = $item->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language());

  // Reset organiser link. For CEP we need to check on pages for it.
  if ($detail && $detail->getMedia()) {

    $page_links = $detail->getMedia()->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_CULTUREFEED_PAGE);
    foreach ($page_links as $link) {

      if ($link->getRelationType() == CultureFeed_Cdb_Data_File::REL_TYPE_ORGANISER) {
        $link_parts = explode('/', $page_links->current()->getHLink());
        return $link_parts[count($link_parts) - 1];
      }
    }

  }

}

/**
 * Calculate the different row classes for a program by weekscheme.
 * @param CultureFeed_Cdb_Data_Calendar_Weekscheme $weekscheme
 * @param array $all_classes
 */
function culturefeed_agenda_calculate_program_classes_by_weekscheme(CultureFeed_Cdb_Data_Calendar_Weekscheme $weekscheme, $all_classes) {

  $classes = array();

  $current_day = date('l');
  $opening_info = $weekscheme->getDay($current_day);
  if ($opening_info->isOpen()) {

  }

}

/**
 * Get all the known locations for this item.
 * @param CultureFeed_Cdb_Item_Base $item
 */
function culturefeed_agenda_get_locations_of_item(CultureFeed_Cdb_Item_Base $item) {

  $locations = array();

  if ($item instanceof CultureFeed_Cdb_Item_Event) {
    $locations[] = culturefeed_agenda_get_location_of_event($item);
  }
  elseif ($item instanceof CultureFeed_Cdb_Item_Production) {
    $relations = $item->getRelations();
    // Production has a reference to multiple events.
    if ($relations) {
      foreach ($relations as $relation) {
        $event = culturefeed_agenda_event_load($relation->getCdbid());
        $locations[] = culturefeed_agenda_get_location_of_event($relation->getEntity());
      }
    }
  }
  elseif ($item instanceof CultureFeed_Cdb_Item_Actor) {

    if ($item->getContactInfo()) {

      $contact_info = $item->getContactInfo();
      $addresses = $contact_info->getAddresses();

      if ($addresses) {

        $location = array();
        $location['title'] = $addresses[0]->getLabel();
        $physical_address = $addresses[0]->getPhysicalAddress();
        if ($physical_address) {
          $location['city'] = check_plain($physical_address->getCity());
          $location['street'] = check_plain($physical_address->getStreet() . ' ' . $physical_address->getHouseNumber());
          $location['zip'] = check_plain($physical_address->getZip());
        }
        $locations[] = $location;

      }

    }

  }

  return $locations;

}

/**
 * Get the location of an event.
 * @param CultureFeed_Cdb_Item_Event $event
 */
function culturefeed_agenda_get_location_of_event(CultureFeed_Cdb_Item_Event $event) {

  $location = array();
  if ($event->getLocation()) {

    // Always send a title key.
    $location['title'] = '';

    $location_info = $event->getLocation();

    if ($location_info->getLabel()) {
      $location['title'] = $location_info->getLabel();
    }
    if ($location_info->getActor()) {
      $actor_detail = $location_info->getActor()->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language());
      if ($actor_detail) {
        $location['title'] = check_plain($actor_detail->getTitle());
        $location['link'] = culturefeed_search_detail_l('actor', $location_info->getActor()->getCdbid(), $location['title']);
      }
    }

    $physical_address = $location_info->getAddress()->getPhysicalAddress();
    if ($physical_address) {
      $location['city'] = $physical_address->getCity();
      $location['street'] = $physical_address->getStreet() . ' ' . $physical_address->getHouseNumber();
      $location['zip'] = $physical_address->getZip();
    }

  }

  return $location;

}

/**
 * Get the data for an item in the production program list.
 */
function culturefeed_agenda_get_production_program_data_for_item(Cultuurnet\Search\ActivityStatsExtendedEntity $item) {

  $cdb_item = $item->getEntity();
  $detail = $cdb_item->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language());

  // Only place items in list that have a detail for current language.
  if ($detail) {

    $location = culturefeed_agenda_get_location_of_event($cdb_item);

    // Construct base data.
    $data = array(
      'title' => $detail->getTitle(),
      'id' => $item->getId(),
      'url' => culturefeed_search_detail_url($item->getType(), $item->getId(), $detail->getTitle()),
      'calendar' => $detail->getCalendarSummary(),
      'city' => isset($location['city']) ? $location['city'] : '',
    );

    // Extend with an url to the actor.
    // This can be an organiser, or a location.
    $organiser = $cdb_item->getOrganiser();
    if ($organiser) {

      // Check if we can construct an actor url by label or actor reference..
      if ($organiser->getActor() && $organiser->getActor()->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language())) {
        $actor = $organiser->getActor();
        $organiser_actor_details = $organiser->getActor()->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language());
        $data['all_url'] = culturefeed_search_detail_url('production', $actor->getCdbid(), $actor->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language())->getTitle());
      }
      elseif ($organiser->getLabel() && $organiser->getCdbid()) {
        $data['all_url'] = culturefeed_search_detail_url('production', $organiser->getCdbid(), $organiser->getLabel());
      }

    }

    // If we did not find an organiser. Check if we can link to the location.
    if (!isset($data['all_url']) && isset($location['link'])) {
      $location_actor = $cdb_item->getLocation()->getActor();
      $data['all_url'] = culturefeed_search_detail_url('actor', $location_actor->getCdbid(), $location['title']);
    }

  }

  return $data;

}

/**
 * Set the breadcrumb for an event / production.
 * @param \CultuurNet\Search\ActivityStatsExtendedEntity $item
 */
function culturefeed_agenda_set_activity_breadcrumb(\CultuurNet\Search\ActivityStatsExtendedEntity $item) {

  $active_trail = array();

  $active_trail[] = array(
    'title' => t('Home'),
    'href' => '<front>',
    'link_path' => '',
    'localized_options' => array(),
    'type' => 0,
  );

  // Show the event type in breadcrumb.
  $event_types = $item->getEntity()->getCategories()->getCategoriesByType(CultureFeed_Cdb_Data_Category::CATEGORY_TYPE_EVENT_TYPE);
  if ($event_types) {

    // First event type.
    $active_trail[] = array(
      'title' => $event_types[0]->getName(),
      'href' => 'agenda/search',
      'link_path' => '',
      'localized_options' => array(
        'query' => array(
          'facet' => array('category_eventtype_id' => array($event_types[0]->getId())),
        ),
      ),
      'type' => 0,
    );

  }

  // City
  $locations = culturefeed_agenda_get_locations_of_item($item->getEntity());
  if (!empty($locations) && isset($locations[0]['city'])) {
    $active_trail[] = array(
      'title' => $locations[0]['city'],
      'href' => 'agenda/search',
      'link_path' => '',
      'localized_options' => array(
        'query' => array(
          'location' => $locations[0]['zip'] . ' ' . $locations[0]['city'],
        ),
      ),
      'type' => 0,
    );
  }


  $active_trail[] = array(
    'title' => $item->getTitle(culturefeed_search_get_preferred_language()),
    'href' => $_GET['q'],
    'link_path' => '',
    'localized_options' => array(),
    'type' => 0,
  );

  menu_set_active_trail($active_trail);

}

/**
 * Set the breadcrumb for an actor.
 * @param \CultuurNet\Search\ActivityStatsExtendedEntity $item
 */
function culturefeed_agenda_set_actor_breadcrumb(\CultuurNet\Search\ActivityStatsExtendedEntity $item) {

  $active_trail = array();

  $active_trail[] = array(
    'title' => t('Home'),
    'href' => '<front>',
    'link_path' => '',
    'localized_options' => array(),
    'type' => 0,
  );

  // Show the actor type in breadcrumb.
  $actor_types = $item->getEntity()->getCategories()->getCategoriesByType(CultureFeed_Cdb_Data_Category::CATEGORY_TYPE_ACTOR_TYPE);
  if ($actor_types) {

    // First event type.
    $active_trail[] = array(
      'title' => $actor_types[0]->getName(),
      'href' => 'agenda/search',
      'link_path' => '',
      'localized_options' => array(
        'query' => array(
          'facet' => array('category_actortype_id' => array($actor_types[0]->getId())),
        ),
      ),
      'type' => 0,
    );

  }

  // City
  $locations = culturefeed_agenda_get_locations_of_item($item->getEntity());
  if (!empty($locations) && isset($locations[0]['city'])) {
    $active_trail[] = array(
      'title' => $locations[0]['city'],
      'href' => 'agenda/search',
      'link_path' => '',
      'localized_options' => array(
        'query' => array(
          'location' => $locations[0]['zip'] . ' ' . $locations[0]['city'],
        ),
      ),
      'type' => 0,
    );
  }

  $active_trail[] = array(
    'title' => $item->getTitle(culturefeed_search_get_preferred_language()),
    'href' => $_GET['q'],
    'link_path' => '',
    'localized_options' => array(),
    'type' => 0,
  );

  menu_set_active_trail($active_trail);

}