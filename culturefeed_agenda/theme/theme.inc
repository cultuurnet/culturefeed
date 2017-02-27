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

  // Themes of this item (as plain text and as links).
  $themes = culturefeed_agenda_get_category_list_of_item(CultureFeed_Cdb_Data_Category::CATEGORY_TYPE_THEME, $cdb_item);
  $variables['themes'] = $themes['text'];
  $variables['themes_links'] = $themes['links'];

  // Eventtypes of this item (plain text and links).
  $eventtypes = culturefeed_agenda_get_category_list_of_item(CultureFeed_Cdb_Data_Category::CATEGORY_TYPE_EVENT_TYPE, $cdb_item);
  $variables['types'] = $eventtypes['text'];
  $variables['types_links'] = $eventtypes['links'];

  // Actortypes of this item (plain text and links).
  $actortypes = culturefeed_agenda_get_category_list_of_item(CultureFeed_Cdb_Data_Category::CATEGORY_TYPE_ACTOR_TYPE, $cdb_item);
  $variables['actortypes'] = $actortypes['text'];
  $variables['actortypes_links'] = $actortypes['links'];

  // Facilitytypes of this item (plain text and links).
  $facilities = culturefeed_agenda_get_category_list_of_item(CultureFeed_Cdb_Data_Category::CATEGORY_TYPE_FACILITY, $cdb_item);
  $enabled_facilities = variable_get('culturefeed_facet_enabled_options', NULL);
  $variables['facilities'] = array();
  if(!empty($facilities) && !empty($enabled_facilities['facility'])) {
    foreach($facilities['id'] as $key => $facility) {
      if(in_array($facility,$enabled_facilities['facility'])) {
        $variables['facilities'][] = $facilities['text'][$key];
      }
    }
  }

  // Vliegtypes of this item (plain text and links).
  $vliegtypes = culturefeed_agenda_get_category_list_of_item(CultureFeed_Cdb_Data_Category::CATEGORY_TYPE_UMV, $cdb_item);
  if (!empty($vliegtypes['text'])) {
    $variables['forkids'] = TRUE;
  }

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
      $variables['price'] = str_replace('.', ',', $variables['price']);
      $variables['price_description'] = $detail->getPrice()->getDescription();
    }

    // Ticket links.
    $ticket_links = $detail->getMedia()->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_RESERVATIONS);

    $variables['tickets'] = array();
    $variables['ticket_buttons'] = array();
    $variables['ticket_links'] = array();
    $cta_tickets = '';
    $cta_array = array(
      '0.3.1.0.0',
      '0.7.0.0.0',
      '0.57.0.0.0',
      '0.59.0.0.0',
      '0.17.0.0.0',
      '0.50.21.0.0',
      '0.3.2.0.0',
    );


    if (isset($eventtypes['id'][0]) && in_array($eventtypes['id'][0], $cta_array)) {
      $cta_tickets = t('Sign Up');
    }
    else {
      $cta_tickets = t('Buy tickets');
    }

    foreach ($ticket_links as $link) {
      $variables['tickets'][] = l($cta_tickets, $link->getHLink(), array('attributes' => array('class' => 'reservation-link', 'id' => 'cf-ticket')));

      // Provide some themable ticket buttons.
      $button = array();
      $button['text'] = $cta_tickets;
      $button['link'] = $link->getHLink();
      $button['options'] = array('attributes' => array('class' => 'reservation-link button', 'id' => 'cf-ticket'));
      $variables['ticket_buttons'][] = $button;
      $variables['ticket_links'][] = $link->getHLink();
    }

    // Performers
    if (method_exists($detail, 'getPerformers')) {
      $performers = $detail->getPerformers();
      $variables['performers'] = '';
      if ($performers) {
        $performer_labels = array();
        foreach ($performers as $performer) {
          $performer_label = check_plain($performer->getLabel());
          $performer_labels[] = $performer_label;
          $performer_role = check_plain($performer->getRole());
          $performer_roles[] = $performer_role;
          $performers_list[] = array('label' => $performer_label,'role' => $performer_role);
        };

        $variables['performers'] = implode(', ', $performer_labels);
        $variables['performers_list'] = $performers_list;
      }
    }

    // Pictures
    $main_picture = '';
    $main_picture_cr = '';
    $main_index = 0;
    $pictures = array();
    $copyrights = array();

    $photos = $detail->getMedia()->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_PHOTO);
    $i = 0;
    foreach ($photos as $photo) {
      $pictures[] = culturefeed_get_relative_image($photo->getHLink());
      $copyrights[] = $photo->getCopyright();
      if ($photo->isMain()) {
        $main_picture = culturefeed_get_relative_image($photo->getHLink());
        $main_picture_cr = $photo->getCopyright();
        $main_index = $i;
      }
      $i++;
    }

    // Support for mediatype imageweb
    $images = $detail->getMedia()->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_IMAGEWEB);
    foreach ($images as $image) {
      $pictures[] = culturefeed_get_relative_image($image->getHLink());
      if ($image->isMain() && empty($main_picture)) {
        $main_picture = culturefeed_get_relative_image($image->getHLink());
        $main_picture_cr = $image->getCopyright();
        $main_index = $i;
      }
      $i++;
    }

    // If no image we get image from eventdetails language=nl
    if (empty($pictures)) {

      $detail_nl = $cdb_item->getDetails()->getDetailByLanguage('nl');
      if (!empty($detail_nl)) {
        $photos = $detail_nl->getMedia()->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_PHOTO);
        foreach ($photos as $photo) {
          $pictures[] = culturefeed_get_relative_image($photo->getHLink());
          if ($photo->isMain()) {
            $main_picture = culturefeed_get_relative_image($photo->getHLink());
            $main_picture_cr = $photo->getCopyright();
            $main_index = $i;
          }
          $i++;
        }

        // Support for mediatype imageweb
        $images = $detail_nl->getMedia()->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_IMAGEWEB);
        foreach ($images as $image) {
          $pictures[] = $image->getHLink();
          if ($image->isMain() && empty($main_picture)) {
            $main_picture = culturefeed_get_relative_image($image->getHLink());
            $main_picture_cr = $image->getCopyright();
            $main_index = $i;
          }
          $i++;
        }
      }
    }

    // If no main picture is set we use first picture
    if (empty($main_picture) && count($pictures) > 0) {
      $main_picture = $pictures[0];
      if (isset($copyrights[0])) {
        $main_picture_cr = $copyrights[0];
      }
    }
    
    // check if same image isn't used twice or more
    $pictures = array_unique($pictures);

    // we don't want the main picture to show twice
    if (!empty($main_picture)) {
      unset($pictures[$main_index]);
      unset($copyrights[$main_index]);
    }

    $variables['main_picture'] = $main_picture;
    $variables['main_picture_cr'] = $main_picture_cr;
    $variables['pictures'] = $pictures;
    $variables['copyrights'] = $copyrights;

    // Make sure usage of thumbnail is backwards compatible
    $variables['thumbnail'] = $variables['main_picture'];

  }

  $variables['cdbid'] = $cdb_item->getCdbid();

  if (method_exists($cdb_item, 'getCreatedBy')) { // TODO: add method for productions (UIV-1761)
    $variables['created_by'] = $cdb_item->getCreatedBy();
  }

  // Social stats.
  $variables['recommend_count'] = $item->getActivityCount(\CultuurNet\Search\ActivityStatsExtendedEntity::ACTIVITY_COUNT_RECOMMEND);
  $variables['comment_count'] = $item->getActivityCount(\CultuurNet\Search\ActivityStatsExtendedEntity::ACTIVITY_COUNT_COMMENT);
  $variables['attend_count'] = $item->getActivityCount(\CultuurNet\Search\ActivityStatsExtendedEntity::ACTIVITY_COUNT_ATTEND);
  $variables['like_count'] = $item->getActivityCount(\CultuurNet\Search\ActivityStatsExtendedEntity::ACTIVITY_COUNT_LIKE);

  // Social actions.
  $variables['recommend_link'] = '';
  $variables['attend_link'] = '';
  if (culturefeed_agenda_social_links_preprocessing_enabled()) {
    $variables['recommend_link'] = culturefeed_social_activity_link(CultureFeed_Activity::TYPE_RECOMMEND, $item);
    $variables['attend_link'] = culturefeed_social_activity_link(CultureFeed_Activity::TYPE_IK_GA, $item);
  }

  // Themes of this item (as plain text and as links).
  $themes = culturefeed_agenda_get_category_list_of_item(CultureFeed_Cdb_Data_Category::CATEGORY_TYPE_THEME, $cdb_item);
  $variables['themes'] = $themes['text'];
  $variables['themes_links'] = $themes['links'];

  // Eventtypes of this item (plain text and links).
  $eventtypes = culturefeed_agenda_get_category_list_of_item(CultureFeed_Cdb_Data_Category::CATEGORY_TYPE_EVENT_TYPE, $cdb_item);
  $variables['types'] = $eventtypes['text'];
  $variables['types_links'] = $eventtypes['links'];

  // Actortypes of this item (plain text and links).
  $actortypes = culturefeed_agenda_get_category_list_of_item(CultureFeed_Cdb_Data_Category::CATEGORY_TYPE_ACTOR_TYPE, $cdb_item);
  $variables['actortypes'] = $actortypes['text'];
  $variables['actortypes_links'] = $actortypes['links'];

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

  $version = variable_get('culturefeed_search_cdb_version', CULTUREFEED_SEARCH_CDB_DEFAULT_VERSION);
  if (version_compare($version, '3.3', '>=')) {

    /* @var CultureFeed_Cdb_Data_Keyword[] $keywords */
    $keywords = $cdb_item->getKeywords(TRUE);
    $variables['keywords'] = '';

    if ($keywords) {

      $variables['raw_keywords'] = array();
      $linked_keywords = array();

      foreach ($keywords as $keyword) {

        if ($keyword->isVisible()) {
          $variables['raw_keywords'][] = $keyword->getValue();
          $linked_keywords[] = l($keyword->getValue(), 'agenda/search', array('query' => array('search' => '"' . $keyword->getValue() . '"')));
        }

        $variables['keywords'] = implode(', ', $linked_keywords);

      }

      // check if event is suitable for kids
      if (array_key_exists('ook voor kinderen', $keywords)) {
        $variables['forkids'] = TRUE;
      }

      foreach (array_keys($keywords) as $keyword) {
        if(stripos($keyword, 'uitpas')!==false || stripos($keyword, 'paspartoe')!==false)
        {
          $variables['uitpas'] = TRUE;
        }
      }

    }

  }
  else {

    $keywords = $cdb_item->getKeywords();
    $variables['keywords'] = '';

    if ($keywords) {
      $variables['raw_keywords'] = array();
      $linked_keywords = array();
      foreach ($keywords as $keyword) {
        $variables['raw_keywords'][] = $keyword;
        $linked_keywords[] = l($keyword, 'agenda/search', array('query' => array('search' => '"' . $keyword . '"')));
      }

      $variables['keywords'] = implode(', ', $linked_keywords);
      // check if event is suitable for kids
      if (array_key_exists('ook voor kinderen', $keywords)) {
        $variables['forkids'] = TRUE;
      }

      if (sizeof(preg_grep('/uitpas/i', $keywords))>0 or sizeof(preg_grep('/paspartoe/i', $keywords))>0) {
        $variables['uitpas'] = TRUE;
      }
    }
  }

  $variables['device'] = '(not set)';
  $variables['is_ios'] = FALSE;
  $variables['is_android'] = FALSE;

  if (module_exists('culturefeed')) {
    $detect = culturefeed_mobile_detect_get_object();
  }

  if ($detect) {
    $device = ($detect->isMobile() ? ($detect->isTablet() ? 'tablet' : 'phone') : 'computer');
    $variables['device'] = $device;

    $is_ios = $detect->isiOS();
    $variables['is_ios'] = $is_ios;

    $is_android = $detect->isAndroidOS();
    $variables['is_android'] = $is_android;
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
  $detail= $cdb_item->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language());

  // Facilities: only for events with a location
  if ($cdb_item instanceof CultureFeed_Cdb_Item_Event) {
    $actor_id = $cdb_item->getLocation()->getCdbid();
    if ($actor_id) {
      $actor = $cdb_item->getLocation()->getActor() ?: culturefeed_agenda_actor_load($actor_id)->getEntity();
      if ($actor) {
        $facilities = culturefeed_agenda_get_category_list_of_item(CultureFeed_Cdb_Data_Category::CATEGORY_TYPE_FACILITY, $actor);
        $variables['locations_facilities'] = $facilities['text'];
      }
    }
  }

  // Enhance event-details with UiTPAS-information if culturefeed_uitpas is enabled
  if (module_exists('culturefeed_uitpas')) {
    $uitpas_event = culturefeed_uitpas_get_uitpas_event($item);
    $variables['uitpas_info'] = $uitpas_event;
  }

  if ($detail) {

    // Add videos.
    $variables['videos'] = array();
    $variables['videos_id'] = array();

    $videos = $detail->getMedia()->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_VIDEO);
    foreach ($videos as $video) {
      if ($video_embed = _culturefeed_search_ui_video_url_to_embed($video->getHLink())) {
        $variables['videos'][] = $video_embed;
      }
      $variables['videos_id'][] = _culturefeed_search_ui_video_url_to_id($video->getHLink());
    }

    // Absolute link to detail.
    global $base_root;
    $variables['share_url'] = $base_root . $variables['url'];

    // Build share URL's.
    $variables['facebook']['url'] = culturefeed_share_link($variables['share_url'], 'facebook', $variables['title'], $variables['device']);
    $variables['googleplus']['url'] = culturefeed_share_link($variables['share_url'], 'googleplus', $variables['title'], $variables['device']);
    $variables['twitter']['url'] = culturefeed_share_link($variables['share_url'], 'twitter', $variables['title'], $variables['device']);
    $variables['whatsapp']['url'] = culturefeed_share_link($variables['share_url'], 'whatsapp', $variables['title'], $variables['device']);
    $variables['mail']['url'] = culturefeed_share_link($variables['share_url'], 'mail', $variables['title'], $variables['device']);

    $variables['facebook']['options'] = array();
    $variables['twitter']['options'] = array();
    $variables['googleplus']['options'] = array();
    $variables['whatsapp']['options'] = array();
    $variables['mail']['options'] = array();

    // Prepare to push share activities to Culturefeed.
    $track = FALSE;
    if (culturefeed_is_culturefeed_user()) {
      $track = TRUE;
      $content_type = culturefeed_get_content_type($item->getType());
      $id = $item->getId();
      $track_facebook = url('culturefeed/do/' . CultureFeed_Activity::TYPE_FACEBOOK .'/' . $content_type . '/' . urlencode($id) . '/ajax');
      $track_twitter = url('culturefeed/do/' . CultureFeed_Activity::TYPE_TWITTER .'/' . $content_type . '/' . urlencode($id) . '/ajax');
      $track_mail = url('culturefeed/do/' . CultureFeed_Activity::TYPE_MAIL .'/' . $content_type . '/' . urlencode($id) . '/ajax');

      // Add options.
      $variables['facebook']['options'] = array(
        'attributes' => array(
          'class' => array('share-link', 'facebook-share'),
          'rel' => $track ? $track_facebook : '' ,
        ),
      );
      $variables['twitter']['options'] = array(
        'attributes' => array(
          'class' => array('share-link', 'twitter-share'),
          'rel' => $track ? $track_twitter : '' ,
        ),
      );
      $variables['googleplus']['options'] = array(
        'attributes' => array(
          'class' => array('share-link', 'googleplus-share'),
        ),
      );
      $variables['whatsapp']['options'] = array(
        'attributes' => array(
          'class' => array('share-link', 'whatsapp-share'),
        ),
        'external' => TRUE,
      );
      $variables['mail']['options'] = array(
        'attributes' => array(
          'class' => array('share-link', 'forward-mail'),
          'rel' => $track ? $track_mail : '' ,
        ),
        'external' => TRUE,
      );

    }

    // Share Links.
    $facebook_title = t('Share on Facebook');
    $facebook_options = $variables['facebook']['options'];
    $facebook_options['attributes']['title'] = $facebook_title;
    $facebook_options['attributes']['alt'] = $facebook_title;
    $variables['facebook_link'] = l($facebook_title, $variables['facebook']['url'], $facebook_options);

    $googleplus_title = t('Share on Google+');
    $googleplus_options = $variables['googleplus']['options'];
    $googleplus_options['attributes']['title'] = $googleplus_title;
    $googleplus_options['attributes']['alt'] = $googleplus_title;
    $variables['googleplus_link'] = l($googleplus_title, $variables['googleplus']['url'], $googleplus_options);

    $twitter_title = t('Share on Twitter');
    $twitter_options = $variables['twitter']['options'];
    $twitter_options['attributes']['title'] = $twitter_title;
    $twitter_options['attributes']['alt'] = $twitter_title;
    $variables['twitter_link'] = l($twitter_title, $variables['twitter']['url'], $twitter_options);

    $whatsapp_title = t('Share on Whatsapp');
    $whatsapp_options = $variables['whatsapp']['options'];
    $whatsapp_options['attributes']['title'] = $whatsapp_title;
    $whatsapp_options['attributes']['alt'] = $whatsapp_title;
    $variables['whatsapp_link'] = l($whatsapp_title, $variables['whatsapp']['url'], $whatsapp_options);

    $mail_title = t('Mail');
    $mail_options = $variables['mail']['options'];
    $mail_options['attributes']['title'] = $mail_title;
    $mail_options['attributes']['alt'] = $mail_title;
    $variables['mail_link'] = l($mail_title, $variables['mail']['url'], $mail_options);

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
  $reservation_links = array();
  if (method_exists($cdb_item, 'getContactInfo')) {
    try {
      $contact_info = $cdb_item->getContactInfo();
      if ($contact_info) {
        $reservation_info = $cdb_item->getContactInfo()->getReservationInfo();
        $reservation_links = isset($reservation_info['url']) ? $reservation_info['url'] : $reservation_links;
      }
    }
    catch (Exception $e) {
      watchdog_exception('notice', $e);
    }
  }
  foreach ($links as $link) {

    if (!in_array($link, $reservation_links)) {

      // clean up link text
      $link_text = str_replace('http://', '', $link);
      $link_text = str_replace('https://', '', $link_text);
      $variables['links'][] = l(truncate_utf8($link_text, 40, FALSE, TRUE), trim($link), array('attributes' => array('id' => 'cf-link')));

    }

  }

  if (culturefeed_agenda_social_links_preprocessing_enabled()) {
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
  if (is_numeric($event->getAgeFrom()) && ($event->getAgeFrom() < 12)) {
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
      if (!$mail->isForReservations()) {
        $mails[] = l($mail->getMailAddress(), 'mailto:' . $mail->getMailAddress(), array('attributes' => array('id' => 'cf-contact')));
      }
    }
    $variables['contact']['mail'] = implode(', ', $mails);

    // Fax + phone numbers.
    $phones = array();
    $faxes = array();
    foreach ($contact_info->getPhones() as $phone) {
      if (!$phone->isForReservations()) {
        if ($phone->getType() == CultureFeed_Cdb_Data_Phone::PHONE_TYPE_FAX) {
          $faxes[] = $phone->getNumber();
        }
        else {
          if (isset($variables['device']) && $variables['device'] == 'phone') {
            $phones[] = l($phone->getNumber(), 'tel:' . culturefeed_valid_phone($phone->getNumber()));
          }
          else {
            $phones[] = $phone->getNumber();
          }
        }
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
          if(isset($variables['device']) && $variables['device'] == 'phone')
          {
            $reservation_phones[] = l($phone, 'tel:'.culturefeed_valid_phone($phone));
          }
          else {
            $reservation_phones[] = $phone;
          }
        }
        $variables['reservation']['phone'] = implode(', ', $reservation_phones);
      }

      // Reservation url
      if (isset($reservation_info['url'])) {
        foreach ($reservation_info['url'] as $url) {
          if(isset($variables['ticket_links']) && !in_array($url, $variables['ticket_links'])){
            // clean up link text
            $url_text = str_replace('http://', '', $url);
            $url_text = str_replace('https://', '', $url_text);
            $reservation_urls[] = l(truncate_utf8($url_text, 40, FALSE, TRUE), $url, array('attributes' => array('id' => 'cf-reservation')));
          }
        }
        if (isset($reservation_urls)) {
          $variables['reservation']['url'] = implode(', ', $reservation_urls);
        }
        else {
          $variables['reservation']['url'] = implode(', ', $reservation_info['url']);
        }
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

    // Format Calendar details to readable & themable HTML.
    $html_formatter = culturefeed_agenda_get_html_calendar_formatter();

    // Large Calendar format (for detail pages).
    $variables['when_lg'] = '';
    try {
      $variables['when_lg'] = $html_formatter->format($calendar, 'lg');
    }
    catch (Exception $e) {
    }

    // Medium Calendar format (for event-summary on search result pages).
    $variables['when_md'] = '';
    try {
      $variables['when_md'] = $html_formatter->format($calendar, 'md');
    }
    catch (Exception $e) {
    }

    // Small Calendar format (for mini-summary in event teasers).
    $variables['when_sm'] = '';
    try {
      $variables['when_sm'] = $html_formatter->format($calendar, 'sm');
    }
    catch (Exception $e) {
    }

    // Extra Small Calendar format (for teasers that need specific "DD/MM" layout).
    $variables['when_xs'] = '';
    try {
      $variables['when_xs'] = $html_formatter->format($calendar, 'xs');
    }
    catch (Exception $e) {
    }

    // Format calendar to TXT
    $txt_formatter = culturefeed_agenda_get_plain_text_calendar_formatter();

    // Large Calendar format (for detail pages).
    $variables['when_lg_txt'] = '';
    try {
      $variables['when_lg_txt'] = $txt_formatter->format($calendar, 'lg');
    }
    catch (Exception $e) {
    }

    // Medium Calendar format (for event-summary on search result pages).
    $variables['when_md_txt'] = '';
    try {
      $variables['when_md_txt'] = $txt_formatter->format($calendar, 'md');
    }
    catch (Exception $e) {
    }

    // Small Calendar format (for mini-summary in event teasers).
    $variables['when_sm_txt'] = '';
    try {
      $variables['when_sm_txt'] = $txt_formatter->format($calendar, 'sm');
    }
    catch (Exception $e) {
    }

    // Extra Small Calendar format (for teasers that need specific "DD/MM" layout).
    $variables['when_xs_txt'] = '';
    try {
      $variables['when_xs_txt'] = $txt_formatter->format($calendar, 'xs');
    }
    catch (Exception $e) {
    }

    // Check if event has finished or not.
    $event_is_passed = FALSE;
    if ($event instanceof CultureFeed_Cdb_Item_Event) {
      $objDateTime = new DateTime('NOW');
      $calendar = $event->getCalendar();
      if ($calendar instanceof CultureFeed_Cdb_Data_Calendar_TimestampList || $calendar instanceof CultureFeed_Cdb_Data_Calendar_PeriodList) {

        // Check if 1 of the items is still in the future.
        $event_is_passed = TRUE;
        foreach ($calendar as $calendar_item) {
          if ($calendar_item instanceof CultureFeed_Cdb_Data_Calendar_Period && DateTime::createFromFormat('Y-m-d', $calendar_item->getDateTo())->getTimestamp() >= $objDateTime->getTimestamp()) {
            $event_is_passed = FALSE;
            break;
          }
          elseif ($calendar_item instanceof CultureFeed_Cdb_Data_Calendar_Timestamp && DateTime::createFromFormat('Y-m-d', $calendar_item->getDate())->getTimestamp() >= $objDateTime->getTimestamp()) {
            $event_is_passed = FALSE;
            break;
          }
        }

      }
    }
    $variables['event_is_passed'] = $event_is_passed;

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

          // Calendar.
          if ($calendar = $event->getEntity()->getCalendar()) {
            $variables['calendar'] = culturefeed_agenda_parse_calendar($calendar);
          }

          $variables['when'] = $event_detail->getCalendarSummary();

          // Format Calendar details to readable & themable HTML.
          $html_formatter = culturefeed_agenda_get_html_calendar_formatter();

          // Large Calendar format (for detail pages).
          $variables['when_lg'] = '';
          try {
            $variables['when_lg'] = $html_formatter->format($calendar, 'lg');
          }
          catch (Exception $e) {
          }

          // Medium Calendar format (for search results).
          $variables['when_md'] = '';
          try {
            $variables['when_md'] = $html_formatter->format($calendar, 'md');
          }
          catch (Exception $e) {
          }
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

  if ($actor->getWeekScheme()) {

    $variables['weekscheme'] = culturefeed_agenda_parse_weekscheme($actor->getWeekScheme());

    // Format Calendar details to readable & themable HTML.
    $html_formatter = culturefeed_agenda_get_html_calendar_formatter();

    // Use Permanent Calendar because html_formatter uses CultureFeed_Cdb_Data_Calendar and not Weekscheme
    $calendar = new CultureFeed_Cdb_Data_Calendar_Permanent();
    $calendar->setWeekScheme($actor->getWeekScheme());

    // Large Calendar format (for detail pages).
    $variables['when_lg'] = '';
    try {
      $variables['when_lg'] = $html_formatter->format($calendar, 'lg');
    }
    catch (Exception $e) {
    }

  }

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
      if ($phone->getType() == CultureFeed_Cdb_Data_Phone::PHONE_TYPE_FAX) {
        $faxes[] = $phone->getNumber();
      }
      else {
        if (isset($variables['device']) && $variables['device'] == 'phone') {
          $phones[] = l($phone->getNumber(), 'tel:' . culturefeed_valid_phone($phone->getNumber()));
        }
        else {
          $phones[] = $phone->getNumber();
        }
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

  switch ($variables['item']->getType()) {

    case 'event':
      culturefeed_agenda_preprocess_culturefeed_event_summary($variables);
      break;

    case 'production':
      culturefeed_agenda_preprocess_culturefeed_production_summary($variables);
      break;
  }

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
  $og_tags = array();

  if (!empty($variables['title'])) {
    $title_parts[] = $variables['title'];
    $og_tags['og:title'] = decode_entities(strip_tags($variables['title']));
  }
  if (!empty($variables['location']) && !empty($variables['location']['city'])) {
    $title_parts[] = $variables['location']['city'];
    $description_parts[] = t('Location: @location', array('@location' => $variables['location']['title'] . ', ' . $variables['location']['city']));
  }
  if (!empty($variables['when_md_txt'])) {
    $description_parts[] = t('When: @when', array('@when' => $variables['when_md_txt']));
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

  // Add open graph meta tags.
  $fb_app_id = variable_get('culturefeed_search_ui_fb_app_id', '');
  if (!empty($fb_app_id)) {
    $og_tags['fb:app_id'] = $fb_app_id;
  }
  $og_tags['og:site_name'] = variable_get('site_name', '');
  $og_tags['og:url'] = url($variables['url'], array('absolute' => TRUE));
  $og_tags['og:description'] = implode(' - ', $description_parts);
  if ($variables['main_picture']) {
    $image = str_replace(' ', '%20', $variables['main_picture']);
    $colonpos = strpos($image, ':');
    if ($colonpos) {
      $image = substr($image, $colonpos + 1);
    }
    $image_path = 'http:' . $image;
    $image_secure_path = 'https:' . $image;
    $og_tags['og:image'] = $image_path . '?width=200&height=200&crop=auto&scale=both';
    $og_tags['og:image:secure_url'] = $image_secure_path . '?width=200&height=200&crop=auto&scale=both';
  }
  else {
    $path = drupal_get_path('module', 'culturefeed_agenda');
    $og_tags['og:image'] = url($path . '/images/no-thumbnail.png', array('absolute' => TRUE));
  }
  $og_tags['og:image:width'] = '200';
  $og_tags['og:image:height'] = '200';

  foreach ($og_tags as $property => $content) {
    drupal_add_html_head(array(
      '#tag' => 'meta',
      '#attributes' => array(
        'property' => $property,
        'content' => $content,
      ),
    ), $property);
  }

}

/**
 * Helper function to create a meta description tag for actors and pages.
 * @param type $variables
 */
function _culturefeed_agenda_preprocess_actor_page_metatags($variables) {

  $title_parts = array();
  $description_parts = array();
  $og_tags = array();

  if (!empty($variables['title'])) {
    $title_parts[] = $variables['title'];
    $og_tags['og:title'] = html_entity_decode($variables['title']);
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

  // Add open graph meta tags.
  $fb_app_id = variable_get('culturefeed_search_ui_fb_app_id', '');
  if (!empty($fb_app_id)) {
    $og_tags['fb:app_id'] = $fb_app_id;
  }
  $og_tags['og:site_name'] = variable_get('site_name', '');
  $og_tags['og:url'] = url($variables['url'], array('absolute' => TRUE));
  $og_tags['og:description'] = implode(' - ', $description_parts);
  if ($variables['main_picture']) {
    $colonpos = strpos($variables['main_picture'], ':');
    $path = ($colonpos == FALSE) ? 'http:' : '' ;
    $path .= str_replace(' ', '%20', $variables['main_picture']);
    $og_tags['og:image'] = $path . '?width=200&height=200&crop=auto&scale=both';
  }
  else {
    $path = drupal_get_path('module', 'culturefeed_agenda');
    $og_tags['og:image'] = url($path . '/images/no-thumbnail.png', array('absolute' => TRUE));
  }

  foreach ($og_tags as $property => $content) {
    drupal_add_html_head(array(
      '#tag' => 'meta',
      '#attributes' => array(
        'property' => $property,
        'content' => $content,
      ),
    ), $property);
  }
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

  $variables['links'] = $actors;

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

function theme_culturefeed_agenda_page_list_rss($variables) {
  $objects = $variables['objects'];
  $active_filters = $variables['path']['title'];

  $items = '';
  if ($objects) {
    foreach ($objects as $object) {
      // object_url
      $object_url = array('path' => $object['path']);
      $object_url['options']['absolute'] = TRUE;
      $object_url['options']['query'] = array('utm_campaign' => 'rss', 'utm_medium' => 'content', 'utm_source' => 'external', 'utm_content' => 'feed');
      $object_url = url($object_url['path'], $object_url['options']);

      $args = array(
        array('key' => 'pubDate', 'value' => date('r', strtotime($object['created']))),
        array('key' => 'guid', 'value' => $object_url, 'attributes' => array('isPermaLink' => 'true')),
        array('key' => 'postid', 'value' => $object['cdbid']),
      );

      $body = '';
      if (isset($object['city']) && !empty($object['city'])) {
        $body .= strtoupper($object['city']) . ', ';
      }
      if (isset($object['when_md_txt']) && !empty($object['when_md_txt'])) {
        $body .= strtolower($object['when_md_txt']) . ' - ';
      }
      $body .= $object['shortdescription'];
      $items .= format_rss_item($object['title'], $object_url, $body, $args);
    }
  }

  $site_name = variable_get('site_name', 'Drupal');
  $url = url('', array('absolute' => TRUE, 'query' => array('utm_campaign' => 'rss', 'utm_medium' => 'content', 'utm_source' => 'external', 'utm_content' => 'feed')));
  $description = t('Recently added in:') . ' ' . $active_filters ;

  $output  = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  $output .= "<rss version=\"2.0\">\n";
  $output .= format_rss_channel(t('@site_name', array('@site_name' => $site_name)), $url, $description, $items);
  $output .= "</rss>\n";

  return $output;
}
