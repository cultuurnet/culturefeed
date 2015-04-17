<?php
/**
 * @file
 * Theme / preprocess functions for culturefeed agenda.
 */

/**
 * Preprocess the variables for the actor summary.
 * @see culturefeed-actor-summary.tpl.php
 */
function culturefeed_agenda_preprocess_culturefeed_actor_summary(&$variables, $include_social_links = TRUE) {

  _culturefeed_agenda_preprocess_agenda($variables, $include_social_links);
  _culturefeed_agenda_preprocess_agenda_summary($variables);
  _culturefeed_agenda_preprocess_actor($variables);

}

/**
 * Preprocess the variables for the actor short summary.
 * @see culturefeed-actory-short-summary.tpl.php
 */
function culturefeed_agenda_preprocess_culturefeed_actor_short_summary(&$variables) {

  _culturefeed_agenda_preprocess_agenda($variables);
  _culturefeed_agenda_preprocess_agenda_summary($variables);
  _culturefeed_agenda_preprocess_actor($variables);

}

/**
 * Preprocess the variables for the actor detail.
 * @see culturefeed-actor.tpl.php
 */
function culturefeed_agenda_preprocess_culturefeed_actor(&$variables) {

  _culturefeed_agenda_preprocess_agenda($variables);
  _culturefeed_agenda_preprocess_actor($variables);
  _culturefeed_agenda_preprocess_agenda_detail($variables);

  _culturefeed_agenda_preprocess_actor_page_metatags($variables);

}

/**
 * Preprocess the variables for the event summary.
 * @see culturefeed-event-summary.tpl.php
 */
function culturefeed_agenda_preprocess_culturefeed_event_summary(&$variables, $include_social_links = TRUE) {

  _culturefeed_agenda_preprocess_agenda($variables, $include_social_links);
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
 * Preprocess the variables for the event mini summary.
 * @see culturefeed-event-mini-summary.tpl.php
 */
function culturefeed_agenda_preprocess_culturefeed_event_mini_summary(&$variables) {

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

  _culturefeed_agenda_preprocess_event_production_metatags($variables);
}

/**
 * Preprocess the variables for the production summary.
 * @see culturefeed-production-summary.tpl.php
 */
function culturefeed_agenda_preprocess_culturefeed_production_summary(&$variables, $include_social_links = TRUE) {

  _culturefeed_agenda_preprocess_agenda($variables, $include_social_links);
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
 * Preprocess the variables for the production mini summary.
 * @see culturefeed-production-mini-summary.tpl.php
 */
function culturefeed_agenda_preprocess_culturefeed_production_mini_summary(&$variables) {

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

  _culturefeed_agenda_preprocess_event_production_metatags($variables);
}

/**
 * Helper preprocess function to preprocess the general agenda variables.
 */
function _culturefeed_agenda_preprocess_agenda(&$variables) {

  $item = $variables['item'];

  $cdb_item = $item->getEntity();
  $detail = $cdb_item->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language());
  if ($detail) {

    $variables['title'] = check_plain($detail->getTitle());
    $variables['shortdescription'] = check_plain($detail->getShortDescription());
    $variables['longdescription'] = $detail->getLongDescription();

    // Ad id / title to Drupal settings for easy reference in GTM.
    drupal_add_js(array('culturefeed_search_ui' => array('gtm' => array(
      'events' => array(
        $item->getId() => $variables['title'],
      ),
    ))), 'setting');

    if ($detail->getPrice()) {
      $variables['price'] = $detail->getPrice()->getValue() > 0 ? '&euro; ' . $detail->getPrice()->getValue() : t('free');
      $variables['price_description'] = $detail->getPrice()->getDescription();
    }

    // Ticket links.
    $ticket_links = $detail->getMedia()->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_RESERVATIONS);

    $variables['tickets'] = array();
    $variables['ticket_buttons'] = array();

    foreach ($ticket_links as $link) {
      $variables['tickets'][] = l(t('Buy tickets'), $link->getHLink(), array('attributes' => array('class' => 'reservation-link', 'id' => 'cf-ticket')));

      // Provide some themable ticket buttons.
      $button = array();
      $button['text'] = t('Buy tickets');
      $button['link'] = $link->getHLink();
      $button['options'] = array('attributes' => array('class' => 'reservation-link button', 'id' => 'cf-ticket'));
      $variables['ticket_buttons'][] = $button;
    }

    // Performers
    if (method_exists($detail, 'getPerformers')) {
      $performers = $detail->getPerformers();
      $variables['performers'] = '';
      if ($performers) {

        $performer_labels = array();
        foreach ($performers as $performer) {
          $performer_labels[] = check_plain($performer->getLabel());
        }

        $variables['performers'] = implode(', ', $performer_labels);
      }
    }

    // Pictures
    $main_picture = '';
    $pictures = array();

    $photos = $detail->getMedia()->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_PHOTO);
    foreach ($photos as $photo) {
      $pictures[] = $photo->getHLink();
      if ($photo->isMain()) {
        $main_picture = $photo->getHLink();
      }
    }

    // Support for mediatype imageweb
    $images = $detail->getMedia()->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_IMAGEWEB);
    foreach ($images as $image) {
      $pictures[] = $image->getHLink();
      if ($image->isMain() && empty($main_picture)) {
        $main_picture = $image->getHLink();
      }
    }

    // If no image we get image from eventdetails language=nl
    if (empty($pictures)) {

      $detail_nl = $cdb_item->getDetails()->getDetailByLanguage('nl');
      if (!empty($detail_nl)) {
        $photos = $detail_nl->getMedia()->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_PHOTO);
        foreach ($photos as $photo) {
          $pictures[] = $photo->getHLink();
          if ($photo->isMain()) {
            $main_picture = $photo->getHLink();
          }
        }

        // Support for mediatype imageweb
        $images = $detail_nl->getMedia()->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_IMAGEWEB);
        foreach ($images as $image) {
          $pictures[] = $image->getHLink();
          if ($image->isMain() && empty($main_picture)) {
            $main_picture = $image->getHLink();
          }
        }
      }
    }

    // If no main picture is set we use first picture
    if (empty($main_picture) && count($pictures) > 0) {
      $main_picture = $pictures[0];
    }

    // we don't want the main picture to show twice
    if (!empty($main_picture)) {
      unset($pictures[0]);
    }

    $variables['main_picture'] = $main_picture;
    $variables['pictures'] = $pictures;

    // Make sure usage of thumbnail is backwards compatible
    $variables['thumbnail'] = $variables['main_picture'];

  }

  $variables['cdbid'] = $cdb_item->getCdbid();

  // Social stats.
  $variables['recommend_count'] = $item->getActivityCount(\CultuurNet\Search\ActivityStatsExtendedEntity::ACTIVITY_COUNT_RECOMMEND);
  $variables['comment_count'] = $item->getActivityCount(\CultuurNet\Search\ActivityStatsExtendedEntity::ACTIVITY_COUNT_COMMENT);
  $variables['attend_count'] = $item->getActivityCount(\CultuurNet\Search\ActivityStatsExtendedEntity::ACTIVITY_COUNT_ATTEND);
  $variables['like_count'] = $item->getActivityCount(\CultuurNet\Search\ActivityStatsExtendedEntity::ACTIVITY_COUNT_LIKE);

  // Social actions.
  $variables['recommend_link'] = '';
  $variables['attend_link'] = '';
  if (module_exists('culturefeed_social')) {
    $variables['recommend_link'] = culturefeed_social_activity_link(CultureFeed_Activity::TYPE_RECOMMEND, $item);
    $variables['attend_link'] = culturefeed_social_activity_link(CultureFeed_Activity::TYPE_IK_GA, $item);
  }

  // Themes of this item (as plain text and as links).
  $themes = culturefeed_agenda_get_category_list_of_item(CultureFeed_Cdb_Data_Category::CATEGORY_TYPE_THEME, $cdb_item);
  $variables['themes'] = $themes['text'];
  $variables['themes_links'] = $themes['links'];

  // Types of this item (plain text and links).
  $types = culturefeed_agenda_get_category_list_of_item(CultureFeed_Cdb_Data_Category::CATEGORY_TYPE_EVENT_TYPE, $cdb_item);
  $variables['types'] = $types['text'];
  $variables['types_links'] = $types['links'];

  // Preprocess the organiser variables.
  if (method_exists($cdb_item, 'getOrganiser')) {

    $organiser = $cdb_item->getOrganiser();
    $variables['organiser'] = array();
    if ($organiser) {

      // If the organiser is a reference to an actor, link to it.
      if ($organiser->getActor() && $organiser->getActor()->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language())) {
        $actor = $organiser->getActor();
        $organiser_actor_details = $organiser->getActor()->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language());
        $variables['organiser']['title'] = $organiser_actor_details->getTitle();
        $variables['organiser']['link'] = culturefeed_search_detail_l('actor', $actor->getCdbid(), $variables['organiser']['title']);
      }
      elseif ($organiser->getLabel()) {
        $variables['organiser']['title'] = $organiser->getLabel();
        if ($organiser->getCdbid()) {
          $variables['organiser']['link'] = culturefeed_search_detail_l('actor', $organiser->getCdbid(), $variables['organiser']['title']);
        }
      }

    }
  }

  $keywords = $cdb_item->getKeywords();
  $variables['keywords'] = '';

  if ($keywords) {
		$variables['raw_keywords'] = array();
    $linked_keywords = array();
    foreach ($keywords as $keyword) {
			$variables['raw_keywords'][] = $keyword;
      $linked_keywords[] = l($keyword, 'agenda/search', array('query' => array('keyword' => urlencode($keyword))));
    }

    $variables['keywords'] = implode(', ', $linked_keywords);
    // check if event is suitable for kids
    if (array_key_exists('ook voor kinderen', $keywords)) {
      $variables['forkids'] = TRUE;
    }
  }

}

/**
 * Helper preprocess function to preprocess the general agenda summary variables.
 */
function _culturefeed_agenda_preprocess_agenda_summary(&$variables) {
  _culturefeed_agenda_preprocess_agenda($variables);
}

/**
 * Helper preprocess function to preprocess the general agenda detail variables.
 */
function _culturefeed_agenda_preprocess_agenda_detail(&$variables) {

  $item = $variables['item'];
  $cdb_item = $item->getEntity();
  $detail = $cdb_item->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language());

  if ($detail) {

    // Add videos.
    $variables['videos'] = array();
    $videos = $detail->getMedia()->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_VIDEO);
    foreach ($videos as $video) {
      if ($video_embed = _culturefeed_search_ui_video_url_to_embed($video->getHLink())) {
        $variables['videos'][] = $video_embed;
      }
    }

    // Absolute link to detail.
    $variables['share_url'] = url($variables['url'], array('absolute' => TRUE));
    
    $variables['facebook'] = array();
    $variables['googleplus'] = array();
    $variables['twitter'] = array();
    $variables['whatsapp'] = array();

    // Set prefix for share URL's.
    $variables['facebook']['prefix'] = 'https://www.facebook.com/sharer/sharer.php?u=';
    $variables['googleplus']['prefix'] = 'https://plus.google.com/share?url=';
    $variables['twitter']['prefix'] = 'https://twitter.com/intent/tweet?text=';
    $variables['whatsapp']['prefix'] = 'whatsapp://send?text=';

    // Share URL's.      
    $variables['facebook']['url'] = $variables['facebook']['prefix'] . $variables['share_url'];
    $variables['googleplus']['url'] = $variables['googleplus']['prefix'] . $variables['share_url'];
    $variables['twitter']['url'] = $variables['twitter']['prefix'] . $variables['share_url'];
    $variables['whatsapp']['url'] = $variables['whatsapp']['prefix'] . $variables['share_url'];

    if (module_exists('forward')) {
      $variables['mail_url'] = url('forward', array('query' => array('path' => $variables['url'])));
    }
    else {
      $variables['mail_url'] = 'mailto:?subject=' . $variables['title'] . '&body=' . $variables['share_url']; 
    }

    // Prepare to push share activities to Culturefeed.
    if (culturefeed_is_culturefeed_user()) {
      $track = TRUE;
      $content_type = culturefeed_get_content_type($item->getType());
      $id = $item->getId();
      $track_facebook = url('culturefeed/do/' . CultureFeed_Activity::TYPE_FACEBOOK .'/' . $content_type . '/' . urlencode($id) . '/ajax');
      $track_twitter = url('culturefeed/do/' . CultureFeed_Activity::TYPE_TWITTER .'/' . $content_type . '/' . urlencode($id) . '/ajax');
    }
    
    // Add options.  
    $variables['facebook']['options'] = array(
      'attributes' => array(
        'class' => array('share-link','facebook-share'),
        'rel' => $track ? $track_facebook : '' ,
      ),
    );
    $variables['googleplus']['options'] = array(
      'attributes' => array(
        'class' => array('share-link','googleplus-share'),
      ),
    );
    $variables['twitter']['options'] = array(
      'attributes' => array(
        'class' => array('share-link','twitter-share'),
        'rel' => $track ? $track_twitter : '' ,
      ),
    );
    $variables['whatsapp']['options'] = array(
      'attributes' => array(
        'class' => array('share-link','whatsapp-share'),
      ),
      'external' => TRUE,
    );
    $variables['mail']['options'] = array(
      'attributes' => array(
        'class' => array('share-link','forward-mail'),
      ),
      'external' => TRUE,
    );

    // Share Links.      
    $variables['facebook_link'] = l(t('Share on Facebook'), $variables['facebook']['url'], $variables['facebook']['options']);
    $variables['googleplus_link'] = l(t('Share on Google+'), $variables['googleplus']['url'], $variables['googleplus']['options']);
    $variables['twitter_link'] = l(t('Share on Twitter'), $variables['twitter']['url'], $variables['twitter']['options']);
    $variables['whatsapp_link'] = l(t('Share on Whatsapp'), $variables['whatsapp']['url'], $variables['whatsapp']['options']);
    $variables['mail_link'] = l(t('Mail'), $variables['mail_url'], $variables['mail']['options']);
    
    // Use dialog when Facebook app id is set.
    culturefeed_search_ui_add_facebook_share();

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
    $variables['links'][] = l(truncate_utf8($link_text, 40, FALSE, TRUE), $link, array('attributes' => array('id' => 'cf-link')));
  }

  if (module_exists('culturefeed_social')) {
    $variables['recommend_link'] = culturefeed_social_activity_link(CultureFeed_Activity::TYPE_RECOMMEND, $item, FALSE, FALSE, '', '', FALSE);
  }
}

/**
 * Preprocess the variables for one of the event templates.
 */
function _culturefeed_agenda_preprocess_event(&$variables) {

  $event = $variables['item']->getEntity();
  $event_detail = $event->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language());

  $variables['agefrom'] = check_plain($event->getAgeFrom());
  if (is_numeric($event->getAgeFrom()) && ($event->getAgeFrom() <= 12)) {
    $variables['forkids'] = TRUE;
  }
  $variables['location'] = culturefeed_agenda_get_location_of_event($event);

  $actor = NULL;

  // Contact information
  $variables['contact'] = array();
  if ($event->getContactInfo()) {

    $contact_info = $event->getContactInfo();

    // Mails.
    $mails = array();
    foreach ($contact_info->getMails() as $mail) {
      $mails[] = l($mail->getMailAddress(), 'mailto:' . $mail->getMailAddress(), array('attributes' => array('id' => 'cf-contact')));
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

    // Reservation data.
    $variables['reservation'] = $contact_info->getReservationInfo();
    $reservation_info = $variables['reservation'];

      // Reservation mail
      if (isset($reservation_info['mails'])) {
        foreach ($reservation_info['mails'] as $mail) {
          $reservation_mails[] = l($mail, 'mailto:' . $mail, array('attributes' => array('id' => 'cf-reservation')));
        }
        $variables['reservation']['mail'] = implode(', ', $reservation_mails);
      }

      // Reservation phone
      if (isset($reservation_info['phone'])) {
        foreach ($reservation_info['phone'] as $phone) {
          $reservation_phones[] = $phone;
        }
        $variables['reservation']['phone'] = implode(', ', $reservation_phones);
      }

      // Reservation url
      if (isset($reservation_info['url'])) {
        foreach ($reservation_info['url'] as $url) {
          // clean up link text
          $url_text = str_replace('http://', '', $url);
          $url_text = str_replace('https://', '', $url_text);
          $reservation_urls[] = l(truncate_utf8($url_text, 40, FALSE, TRUE), $url, array('attributes' => array('id' => 'cf-reservation')));
        }
        $variables['reservation']['url'] = implode(', ', $reservation_urls);
      }

    // Coordinates.
    $addresses = $contact_info->getAddresses();
    if ($addresses) {
      $address = current($addresses);
      $coordinates = $address->getPhysicalAddress()->getGeoInformation();
      if ($coordinates) {
        $variables['coordinates'] = array(
          'lat' => $coordinates->getYCoordinate(),
          'lng' => $coordinates->getXCoordinate(),
        );
      }
    }

  }

  // Calendar.
  if ($calendar = $event->getCalendar()) {
    $variables['calendar'] = culturefeed_agenda_parse_calendar($calendar);
  }

  if ($event_detail) {

    // Link to detail.
    $variables['url'] = culturefeed_search_detail_url($variables['item']->getType(), $event->getCdbid(), $event_detail->getTitle());

    // When.
    $variables['when'] = check_plain($event_detail->getCalendarSummary());

  }

  // Personal calendar buttons.
  if (module_exists('culturefeed_calendar')) {
    $variables['personal_calendar_buttons'] = culturefeed_calendar_render_calendar_buttons($variables['item']);
  }
}

/**
 * Preprocess the general variables for one of the production templates.
 */
function _culturefeed_agenda_preprocess_production(&$variables) {

  $production = $variables['item']->getEntity();
  $production_detail = $production->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language());
  $organiser = $production->getOrganiser();

  $variables['agefrom'] = $production->getAgeFrom();
  if (is_numeric($production->getAgeFrom()) && ($production->getAgeFrom() <= 12)) {
    $variables['forkids'] = TRUE;
  }

  $relations = $production->getRelations();
  $total_relations = count($relations);
  $variables['relations'] = $total_relations;

  // If a production has multiple events. Show general information.
  if ($total_relations > 1) {
    $variables['location']['title'] = t('multiple locations');
    $variables['when'] = t('multiple dates');
  }

  // If only 1 event. Show calendar information from that event.
  elseif ($total_relations == 1) {

    try {
      $event = culturefeed_agenda_event_load($relations[key($relations)]->getCdbid());
      $variables['event'] = $event;
      if ($event) {
        $event_detail = $event->getEntity()->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language());
        if ($event_detail) {
          $variables['when'] = $event_detail->getCalendarSummary();
        }

        // Add location information from the event.
        $variables['location'] = culturefeed_agenda_get_location_of_event($event->getEntity());
      }
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_agenda', $e);
    }

  }
  // If no event, check if this productions is in regions.
  else {

    // Check for regions
    $region_data = $production->getCategories()->getCategoriesByType('flandersregion');

    if ($region_data) {

      $regions = array();
      foreach ($region_data as $region) {
        $regions[] = check_plain($region->getName());
      }

      $variables['location']['title'] = t('on location in');
      $variables['regions'] = $regions;

    }

    // Check for workingregions
    $workingregion_data = $production->getCategories()->getCategoriesByType('workingregion');

    if ($workingregion_data) {

      $workingregions = array();
      foreach ($workingregion_data as $workingregion) {
        $workingregions[] = check_plain($workingregion->getName());
      }

      $variables['location']['title'] = t('on location in');
      $variables['workingregions'] = $workingregions;

    }

    // No regions or workingregions.
    if (!($region_data) && !($workingregion_data)) {
      $variables['location']['title'] = t('by appointment');
    }

  }

  if ($production_detail) {

    // Link to detail.
    $variables['url'] = culturefeed_search_detail_url('production', $production->getCdbid(), $production_detail->getTitle());

  }

}

/**
 * Preprocess the general variables for an actor.
 */
function _culturefeed_agenda_preprocess_actor(&$variables) {

  $actor = $variables['item']->getEntity();

  // Location information.
  $variables['location'] = '';
  $variables['contact'] = array();
  if ($actor->getContactInfo()) {

    $contact_info = $actor->getContactInfo();
    $locations = culturefeed_agenda_get_locations_of_item($actor);
    if ($locations) {
      $variables['location'] = $locations[0];
    }

    // Coordinates.
    $addresses = $contact_info->getAddresses();
    if ($addresses) {
      $address = current($addresses);
      $coordinates = $address->getPhysicalAddress()->getGeoInformation();
      if ($coordinates) {
        $variables['coordinates'] = array(
          'lat' => $coordinates->getYCoordinate(),
          'lng' => $coordinates->getXCoordinate(),
        );
      }
    }

    // Mails.
    $mails = array();
    foreach ($contact_info->getMails() as $mail) {
      $mails[] = l($mail->getMailAddress(), 'mailto:' . $mail->getMailAddress(), array('attributes' => array('id' => 'cf-contact')));
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
    $variables['contact']['phone'] = check_plain(implode(', ', $phones));
    $variables['contact']['fax'] = check_plain(implode(', ', $faxes));

  }

  // Link to detail.
  $actor_detail = $actor->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language());
  if ($actor_detail) {
    $actor_title = check_plain($actor_detail->getTitle());
    $variables['url'] = culturefeed_search_detail_url($variables['item']->getType(), $actor->getCdbid(), $actor_title);
  }

}

/**
 * Preprocess the agenda actor program.
 */
function culturefeed_agenda_preprocess_culturefeed_agenda_actor_program(&$variables) {

  $items = array();
  foreach ($variables['items'] as $item) {
    $items[] = theme('culturefeed_agenda_actor_program_item', array(
      'item' => $item,
    ));
  }
  $variables['items'] = $items;

  $variables['search_url'] = url('agenda/search', array('query' => array('actor' => $variables['actor']->getId())));

  $variables['title'] = culturefeed_agenda_detail_title($variables['actor']);

}

/**
 * Preprocess the agenda actor program.
 */
function culturefeed_agenda_preprocess_culturefeed_agenda_actor_program_item(&$variables) {
  culturefeed_agenda_preprocess_culturefeed_event_summary($variables);
}

/**
 * Preprocess the production program.
 * @see culturefeed-agenda-production-program.tpl.php
 */
function culturefeed_agenda_preprocess_culturefeed_agenda_production_program(&$variables) {

  $provinces = $variables['provinces'];
  $items = $variables['items'];

  $tabs = array();
  $other = array(
    'name' => t('Other'),
    'class' => 'other',
    'children' => array(),
  );
  foreach ($items as $item) {

    $region_found = FALSE;

    // Get all flanders regions, and check if a province was set.
    $regions = $item->getEntity()->getCategories()->getCategoriesByType(CultureFeed_Cdb_Data_Category::CATEGORY_TYPE_FLANDERS_REGION);
    if ($regions) {
      foreach ($regions as $region) {

        if (isset($provinces[$region->getId()])) {

          $province = $provinces[$region->getId()];

          // Make sure tab exists.
          if (!isset($tabs[$province->province_id])) {
            $tabs[$province->province_id] = array(
              'name' => $province->province_name,
              'class' => culturefeed_search_slug($province->province_name),
            );
          }

          $tabs[$province->province_id]['children'][] = culturefeed_agenda_get_production_program_data_for_item($item);

          $region_found = TRUE;
        }
      }
    }

    // Events without region.
    if (!$region_found) {
      $other['children'][] = culturefeed_agenda_get_production_program_data_for_item($item);
    }

  }

  if (count($other['children']) > 0) {
    $tabs['other'] = $other;
  }

  $variables['tabs'] = $tabs;

}

/**
 * Helper function to create a meta description tag for events and productions.
 * @param type $variables
 */
function _culturefeed_agenda_preprocess_event_production_metatags($variables) {

  $title_parts = array();
  $description_parts = array();

  if (!empty($variables['title'])) {
    $title_parts[] = $variables['title'];
  }
  if (!empty($variables['location']) && !empty($variables['location']['city'])) {
    $title_parts[] = $variables['location']['city'];
    $description_parts[] = t('Location: @location', array('@location' => $variables['location']['title'] . ', ' . $variables['location']['city']));
  }
  if (!empty($variables['when'])) {
    $description_parts[] = t('When: @when', array('@when' => $variables['when']));
  }
  if (!empty($variables['organiser'])) {
    $description_parts[] = t('Organisation: @organisation', array('@organisation' => $variables['organiser']['title']));
  }
  if (!empty($variables['price'])) {
    $description_parts[] = t('Price: € @price', array('@price' => str_replace('&euro; ' , '' , $variables['price'])));
  }
  if (!empty($variables['tickets'])) {
    $description_parts[] = t('Order your tickets');
  }

  // Add meta title tag.
  drupal_set_title(implode(' | ', $title_parts), PASS_THROUGH);

  // Add meta description tag.
  drupal_add_html_head(array(
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'description',
      'content' => implode(' - ', $description_parts),
    ),
  ), 'meta_description');

}

/**
 * Helper function to create a meta description tag for actors and pages.
 * @param type $variables
 */
function _culturefeed_agenda_preprocess_actor_page_metatags($variables) {

  $title_parts = array();
  $description_parts = array();

  if (!empty($variables['title'])) {
    $title_parts[] = $variables['title'];
  }
  if (!empty($variables['location']) && !empty($variables['location']['street'])) {
    $location = $variables['location']['street'];
    if (!empty($variables['location']['street'])) {
      $title_parts[] = $variables['location']['city'];
      if (!empty($variables['location']['zip'])) {
         $location .= ', ' . $variables['location']['zip'] . ' ' . $variables['location']['city'];
      }
    }
    $description_parts[] = t('Address: @location', array('@location' =>  $location));
  }
  if (!empty($variables['contact']) && !empty($variables['contact']['phone'])) {
    $description_parts[] = t('Phone number: @number', array('@number' => $variables['contact']['phone']));
  }

  // Add meta title tag.
  drupal_set_title(implode(' | ', $title_parts), PASS_THROUGH);

  // Add meta description field.
  drupal_add_html_head(array(
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'description',
      'content' => implode(' - ', $description_parts),
    ),
  ), 'meta_description');
}

/**
 * Preprocess the related activities.
 * @see culturefeed-agenda-related-activities.tpl.php
 */
function culturefeed_agenda_preprocess_culturefeed_agenda_related_activities(&$variables) {

  $results = $variables['results'];
  $variables['items'] = array();

  foreach ($results as $result) {
    $variables['items'][] = theme('culturefeed_' . $result->getType() . '_short_summary', array('item' => $result));
  }
}

/**
 * Preprocess event types.
 * @see culturefeed-agenda-event-types.tpl.php
 */
function culturefeed_agenda_preprocess_culturefeed_agenda_event_types(&$variables) {

}

/**
 * Preprocess the variables for a nearby activities list item.
 * @see culturefeed-agenda-nearby-activities-list-item.tpl.php
 */
function culturefeed_agenda_preprocess_culturefeed_agenda_nearby_activities_list_item(&$variables) {

  _culturefeed_agenda_preprocess_agenda($variables);
  _culturefeed_agenda_preprocess_event($variables);

}


/**
 * Implements hook_preprocess_culturefeed_agenda_de_lijn_widget().
 */
function culturefeed_agenda_preprocess_culturefeed_agenda_de_lijn_widget(&$variables) {

  $address_from = array();
  $address_to = array();

  $cf_account = NULL;
  try {
    $cf_account = DrupalCultureFeed::getLoggedInUser();
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_agenda', $e);
  }

  // Get addres from (user his address)
  if ($cf_account) {
    $delijn_str_nr = explode(' ', $cf_account->street);
    $delijn_nr = array_pop($delijn_str_nr);
    if (1 === preg_match('#[0-9]#', $delijn_nr)) {
      $delijn_str = str_replace($delijn_nr, '', $cf_account->street);
    }
    else {
      $delijn_nr = '';
      $delijn_str = $cf_account->street;
    }
    $address_from = array(
      'city' => $cf_account->city,
      'street' => $delijn_str,
      'nr' => $delijn_nr
    );
  }

  // Get address to.
  $cdb_item = $variables['item']->getEntity();
  switch ($variables['item']->getType()) {
    case 'actor':

      $contact_info = $cdb_item->getContactInfo();
      $addresses = $contact_info->getAddresses();
      if ($addresses) {
        $physical_address = $addresses[0]->getPhysicalAddress();
      }

      break;

    case 'event':

      if ($cdb_item->getLocation()) {
        $physical_address = $cdb_item->getLocation()->getAddress()->getPhysicalAddress();
      }

      break;
  }

  if ($physical_address) {
    $address_to = array(
      'city' => check_plain($physical_address->getCity()),
      'street' => check_plain($physical_address->getStreet()),
      'nr' => check_plain($physical_address->getHouseNumber()),
    );
  }

  $settings = array('address_from' => $address_from, 'address_to' => $address_to);

  drupal_add_js(array('delijn' => $settings), 'setting');
}

/**
 * Preprocess the agenda nearby actor search.
 */
function culturefeed_agenda_preprocess_culturefeed_agenda_actors_nearby(&$variables) {

  //Create link for each actor.
  $actors = array();

  foreach ($variables['items'] as $item) {
    $title = $item->getTitle(culturefeed_search_get_preferred_language());
    $id = $item->getId();
    $url = culturefeed_search_detail_url('actor', $id, $title);
    $actors[] = l($title, $GLOBALS['base_url'] . $url);
  }

  $variables['links'] = implode(', ', $actors);

}

/*
 * Theme an admin table for the possible filter options on the agenda search block.
 */
function theme_culturefeed_agenda_search_block_field_settings($variables) {

  $container = $variables['element'];

  $rows = array();
  foreach (element_children($container) as $key) {

   $container[$key]['weight']['#attributes']['class'] = array('enabled-fields-order-weight');

    $row = array();
    $row[] = array('data' => drupal_render($container[$key]['enabled']), 'width' => '20px');
    $row[] = drupal_render($container[$key]['title']);
    $row[] = drupal_render($container[$key]['weight']);

    $rows[] = array(
      'data' => $row,
      'class' => array('draggable'),
    );
  }

  $table_variables = array(
    'header' => array(
      t('Enabled'),
      t('Filter'),
      t('Weight'),
    ),
    'rows' => $rows,
    'attributes' => array('id' => 'enabled-fields'),
  );

  drupal_add_tabledrag('enabled-fields', 'order', 'sibling', 'enabled-fields-order-weight');

  return theme('table', $table_variables);

}

/**
 * Preprocess the agenda organiser events block.
 */
function culturefeed_agenda_preprocess_culturefeed_agenda_organiser_events(&$variables) {
	$items = array();
	foreach ($variables['items'] as $item) {
		/*$items[] = theme('culturefeed_agenda_organiser_events_item', array(
			'item' => $item,
		));*/
		$type = $item->getType();
		if ($type == 'event') {
			$type = 'e';
		}
		else if ($type == 'production') {
			$type = 'p';
		}

		$event = $item->getEntity();
		$event_detail = $event->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language());

		$title = $event_detail->getTitle();
		$cdbid = $event->getCdbid();

		$items[] = '<a href="/agenda/' . $type . '/slug/' . $cdbid . '">' . $title . '</a>';
	}
	$variables['items'] = $items;

	$organiser = $variables['organiser']->getLabel();
	$organiser_id = $variables['organiser']->getCdbid();
	$variables['organiser_link'] = '<a href="/agenda/a/' . $organiser . '/' . $organiser_id . '">' . $organiser . '</a>';


}
