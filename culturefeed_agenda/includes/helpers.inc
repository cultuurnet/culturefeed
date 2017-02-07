<?php
/**
 * @file
 * Helper functions for the culturefeed agenda.
 **/

 /**
  * Return a phone number without spaces, dashes, slashes, characters
  */
 function culturefeed_valid_phone($raw_phone) {

   $phone = str_replace(' ', '-', $raw_phone);
   $phone = preg_replace('/[^0-9\+\(\)]/', '', $phone);

   return $phone;

}

/**
 * Return all the known agenda entity types.
 */
function culturefeed_agenda_known_entity_types() {

  return array(
    'event' => t('event', array(), array('context' => 'culturefeed-facets')),
    'production' => t('production', array(), array('context' => 'culturefeed-facets')),
    'actor' => t('actor', array(), array('context' => 'culturefeed-facets')),
  );

}

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

    foreach ($detail->getMedia() as $media) {
      if ($media->getMediaType() == CultureFeed_Cdb_Data_File::MEDIA_TYPE_WEBRESOURCE || $media->getMediaType() == CultureFeed_Cdb_Data_File::MEDIA_TYPE_WEBSITE) {
        $links[] = $media->getHLink();
      }
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
    $events = culturefeed_agenda_load_events_of_production($item, FALSE);
    if (!empty($events)) {
      foreach ($events as $event) {
        $locations[] = culturefeed_agenda_get_location_of_event($event->getEntity());
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
    if ($event->getCdbid()) {
      $location['cdbid_event'] = $event->getCdbid();
    }
    if ($event->getLocation()->getCdbid()) {
      $location['link'] = culturefeed_search_detail_l('actor', $event->getLocation()->getCdbid(), $location['title']);
    }
    if ($location_info->getActor()) {
      $actor_detail = $location_info->getActor()->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language());
      if ($actor_detail) {
        $location['title'] = check_plain($actor_detail->getTitle());
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
 * @param Cultuurnet\Search\ActivityStatsExtendedEntity $item
 *   Extended entity of a CultureFeed_Cdb_Item_Event.
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
      'when' => $detail->getCalendarSummary(),
      'city' => isset($location['city']) ? $location['city'] : '',
      'venue' => isset($location['title']) ? $location['title'] : '',
    );

    if (module_exists('culturefeed_calendar')) {
      $data['personal_calendar_buttons'] = culturefeed_calendar_render_calendar_buttons($item);
    }

    // Extend with an url to the actor.
    // This can be an organiser, or a location.
    $organiser = $cdb_item->getOrganiser();
    if ($organiser) {

      // Check if we can construct an actor url by label or actor reference..
      if ($organiser->getActor() && $organiser->getActor()->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language())) {
        $actor = $organiser->getActor();
        $organiser_actor_details = $organiser->getActor()->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language());
        $data['all_url'] = culturefeed_search_detail_url('actor', $actor->getCdbid(), $actor->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language())->getTitle());
      }
      elseif ($organiser->getLabel() && $organiser->getCdbid()) {
        $data['all_url'] = culturefeed_search_detail_url('actor', $organiser->getCdbid(), $organiser->getLabel());
      }

    }

    // If we did not find an organiser. Check if we can link to the location.
    if (!isset($data['all_url']) && isset($location['link'])) {
      $data['all_url'] = culturefeed_search_detail_url('actor', $cdb_item->getLocation()->getCdbid(), $location['title']);
    }

  }

  $data['calendar'] = culturefeed_agenda_parse_calendar($cdb_item->getCalendar());

  return $data;
}

/**
 * Set the breadcrumb for an event / production.
 * @param \CultuurNet\Search\ActivityStatsExtendedEntity $item
 */
function culturefeed_agenda_set_activity_breadcrumb(\CultuurNet\Search\ActivityStatsExtendedEntity $item) {

  $active_trail = array();
  $facet = array();

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

    $facet['category_eventtype_id'] = array($event_types[0]->getId());

    // First event type.
    $active_trail[] = array(
      'title' => $event_types[0]->getName(),
      'href' => 'agenda/search',
      'link_path' => '',
      'localized_options' => array(
        'query' => array(
          'facet' => $facet,
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
          'facet' => $facet,
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

/**
 * Get the render array to show the google map for road description.
 */
function culturefeed_agenda_get_map_render_array(\CultuurNet\Search\ActivityStatsExtendedEntity $item) {

  global $language;
  module_load_include('inc', 'culturefeed_agenda', 'theme/theme');
  $variables = array(
    'item' => $item,
    'title' => $item->getTitle($language->language),
    'link' => culturefeed_search_detail_l($item->getType(), $item->getId(), $item->getTitle($language->language)),
  );

  $variables['track_route'] = '';
  if (culturefeed_is_culturefeed_user()) {
    $content_type = culturefeed_get_content_type($item->getType());
    $id = $item->getId();
    $variables['track_route'] = url('culturefeed/do/' . CultureFeed_Activity::TYPE_ROUTE .'/' . $content_type . '/' . urlencode($id) . '/ajax');
  }

  switch ($item->getType()) {

    case 'event':
      _culturefeed_agenda_preprocess_event($variables);
      break;

    case 'actor':
      _culturefeed_agenda_preprocess_actor($variables);
      break;
  }

  // Fetch the user info
  $cf_account = NULL;
  try {
    $cf_account = DrupalCultureFeed::getLoggedInUser();
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_agenda', $e);
  }

  if ($cf_account) {
    $variables['to_address'] = $cf_account->street . ', ' . $cf_account->zip . ' ' . $cf_account->city;
  }

  drupal_set_breadcrumb(array(l(t('Home'), NULL), $variables['link'], 'Stratenplan'));

  unset($variables['item']);

  $attached_js = array(
    array('data' => culturefeed_agenda_google_maps_js_api_url(), 'type' => 'external'),
    drupal_get_path('module', 'culturefeed_agenda') . '/js/map.js',
  );

  $attached_js[] = array(
    'type' => 'setting',
    'data' => array('culturefeed_map' => array('info' => $variables)),
  );

  $attached_js[] = array(
    'type' => 'inline',
    'data' => 'jQuery(document).ready(Drupal.CultureFeed.Agenda.initializeMap);',
  );

  return array(
    '#theme' => 'culturefeed_agenda_travel_map',
    '#attached' => array(
      'js' => $attached_js,
    ),
  );
}

/**
 * Load all related events for a production.
 * @param $production
 *   The production to load related events for.
 * @param $future_only
 *   FALSE to search also on past events.
 */
function culturefeed_agenda_load_events_of_production($production, $future_only = TRUE) {

  $relations = $production->getRelations();
  if (empty($relations)) {
    return;
  }

  // Currently we do an extra query to load the event details.
  // Sven checks if this could be added to the event loading.
  $cdb_ids = array();
  foreach ($relations as $relation) {
    $cdb_ids[] = 'cdbid:' . $relation->getCdbid();
  }

  $parameters = array();
  $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery('type:event');
  $parameters[] = new \CultuurNet\Search\Parameter\Query(implode(' OR ', $cdb_ids));
  $parameters[] = new \CultuurNet\Search\Parameter\Group();
  $parameters[] = new \CultuurNet\Search\Parameter\Sort('startdate', 'asc');

  if (!$future_only) {
    $parameters[] = new \CultuurNet\Search\Parameter\Parameter('past', 'true');
  }

  try {
    $result = culturefeed_get_search_service()->search($parameters);
    if ($result->getTotalCount() == 0) {
      return;
    }
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_agenda', $e);
    return;
  }

  return $result->getItems();
}


/**
 * Parse weekscheme from object to array
 */
function culturefeed_agenda_parse_weekscheme(CultureFeed_Cdb_Data_Calendar_Weekscheme $weekscheme) {

  $weekscheme_array = array();

  foreach ($weekscheme->getDays() as $day) {
    $weekscheme_array[$day->getDayName()] = array(
      'type' => $day->getOpenType(),
    );
    if ($day->getOpenType() == 'open') {
      $i = 0;
      foreach($day->getOpeningTimes() as $openingtime) {
        $weekscheme_array[$day->getDayName()]['openingtimes'][$i]['from'] = $openingtime->getOpenFrom();
        if ($openingtime->getOpenTill()) {
          $weekscheme_array[$day->getDayName()]['openingtimes'][$i]['till'] = $openingtime->getOpenTill();
        }
        $i++;
      }
    }
  }
  return $weekscheme_array;
}

/**
 * Parse timestamps from object (CultureFeed_Cdb_Data_Calendar_TimestampList or CultureFeed_Cdb_Data_Calendar_Exceptions) to array
 */
function culturefeed_agenda_parse_timestamps($timestamps) {

  $timestamps_array = array();

  $timestamps_count = iterator_count($timestamps);
  $timestamps->rewind();
  $current_timestamp = 0;
  for ($i = 0; $i < $timestamps_count; $i++) {

    $timestamp = $timestamps->current();
    $date = $timestamp->getDate();
    $checkdate = new DateTime($date);
    $now = new DateTime(date('Y-m-d', time()));

    if ($checkdate >= $now) {
      $timestamps_array[$current_timestamp] = array(
        'day' => date('D', strtotime($date)),
        'date' => date('d/m', strtotime($date)),
				'year' => date('Y', strtotime($date)),
				'countable_date' => date('Y-m-d', strtotime($date)),
      );
      if ($timestamp->getStartTime()) {
        $timestamps_array[$current_timestamp]['begintime'] = date('H:i', strtotime($timestamp->getStartTime()));
      }
      if ($timestamp->getEndTime()) {
        $timestamps_array[$current_timestamp]['endtime'] = date('H:i', strtotime($timestamp->getEndTime()));
      }
      if ($timestamp->getOpenType()) {
        $timestamps_array[$current_timestamp]['type'] = $timestamp->getOpenType();
      }
      $current_timestamp++;
    }
    $timestamps->next();

  }
  return $timestamps_array;
}

function culturefeed_agenda_group_timestamps(&$timestamps) {

  if (count($timestamps) > 1) {
    for ($i = 0; $i < count($timestamps); $i++) {
      if (isset($timestamps[$i + 1])) {
        $this_timestamp = $timestamps[$i];
        $next_timestamp = $timestamps[$i + 1];

        if ($this_timestamp['date'] == $next_timestamp['date']) {
          if (isset($timestamps[$i]['begintime']) && is_array($timestamps[$i]['begintime'])) {
            //unset($timestamps[$i]['begintime']);
            $timestamps[$i]['begintime'][] = $next_timestamp['begintime'];
            /*if (isset($this_timestamp['begintime'])) {
              $timestamps[$i]['begintime'][0] = $this_timestamp['begintime'];
            }
            if (isset($next_timestamp['begintime'])) {
              $timestamps[$i]['begintime'][1] = $next_timestamp['begintime'];
            }*/
          }
          else {
            if (isset($next_timestamp['begintime']) && isset($this_timestamp['begintime']) && $this_timestamp['begintime'] != $next_timestamp['begintime']) {
              unset($timestamps[$i]['begintime']);
              if (isset($this_timestamp['begintime'])) {
                $timestamps[$i]['begintime'][] = $this_timestamp['begintime'];
              }
              if (isset($next_timestamp['begintime'])) {
                $timestamps[$i]['begintime'][] = $next_timestamp['begintime'];
              }
            }
          }
          if (isset($timestamps[$i]['begintime']) && is_array($timestamps[$i]['begintime'])) {
            sort($timestamps[$i]['begintime']);
          }
          unset($timestamps[$i + 1]);
          $timestamps = array_values($timestamps);
          $i--;
        }
      }
    }
  }
  return $timestamps;
}

function culturefeed_agenda_parse_calendar($calendar) {

  $calendar_class = get_class($calendar);
  $calendar->rewind();

  if ($calendar_class == 'CultureFeed_Cdb_Data_Calendar_PeriodList') {

    $calendar_array = array(
      'type' => 'period',
      'period_start' => $calendar->current()->getDateFrom(),
      'period_end' => $calendar->current()->getDateTo(),
    );

    if ($calendar->current()->getWeekScheme()) {
      $calendar_array['weekscheme'] = culturefeed_agenda_parse_weekscheme($calendar->current()->getWeekScheme());
    }
    if ($calendar->current()->getExceptions()) {
      $calendar_array['exceptions'] = culturefeed_agenda_parse_timestamps($calendar->current()->getExceptions());
    }

  }
  elseif ($calendar_class == 'CultureFeed_Cdb_Data_Calendar_Permanent') {
    $calendar_array = array(
      'type' => 'permanent',
    );

    if ($calendar->getWeekScheme()) {
      $calendar_array['weekscheme'] = culturefeed_agenda_parse_weekscheme($calendar->getWeekScheme());
    }
    if ($calendar->getExceptions()) {
      $calendar_array['exceptions'] = culturefeed_agenda_parse_timestamps($calendar->getExceptions());
    }

  }
  elseif ($calendar_class == 'CultureFeed_Cdb_Data_Calendar_TimestampList') {
    $calendar_array = array(
      'type' => 'timestamps',
    );
    $calendar_array['timestamps'] = culturefeed_agenda_parse_timestamps($calendar);
    $calendar_array['timestamps'] = culturefeed_agenda_group_timestamps($calendar_array['timestamps']);
  }
  return $calendar_array;
}

/**
 * Get the themed list of page suggestions.
 */
function culturefeed_agenda_get_nearby_activities_list() {

  $location_cookie = array();
  $user_location = culturefeed_search_ui_get_user_location_cookie();
  if (empty($user_location->postal) && empty($user_location->city)) {
    return '';
  }

  // Compose the search query.
  $parameters = array();
  $parameters[] = new \CultuurNet\Search\Parameter\Query('*:*');
  $parameters[] = new \CultuurNet\Search\Parameter\Rows(variable_get('agenda_nearby_activities_amount', 8));
  $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery('type:event OR type:production');
  $parameters[] = new \CultuurNet\Search\Parameter\Group();

  // Optional query params from block config.
  if ($extra_query = variable_get('agenda_nearby_activities_extra_query', '')) {
    $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery($extra_query);
  }

  // Add location filter from cookie.
  $parameters[] = new CultuurNet\Search\Parameter\Spatial\Zipcode($user_location->postal, variable_get('agenda_nearby_activities_distance', 25));

  $sort_options_string = variable_get('agenda_nearby_activities_sort', 'permanent asc, startdateday asc, weight desc');
  if (!empty($sort_options_string)) {
    $sort_options = explode(',', $sort_options_string);
    foreach ($sort_options as $sort_option) {
      $sort_option_values = explode(' ', $sort_option);
      $parameters[] = new \CultuurNet\Search\Parameter\Sort($sort_option_values[0], $sort_option_values[1]);
    }
  }

  try {
    $search = culturefeed_get_search_service();
    $result = $search->search($parameters);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_agenda', $e);
    return;
  }

  if (isset($result)) {
    return theme('culturefeed_agenda_nearby_activities_list', array('activities' => $result->getItems()));
  }

  $actors = array(
    '#theme' => 'culturefeed_agenda_actors_nearby',
    '#items' => $items,
  );

  return $actors;
}

/**
 * Get the category list for a given item, for a given category type.
 * @return array
 *   Array that contains list of plain text names (key = text) and a list of links for the category (key = links).
 */
function culturefeed_agenda_get_category_list_of_item($category_type, $cdb_item) {

  $text_list = array();
  $links_list = array();
  $id_list = array();

  $categories = $cdb_item->getCategories()->getCategoriesByType($category_type);
  foreach ($categories as $category) {

    // Don't add 'Thema onbepaald' and 'Type onbepaald'
    if ($category->getId() != '1.58.0.0.0' && $category->getId() != '0.51.0.0.0') {
      $options = array(
        'query' => array(
          'facet' => array(
            'category_' . $category_type . '_id' => array($category->getId()),
          ),
        ),
      );

      $text_list[] = check_plain($category->getName());
      $links_list[] = l($category->getName(), 'agenda/search', $options);
      $id_list[] = check_plain($category->getId());
    }
  }

  return array(
    'text' => $text_list,
    'links' => $links_list,
    'id' => $id_list,
  );

}

/**
 * A function to initiate a HTML calendar formatter.
 * @return \CultuurNet\CalendarSummary\CalendarHTMLFormatter
 */
function culturefeed_agenda_get_html_calendar_formatter() {
  $formatter = new CultuurNet\CalendarSummary\CalendarHTMLFormatter();

  return $formatter;
}

/**
 * A function to initiate a plain text calendar formatter.
 * @return \CultuurNet\CalendarSummary\CalendarPlainTextFormatter
 */
function culturefeed_agenda_get_plain_text_calendar_formatter() {
  $formatter = new CultuurNet\CalendarSummary\CalendarPlainTextFormatter();

  return $formatter;
}

/**
 * Returns TRUE if social links preprocessing is enabled.
 */
function culturefeed_agenda_social_links_preprocessing_enabled() {
  return variable_get('culturefeed_agenda_enable_activity_links_preprocessing', TRUE) && module_exists('culturefeed_social');
}

/**
 * Returns the google maps api url with the necessary query parameters.
 *
 * @return string
 *   The google maps API url.
 */
function culturefeed_agenda_google_maps_js_api_url() {
  global $language;
  $query = array(
    'v' => 3,
    'region' => 'BE',
    'hl' => $language->language,
    'ln' => $language->language,
    'key' => variable_get('culturefeed_agenda_google_maps_api_key'),
  );
  return url(CULTUREFEED_GOOGLE_MAPS_JS_API, array('query' => $query, 'absolute' => TRUE));
}

/**
 * OMD GMAP add javascript files
 * @param array $settings
 */
function culturefeed_agenda_gmap_add_js($settings = array()) {
  static $added = FALSE;

  if (!$added) {
    $added = TRUE;
    // Include JS files
    drupal_add_js(culturefeed_agenda_google_maps_js_api_url(), 'external');
    drupal_add_js(drupal_get_path('module', 'culturefeed_agenda') . '/js/markercluster.js', 'file');
    drupal_add_js(drupal_get_path('module', 'culturefeed_agenda') . '/js/agenda_map.js', 'file');

    // Set JS setting
    $on_search = $_GET['q'] == 'agenda/search';
    $search_params = drupal_get_query_parameters(NULL, array('page', 'q'));
    drupal_add_js(
      array(
        'culturefeed_agenda_map' => array(
          'cluster' => url(drupal_get_path('module', 'culturefeed_agenda') . '/images/cluster.png'),
          'onSearch' => $on_search,
          'doGeolocate' => $on_search && empty($search_params),
        ),
      ),
      'setting'
    );

  }
}

/**
 * Get the locations for current search by using a new list search request.
 */
function culturefeed_agenda_get_locations_by_list_search() {

  $searchPage = culturefeed_search_get_active_search_page();
  $parameters = $searchPage->getParameters();

  foreach ($parameters as $key => $parameter) {
    if ($parameter->getKey() == 'rows' || $parameter->getKey() == 'start') {
      unset($parameters[$key]);
    }
  }
  $parameters[] = new CultuurNet\Search\Parameter\Rows(variable_get('culturefeed_agenda_max_items_per_map', 700));

  try {
    $service = culturefeed_get_search_service();
    $results = $service->search($parameters);
  }
  catch (Exception $e) {
    watchdog_exception('Culturefeed Agenda', $e);
    $results = array();
  }

  // Loop over the search result items
  $locations = array();

  foreach ($results->getItems() as $item) {
    $event = $item->getEntity();
    $event_detail = $event->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language());

    $cdbid = $item->getId();
    $title = $event_detail->getTitle();

    $address = $event->getLocation()->getAddress()->getPhysicalAddress();

    if ($address == NULL) {
      continue;
    }

    $longitude = $address->getGeoInformation()->getXCoordinate();
    $latitude = $address->getGeoInformation()->getYCoordinate();

    if ($latitude == '' || $longitude == '') {
      continue;
    }

    $key = $longitude . '-' . $latitude;
    $locations[$key]['latitude'] = $latitude;
    $locations[$key]['longitude'] = $longitude;

    // Title
    $locations[$key]['title'] = $title;

    // HTML
    $locations[$key]['events'][] = culturefeed_search_detail_l('event', $cdbid, $title);

  }
  return $locations;

}

/**
 * Get the locations for current search by using current search result.
 */
function culturefeed_agenda_get_locations_by_search_result() {

  $culturefeedSearchResult = culturefeed_get_search_result();
  if (!$culturefeedSearchResult instanceof \Cultuurnet\Search\SearchResult) {
    return;
  }

  global $language;
  $locations = array();

  foreach ($culturefeedSearchResult->getItems() as $item) {
    $event = $item->getEntity();
    $event_detail = $event->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language());

    // get the location info
    if ($event->getLocation()) {
      $location = $event->getLocation();
      $address = $location->getAddress();
      if ($address && $address->getPhysicalAddress()) {
        $coordinates = $address->getPhysicalAddress()->getGeoInformation();
        if ($coordinates) {

          $key = $coordinates->getYCoordinate() . '-' . $coordinates->getXCoordinate();

          $locations[$key]['latitude'] = $coordinates->getYCoordinate();
          $locations[$key]['longitude'] = $coordinates->getXCoordinate();

          // Title
          $locations[$key]['title'] = $location->getLabel();

          // HTML
          if ($event_detail) {
            $url = culturefeed_search_detail_url($item->getType(), $event->getCdbid(), $event_detail->getTitle());
            $locations[$key]['events'][] = l($item->getTitle($language->language), $url);
          }

        }
      }

    }
  }

  return $locations;

}

/**
 * Returns a protocol relative URL
 */
function culturefeed_get_relative_image($image_path) {
  if(substr($image_path,0,7) === "http://") {
    return '//' . substr($image_path, 7);
  }
  elseif(substr($image_path,0,8) === "https://") {
    return '//' . substr($image_path, 8);
  }
  return $image_path;
}
