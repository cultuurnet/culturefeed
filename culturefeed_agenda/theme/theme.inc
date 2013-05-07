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
 * Preprocess the variables for the event short summary.
 * @see culturefeed-event-short-summary.tpl.php
 */
function culturefeed_agenda_preprocess_culturefeed_event_short_summary(&$variables) {

  _culturefeed_agenda_preprocess_agenda($variables);
  _culturefeed_agenda_preprocess_agenda_summary($variables);
  _culturefeed_agenda_preprocess_event($variables);

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
 * Preprocess the variables for the production summary.
 * @see culturefeed-production-summary.tpl.php
 */
function culturefeed_agenda_preprocess_culturefeed_production_summary(&$variables) {

  _culturefeed_agenda_preprocess_agenda($variables);
  _culturefeed_agenda_preprocess_agenda_summary($variables);
  _culturefeed_agenda_preprocess_production($variables);

}

/**
 * Preprocess the variables for the production short summary.
 * @see culturefeed-production-short-summary.tpl.php
 */
function culturefeed_agenda_preprocess_culturefeed_production_short_summary(&$variables) {

  _culturefeed_agenda_preprocess_agenda($variables);
  _culturefeed_agenda_preprocess_agenda_summary($variables);
  _culturefeed_agenda_preprocess_production($variables);

}

/**
 * Preprocess the variables for the production detail.
 * @see culturefeed-production.tpl.php
 */
function culturefeed_agenda_preprocess_culturefeed_production(&$variables) {
  _culturefeed_agenda_preprocess_agenda($variables);
  _culturefeed_agenda_preprocess_production($variables);
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
      $variables['price'] = $detail->getPrice()->getValue() > 0 ? '&euro; ' . $detail->getPrice()->getValue() : 'gratis';
      $variables['price_description'] = $detail->getPrice()->getDescription();
    }

    // Ticket links.
    $ticket_links = $detail->getMedia()->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_RESERVATIONS);
    $variables['tickets'] = array();
  	foreach ($ticket_links as $link) {
  		$variables['tickets'][] = l('Bestel tickets', $link->getHLink(), array('attributes' => array('class' => 'reservation-link', 'rel' => 'nofollow')));
  	}

  }

  $variables['cdbid'] = $cdb_item->getCdbid();

  // Social stats.
  $variables['recommend_count'] = $item->getActivityCount(\CultuurNet\Search\ActivityStatsExtendedEntity::ACTIVITY_COUNT_RECOMMEND);
  $variables['recommend_link'] = culturefeed_social_activity_recommend_link($item);
  
  $variables['comment_count'] = $item->getActivityCount(\CultuurNet\Search\ActivityStatsExtendedEntity::ACTIVITY_COUNT_COMMENT);
  
  $variables['attend_count'] = $item->getActivityCount(\CultuurNet\Search\ActivityStatsExtendedEntity::ACTIVITY_COUNT_ATTEND);
  
  $variables['like_count'] = $item->getActivityCount(\CultuurNet\Search\ActivityStatsExtendedEntity::ACTIVITY_COUNT_LIKE);

  // Themes from this item.
  $variables['themes'] = array();
  $themes = $cdb_item->getCategories()->getCategoriesByType(CultureFeed_Cdb_Data_Category::CATEGORY_TYPE_THEME);
  if ($themes) {
    array_walk($themes, function(&$item) {
      $item = check_plain($item->getName());
    });
    $variables['themes'] = $themes;
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

    // Add videos.
    $videos = $detail->getMedia()->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_VIDEO);
  	foreach ($videos as $video) {
			if ($video_embed = _culturefeed_search_ui_video_url_to_embed($video->getHLink(), 240, 180)) {
				$variables['videos'][] = $video_embed;
  		}
  	}

  }

  // All categories.
  $categories = $cdb_item->getCategories();
  $variables['categories'] = array();
  foreach ($categories as $category) {
    $variables['categories'][] = check_plain($category->getName());
  }

	// Links.
	$links = _culturefeed_agenda_get_links_from_item($cdb_item);
	$variables['links'] = array();
  foreach ($links as $link) {
    // clean up link text
    $link_text = str_replace('http://', '', $link);
    $link_text = str_replace('https://', '', $link_text);
    $variables['links'][] = l(truncate_utf8($link_text, 40, FALSE, TRUE), $link, array('attributes' => array('rel' => 'nofollow')));
  }

  $variables['keywords'] = $cdb_item->getKeywords();

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
  $variables['location'] = '';
  if ($event->getLocation()) {

    $location = $event->getLocation();

    if ($location->getActor()) {
      $actor_detail = $location->getActor()->getDetails()->getDetailByLanguage($language->language);
      $variables['location']['title'] = $actor_detail->getTitle();
      $variables['location']['link'] = culturefeed_search_detail_l('actor', $location->getActor()->getCdbid(), $variables['location']['title']);
    }
    elseif ($location->getLabel()) {
      $variables['location']['title'] = $location->getLabel();
    }

    $physical_address = $location->getAddress()->getPhysicalAddress();
    $variables['location']['city'] = $physical_address->getCity();
    $variables['location']['street'] = $physical_address->getStreet() . ' '  . $physical_address->getHouseNumber();
    $variables['location']['zip'] = $physical_address->getZip();

  }

  $actor = NULL;
  $variables['organiser'] = array();
  // Preprocess the organiser variables.
  if ($organiser) {

    // If the organiser is a reference to an actor, link to it.
    if ($organiser->getActor() && $organiser->getActor()->getDetails()->getDetailByLanguage($language->language)) {
      $actor = $organiser->getActor();
      $organiser_actor_details = $organiser->getActor()->getDetails()->getDetailByLanguage($language->language);
      $variables['organiser']['title'] = $organiser_actor_details->getTitle();
      $variables['organiser']['link'] = culturefeed_search_detail_l('production', $actor->getCdbid(), $variables['organiser']['title']);
    }
    elseif ($organiser->getLabel()) {
      $variables['organiser']['title'] = $organiser->getLabel();
    }

  }

  // Contact information
  $variables['contact'] = array();
  if ($event->getContactInfo()) {

    $contact_info = $event->getContactInfo();

    // Mails.
    $mails = array();
    foreach ($contact_info->getMails() as $mail) {
      $mails[] = l($mail->getMailAddress(), 'mailto:' . $mail->getMailAddress());
    }
    $variables['contact']['mail'] = implode(', ', $mails);

    // Fax + phone numbers.
    $phones = array();
    $faxes = array();
    foreach ($contact_info->getPhones() as $phone) {
      if ($phone->getType() == CultureFeed_Cdb_Data_Phone::PHONE_TYPE_PHONE) {
        $phones[] = $phone->getNumber();
      }
      else {
        $faxes[] = $phone->getNumber();
      }
    }
    $variables['contact']['phone'] = implode(', ', $phones);
    $variables['contact']['fax'] = implode(', ', $faxes);

    // Coordinates.
    $addresses = $contact_info->getAddresses();
    if ($addresses) {
      $address = current($addresses);
      $coordinates = $address->getPhysicalAddress()->getGeoInformation();
      if ($coordinates) {
        $variables['coordinates'] = array(
          'lat' => $coordinates->getXCoordinate(),
          'lng' => $coordinates->getYCoordinate(),
        );
      }
    }

    // Reservation data.
    $variables['reservation'] = $contact_info->getReservationInfo();

  }

  if ($event_detail) {

    // Link to detail.
    $variables['url'] = culturefeed_search_detail_url($variables['item']->getType(), $event->getCdbid(), $event_detail->getTitle());

    // When.
    $variables['when'] = $event_detail->getCalendarSummary();

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
    $variables['location']['title'] = 'meerdere locaties';
    $variables['location']['title'] = 'meerdere data';
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

      // Add location information from the event.
      $location = $event->getLocation();

      if ($location->getActor()) {
        $actor_detail = $location->getActor()->getDetails()->getDetailByLanguage($language->language);
        $variables['location']['title'] = $actor_detail->getTitle();
        $variables['location']['link'] = culturefeed_search_detail_l('actor', $location->getActor()->getCdbid(), $variables['location']['title']);
      }
      elseif ($location->getLabel()) {
        $variables['location']['title'] = $location->getLabel();
      }

      $physical_address = $location->getAddress()->getPhysicalAddress();
      $variables['location']['city'] = $physical_address->getCity();
      $variables['location']['street'] = $physical_address->getStreet() . ' '  . $physical_address->getHouseNumber();
      $variables['location']['zip'] = $physical_address->getZip();

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

      $variables['location']['title'] = 'op verplaatsing in';
      $variables['regions'] = $regions;

    }
    // No regions.
    else {
      $variables['location']['title'] = 'enkel op afspraak';
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
    $variables['link'] = culturefeed_search_detail_url('production', $production->getCdbid(), $production_detail->getTitle());

  }


}