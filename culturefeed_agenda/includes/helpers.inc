<?php
/**
 * @file
 * Helper functions for the culturefeed agenda.
 **/

/**
 * Get the current active agenda object.
 */
function culturefeed_agenda_get_active_object() {

  $supported_types = array(
    'e' => 'culturefeed_agenda_event',
    'p' => 'culturefeed_agenda_production',
  );

  if (arg(0) == 'agenda' && isset($supported_types[arg(1)])) {
    return menu_get_object($supported_types[arg(1)], 3);
  }

}

/**
 * Get all the links from a given item.
 * @var CultureFeed_Cdb_Item_Base $item
 */
function _culturefeed_agenda_get_links_from_item(CultureFeed_Cdb_Item_Base $item) {

  global $language;
  $detail = $item->getDetails()->getDetailByLanguage($language->language);

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

  global $language;
  $detail = $item->getDetails()->getDetailByLanguage($language->language);

  // Reset organiser link. For CEP we need to check on pages for it.
  if ($detail->getMedia()) {

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

  return $locations;

}

/**
 * Get the location of an event.
 * @param CultureFeed_Cdb_Item_Event $event
 */
function culturefeed_agenda_get_location_of_event(CultureFeed_Cdb_Item_Event $event) {

  $location = array();
  if ($event->getLocation()) {

    global $language;
    $location_info = $event->getLocation();

    if ($location_info->getActor()) {
      $actor_detail = $location_info->getActor()->getDetails()->getDetailByLanguage($language->language);
      $location['title'] = check_plain($actor_detail->getTitle());
      $location['link'] = culturefeed_search_detail_l('actor', $location_info->getActor()->getCdbid(), $location['title']);
    }
    elseif ($location_info->getLabel()) {
      $location['title'] = $location_info->getLabel();
    }

    $physical_address = $location_info->getAddress()->getPhysicalAddress();
    if ($physical_address) {
      $location['city'] = $physical_address->getCity();
      $location['street'] = $physical_address->getStreet() . ' '  . $physical_address->getHouseNumber();
      $location['zip'] = $physical_address->getZip();
    }

  }

  return $location;

}