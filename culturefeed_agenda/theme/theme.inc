<?php
/**
 * @file
 * Theme / preprocess functions for culturefeed agenda.
 */

/**
 * Preprocess the variables for the event summary.
 * @see culturefeed-event-summary.tpl.php
 */
function culturefeed_agenda_preprocess_culturefeed_event_summary(&$variables) {

  _culturefeed_agenda_preprocess_agenda($variables);
  _culturefeed_agenda_preprocess_agenda_summary($variables);
  _culturefeed_agenda_preprocess_event($variables);

}

/**
 * Preprocess the variables for the production summary.
 * @see culturefeed-production-summary.tpl.php
 */
function culturefeed_agenda_preprocess_culturefeed_production_summary(&$variables) {

  _culturefeed_agenda_preprocess_agenda($variables);
  _culturefeed_agenda_preprocess_agenda_summary($variables);
  _culturefeed_agenda_preprocess_production($variables);

}

/**
 * Preprocess the variables for the event detail.
 * @see culturefeed-event.tpl.php
 */
function culturefeed_agenda_preprocess_culturefeed_event(&$variables) {
  _culturefeed_agenda_preprocess_agenda($variables);
  _culturefeed_agenda_preprocess_event($variables);
  _culturefeed_agenda_preprocess_agenda_detail($variables);
}

/**
 * Helper preprocess function to preprocess the general agenda variables.
 */
function _culturefeed_agenda_preprocess_agenda(&$variables) {

  global $language;

  $item = $variables['item'];

  $cdb_item = $item->getEntity();
  $detail = $cdb_item->getDetails()->getDetailByLanguage($language->language);
  if ($detail) {

    $variables['title'] = $detail->getTitle();
    $variables['shortdescription'] = $detail->getShortDescription();
    $variables['longdescription'] = $detail->getLongDescription();

    if ($detail->getPrice()) {
      $variables['price'] = $detail->getPrice()->getValue();
      $variables['price_description'] = $detail->getPrice()->getDescription();
    }

  }

  $variables['cdbid'] = $cdb_item->getCdbid();

  // Social stats.
  $variables['recommend_count'] = $item->getActivityCount(\CultuurNet\Search\ActivityStatsExtendedEntity::ACTIVITY_COUNT_RECOMMEND);
  $variables['comment_count'] = $item->getActivityCount(\CultuurNet\Search\ActivityStatsExtendedEntity::ACTIVITY_COUNT_COMMENT);
  $variables['attend_count'] = $item->getActivityCount(\CultuurNet\Search\ActivityStatsExtendedEntity::ACTIVITY_COUNT_ATTEND);
  $variables['like_count'] = $item->getActivityCount(\CultuurNet\Search\ActivityStatsExtendedEntity::ACTIVITY_COUNT_LIKE);

  // Themes from this item.
  $themes = $cdb_item->getCategories()->getCategoriesByType(CultureFeed_Cdb_Data_Category::CATEGORY_TYPE_THEME);
  if ($themes) {
    array_walk($themes, function(&$item) {
      $item = check_plain($item->getName());
    });
    $variables['themes'] = $themes;
  }

  // All categories.
  $categories = $cdb_item->getCategories();
  $variables['categories'] = array();
  foreach ($categories as $category) {
    $variables['categories'][] = check_plain($category->getName());
  }

}

/**
 * Helper preprocess function to preprocess the general agenda summary variables.
 */
function _culturefeed_agenda_preprocess_agenda_summary(&$variables) {

  global $language;

  $item = $variables['item'];
  $cdb_item = $item->getEntity();
  $detail = $cdb_item->getDetails()->getDetailByLanguage($language->language);

  // Add thumbnail image.
  if ($detail) {
    $pictures = $detail->getMedia()->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_PHOTO);
    if (count($pictures) > 0) {
      $variables['thumbnail'] = $pictures->current()->getHLink();
    }
  }

}

/**
 * Helper preprocess function to preprocess the general agenda detail variables.
 */
function _culturefeed_agenda_preprocess_agenda_detail(&$variables) {

  global $language;

  $item = $variables['item'];
  $cdb_item = $item->getEntity();
  $detail = $cdb_item->getDetails()->getDetailByLanguage($language->language);

  // Add thumbnail image.
  if ($detail) {
    $pictures = $detail->getMedia()->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_PHOTO);
    foreach ($pictures as $picture) {
      $variables['pictures'][] = $picture->getHLink();
      if ($picture->isMain()) {
        $variables['main_picture'] = $picture->getHLink();
      }
    }
  }

}

/**
 * Preprocess the variables for one of the event templates.
 */
function _culturefeed_agenda_preprocess_event(&$variables) {

  global $language;

  $event = $variables['item']->getEntity();
  $event_detail = $event->getDetails()->getDetailByLanguage($language->language);
  $organiser = $event->getOrganiser();

  $variables['agefrom'] = $event->getAgeFrom();

  // Location information.
  $variables['has_location'] = FALSE;
  if ($event->getLocation()) {

    $variables['has_location'] = TRUE;

    $location = $event->getLocation();

    if ($location->getActor()) {
      $actor_detail = $location->getActor()->getDetails()->getDetailByLanguage($language->language);
      $variables['where'] = $actor_detail->getTitle();
      $variables['where_linked'] = culturefeed_agenda_detail_l('actor', $actor->getCdbid(), $variables['where']);
    }
    elseif ($location->getLabel()) {
      $variables['where'] = $location->getLabel();
    }

    $physical_address = $location->getAddress()->getPhysicalAddress();
    $variables['city'] = $physical_address->getCity();
    $variables['street'] = $physical_address->getStreet() . ' '  . $physical_address->getHouseNumber();
    $variables['zip'] = $physical_address->getZip();

  }

  $actor = NULL;
  // Preprocess the organiser variables.
  if ($organiser) {

    // If the organiser is a reference to an actor, link to it.
    if ($organiser->getActor() && $organiser->getActor()->getDetailByLanguage($language->language)) {
      $actor = $organiser->getActor();
      $organiser_actor_details = $organiser->getActor()->getDetailByLanguage($language->language);
      $variables['organiser'] = $organiser_actor_details->getTitle();
      $variables['organiser_link'] = culturefeed_agenda_detail_l('production', $actor->getCdbid(), $variables['organiser']);
    }
    elseif ($organiser->getLabel()) {
      $variables['organiser'] = $organiser->getLabel();
    }

  }

  if ($event_detail) {

      // Add a link to the organiser => Media type culturefeed-page;
    if (empty($actor) && $event_detail->getMedia()) {
      $page_links = $event_detail->getMedia()->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_CULTUREFEED_PAGE);
      if ($page_links->count() > 0) {
        $variables['organiser_link'] = l($variables['organiser'], $page_links->current()->getHLink());
      }
    }

    // Link to detail.
    $variables['link'] = culturefeed_agenda_detail_url('event', $event->getCdbid(), $event_detail->getTitle());

    // When.
    if ($event_detail) {
      $variables['when'] = $event_detail->getCalendarSummary();
    }

    // Reservation link.
    $reservation_links = $event_detail->getMedia()->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_RESERVATIONS);
    foreach ($reservation_links as $link) {
      $variables['reservation_links'][] = $link->getHLink();
    }

  }

}

/**
 * Preprocess the general variables for one of the production templates.
 */
function _culturefeed_agenda_preprocess_production(&$variables) {

  global $language;
  $production = $variables['item']->getEntity();
  $production_detail = $production->getDetails()->getDetailByLanguage($language->language);

  $variables['agefrom'] = $production->getAgeFrom();

  $relations = $production->getRelations();
  $total_relations = count($relations);
  // If a production has multiple events. Show general information.
  if ($total_relations > 1) {
    $variables['where'] = 'meerdere locaties';
    $variables['where'] = 'meerdere data';
  }

  // If only 1 event. Show calendar information from that event.
  elseif ($total_relations == 1) {

    try {
      $event_search = culturefeed_agenda_event_load($production->relations[0]->getCdbid());
      $variables['event'] = $event;
      $event_detail = $event_search->getEntity()->getDetails()->getDetailByLanguage($language->language);
      if ($event_detail) {
        $variables['when'] = $event_detail->getCalendarSummary();
      }

    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_agenda', $e);
    }

  }
  // If no event, check if this productions is in regions.
  else {

    $region_data = $production->getCategories()->getCategoriesByType('flandersregion');
    if ($region_data) {

      $regions = array();
      foreach ($region_data as $region) {
        $regions[] = check_plain($region->getName());
      }

      $variables['where'] = 'op verplaatsing in ' . implode(', ', $regions);

    }
    // No regions.
    else {
      $variables['where'] = 'enkel op afspraak';
    }

  }

  if ($production_detail) {

    // Performers
    $performers = $production_detail->getPerformers();
    $performer_labels = array();
    foreach ($performers as $performer) {
      $performer_labels[] = $performer->getLabel();
    }

    $variables['performers'] = implode(', ', $performer_labels);

    // Link to detail.
    $variables['link'] = culturefeed_agenda_detail_url('production', $production->getCdbid(), $production_detail->getTitle());

  }


}