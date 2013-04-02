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

  _culturefeed_agenda_culturefeed_agenda($variables);
  _culturefeed_agenda_culturefeed_agenda_summary($variables);
  _culturefeed_agenda_preprocess_event($variables);

}

/**
 * Preprocess the variables for the production summary.
 * @see culturefeed-production-summary.tpl.php
 */
function culturefeed_agenda_preprocess_culturefeed_production_summary(&$variables) {

  _culturefeed_agenda_culturefeed_agenda($variables);
  _culturefeed_agenda_culturefeed_agenda_summary($variables);
  _culturefeed_agenda_preprocess_production($variables);

}

/**
 * Preprocess the variables for the event detail.
 * @see culturefeed-event.tpl.php
 */
function culturefeed_agenda_preprocess_culturefeed_event(&$variables) {
  _culturefeed_agenda_culturefeed_agenda($variables);
}

/**
 * Helper preprocess function to preprocess the general agenda variables.
 */
function _culturefeed_agenda_culturefeed_agenda(&$variables) {

  global $language;

  $item = $variables['item'];

  $cdb_item = $item->getEntity();
  $detail = $cdb_item->getDetails()->getDetailByLanguage($language->language);
  if ($detail) {
    $variables['title'] = $detail->getTitle();
    $variables['shortdescription'] = $detail->getShortDescription();
    $variables['longdescription'] = $detail->getLongDescription();
  }

  $variables['cdbid'] = $cdb_item->getCdbid();

  // Social stats:
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
    $variables['themes'] = implode(',', $themes);
  }

}

/**
 * Helper preprocess function to preprocess the general agenda summary variables.
 */
function _culturefeed_agenda_culturefeed_agenda_summary(&$variables) {

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
 * Preprocess the variables for one of the event templates.
 */
function _culturefeed_agenda_preprocess_event(&$variables) {

  global $language;

  $event = $variables['item']->getEntity();
  $event_detail = $event->getDetails()->getDetailByLanguage($language->language);
  $organiser = $event->getOrganiser();

  $variables['agefrom'] = $event->getAgeFrom();

  // Location information.
  if ($event->getLocation()) {

    $location = $event->getLocation();

    if ($location->getActor()) {
      $actor_detail = $location->getActor()->getDetailByLanguage($language->language);
      $variables['where'] = $actor_detail->getTitle();
    }
    elseif ($location->getLabel()) {
      $variables['where'] = $location->getLabel();
    }

    $variables['city'] = $location->getAddress()->getPhysicalAddress()->getCity();

  }

  // Preprocess the organiser variables.
  if ($organiser) {

    // If the organiser is a reference to an actor, link to it.
    if ($organiser->getActor() && $organiser->getActor()->getDetailByLanguage($language->language)) {
      $actor = $organiser->getActor();
      $organiser_actor_details = $organiser->getActor()->getDetailByLanguage($language->language);
      $variables['organiser'] = $organiser_actor_details->getTitle();
    }
    elseif ($organiser->getLabel()) {
      $variables['organiser'] = $organiser->getLabel();
    }

  }

  if ($event_detail) {

      // Add a link to the organiser => Media type culturefeed-page;
    if ($event_detail->getMedia()) {
      $page_links = $event_detail->getMedia()->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_CULTUREFEED_PAGE);
      if ($page_links->count() > 0) {
        $variables['organiser_link'] = $page_links->current()->getHLink();
      }
    }

    // Link to detail.
    $variables['link'] = culturefeed_agenda_detail_url('event', $event->getCdbid(), $event_detail->getTitle());

    // When.
    if ($event_detail) {
      $variables['when'] = $event_detail->getCalendarSummary();
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

  }


}