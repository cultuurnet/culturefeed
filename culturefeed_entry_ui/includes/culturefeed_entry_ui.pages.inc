<?php
/**
 * @file
 * Page callbacks for the entry api ui.
 */

/**
 * Form callback: Create a new event, or edit an existing.
 */
function culturefeed_entry_ui_event_form($form, $form_state, $event = NULL) {

  $form = array();
  $default_contacts = array();
  $performers_info = array();
  $category_defaults = array();
  $eventtype_default = '';
  $default_location_label = '';
  $default_location_id = '';
  $default_organiser_label = '';
  $default_organiser_id = '';
  $publication_date = '';
  $when = 'timestamps';
  $timestamps = array();
  $period_start = '';
  $period_end = '';
  $weekscheme = NULL;
  $day_labels = array(
    'monday' => t('mon'),
    'tuesday' => t('tue'),
    'wednesday' => t('wed'),
    'thursday' => t('thu'),
    'friday' => t('fri'),
    'saturday' => t('sat'),
    'sunday' => t('sun'),
  );
  $age = '';
  $age_category = 'everyone';
  $default_short_description = '';
  $default_long_description = '';
  $default_image = '';
  $keywords = '';
  $default_links = array();
  $default_links_reservation = array();
  $phone_contacts = array();
  $mail_contacts = array();
  $is_private = FALSE;

  if (!isset($form_state['#location_needed'])) {
    $form_state['#location_needed'] = FALSE;
  }
  $form_state['#zip_needed'] = FALSE;

  // Language vars
  $nl_title = '';
  $nl_short_description = '';
  $nl_long_description = '';

  $fr_title = '';
  $fr_short_description = '';
  $fr_long_description = '';

  $en_title = '';
  $en_short_description = '';
  $en_long_description = '';

  $de_title = '';
  $de_short_description = '';
  $de_long_description = '';

  // Load all default values.
  if ($event) {
    /** @var \CultuurNet\Search\ActivityStatsExtendedEntity $event */
    $event = $event->getEntity();
    /** @var CultureFeed_Cdb_Item_Event $event */
    $form['#event'] = $event;
    $event_details = $event->getDetails()->current();

    // Default categories.
    if ($event && ($categories = $event->getCategories())) {

      /** @var CultureFeed_Cdb_Data_Category $category */
      foreach ($categories as $category) {
        $category_defaults[] = $category->getId();
        if ($category->getType() == 'eventtype') {
          $eventtype_default = $category->getId();
        }

      }
    }

    // Default location.
    if ($location = $event->getLocation()) {
      $address = $location->getAddress()->getPhysicalAddress();
      if ($location->getCdbId()) {
        $default_location_id = $location->getCdbId();
        $default_location_label = $location->getLabel();
        $form_state['#location_needed'] = FALSE;
      }
      else {
        $form_state['#location_needed'] = TRUE;
        $custom_location_title = $location->getLabel();
        $custom_location_street = $address->getStreet();
        $custom_location_number = $address->getHouseNumber();
        $custom_location_zip = $address->getZip();
        $custom_location_city = $address->getCity();
        $custom_location_country = $address->getCountry();
      }
    }

    // Default organiser.
    if ($organiser = $event->getOrganiser()) {
      $default_organiser_label = $organiser->getLabel();
      $default_organiser_id = $location->getCdbId();
    }

    // Default calendar type, start / end date, weekscheme data.
    if ($calendar = $event->getCalendar()) {

      $calendar_class = get_class($calendar);
      $calendar->rewind();
      
      if ($calendar_class == 'CultureFeed_Cdb_Data_Calendar_PeriodList') {
        $when = 'period';
        $period_start = $calendar->current()->getDateFrom();
        $period_end = $calendar->current()->getDateTo();
        $weekscheme = $calendar->current()->getWeekScheme();
      }
      elseif ($calendar_class == 'CultureFeed_Cdb_Data_Calendar_Permanent') {
        $when = 'permanent';
        $weekscheme = $calendar->getWeekScheme();
      }
      elseif ($calendar_class == 'CultureFeed_Cdb_Data_Calendar_TimestampList') {

        $when = 'timestamps';
        $timestamps_count = iterator_count($calendar);
        $calendar->rewind();

        for ($i = 0; $i < $timestamps_count; $i++) {

          $timestamp = $calendar->current();

          $timestamps[$i] = array(
            'date' => $timestamp->getDate(),
            'begintime' => $timestamp->getStartTime(),
            'endtime' => $timestamp->getEndTime(),
          );

          $calendar->next();

        }
      }
    }
    
    // Set the total opening times needed. This need to be equal for every day.
    if (!isset($form_state['#opening_times_needed'])) {
      $opening_times_needed = 1;
      foreach (CultureFeed_Cdb_Data_Calendar_SchemeDay::$allowedDays as $day) {
        /** @var CultureFeed_Cdb_Data_Calendar_Weekscheme $weekscheme */
        if ($weekscheme && $data = $weekscheme->getDay($day)) {
          $opening_times = $data->getOpeningTimes();
          if (count($opening_times) > $opening_times_needed) {
            $opening_times_needed = count($opening_times);
          }
        }
      }
      $form_state['#opening_times_needed'] = $opening_times_needed;
    }

    // Default short description
    if ($event_details->getShortDescription()) {
      $default_short_description = $event_details->getShortDescription();
    }

    // Default long description
    if ($event_details->getLongDescription()) {
      $default_long_description = $event_details->getLongDescription();
      $default_long_description = substr($default_long_description, 0, -169);
      $form_state['#ld_needed'] = TRUE;
    }

    // Default keywords
    if ($event->getKeywords()) {
      $keywords = $event->getKeywords();
      $keywords = implode(';', $keywords);
    }

    // Default mails, phones, urls
    if ($info = $event->getContactInfo()) {
      foreach ($info->getUrls() as $default_link) {
        $default_links[$default_link->getUrl()] = $default_link->getUrl();
        $default_links_reservation[$default_link->getUrl()] = $default_link->isForReservations();
      }
    }
    
    // Add links form media
    $media_links = 
      $event_details->getMedia()
      ->byMediaTypes(
      array(
        CultureFeed_Cdb_Data_File::MEDIA_TYPE_WEBRESOURCE,
        CultureFeed_Cdb_Data_File::MEDIA_TYPE_RESERVATIONS,
        CultureFeed_Cdb_Data_File::MEDIA_TYPE_FACEBOOK,
        CultureFeed_Cdb_Data_File::MEDIA_TYPE_YOUTUBE,
        CultureFeed_Cdb_Data_File::MEDIA_TYPE_GOOGLEPLUS,
        CultureFeed_Cdb_Data_File::MEDIA_TYPE_TWITTER,
      )
    );
    
    foreach ($media_links as $media_link) {
      $default_links[$media_link->getHLink()] = $media_link->getHLink();
      if ($media_link->getMediaType() == 'reservations') {
        $default_links_reservation[$media_link->getHLink()] = TRUE;
      }
      else {
        $default_links_reservation[$media_link->getHLink()] = FALSE;
      }
    }
      


    // Default image
    $photo_media = $event_details->getMedia()
      ->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_IMAGEWEB);
    if(!$photo_media) {
      $photo_media = $event_details->getMedia()
      ->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_PHOTO);
    }
 
    if (count($photo_media) > 0) {
      $main_image = array();
      foreach ($photo_media as $picture) {
        $pictures[] = $picture;
        if ($picture->isMain()) {
          $main_image = $picture;
        }
      }
      // If no main use first image
      if (empty($main_image)) {
        $main_image = $pictures[0];
      }

      try {
        $default_image = system_retrieve_file($main_image->getHLink(), 'public://uploads/' . $main_image->getFileName(), TRUE);
        if ($default_image) {
          file_usage_add($default_image, 'culturefeed_entry_ui', 'event', $default_image->fid);
          $default_image_hlink = $main_image->getHLink();
        }
      } catch (Exception $e) {
        watchdog_exception('culturefeed_entry_ui', $e);
      }

    }

    // Default price
    if ($info = $event_details->getPrice()) {
      $default_price = $info->getValue();
      $default_free_price = 0;
      $default_price_description = $info->getDescription();
      if ($default_price == '0.00') {
        $default_price = '0';
        $default_free_price = 1;
      }
      if ($default_price_description != '') {
        $form_state['#price_needed'] = TRUE;
      }
    }

    // Default age
    $age = $event->getAgeFrom();
    if ($age) {

      if ($age < 12) {
        $age_category = '1-12';
      }
      elseif ($age < 18) {
        $age_category = '12-18';
      }
      else {
        $age = '18+';
      }

    }

    // Default contact info
    if ($info = $event->getContactInfo()) {
      $phone_info = $info->getPhones();
      $mail_info = $info->getMails();
      foreach ($phone_info as $phone_contact) {
        $phone_contacts[] = array('phone', $phone_contact->getNumber());
      }

      foreach ($mail_info as $mail_contact) {
        $mail_contacts[] = array('E-mail', $mail_contact->getMailAddress());
      }
      $default_contacts = array_merge($phone_contacts, $mail_contacts);
    }

    // Default performers
    if ($info = $event_details->getPerformers()) {
      foreach ($info as $performer) {
        $performers_info[] = array(
          $performer->getRole(),
          $performer->getLabel()
        );
      }
    }

    // Default target group
    if ($info = $event->isPrivate()) {
      $is_private = $event->isPrivate();
    }

    // Default languages
    if ($fr_info = $event->getDetails()->getDetailByLanguage('fr')) {
      $fr_title = $fr_info->getTitle();
      $fr_short_description = $fr_info->getShortDescription();
      $fr_long_description = $fr_info->getLongDescription();
    }

    if ($en_info = $event->getDetails()->getDetailByLanguage('en')) {
      $en_title = $en_info->getTitle();
      $en_short_description = $en_info->getShortDescription();
      $en_long_description = $en_info->getLongDescription();
    }

    if ($de_info = $event->getDetails()->getDetailByLanguage('de')) {
      $de_title = $de_info->getTitle();
      $de_short_description = $de_info->getShortDescription();
      $de_long_description = $de_info->getLongDescription();
    }

    // Default publication
    /*if ($info = $event->getAvailableFrom()) {
	  $publication_date = $event->getAvailableFrom();
    }
    
    var_dump($publication_date);*/

  }

  // Build the form.

  // Basic.
  $form['basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic info'),
  );

  $form['basic']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => ($event) ? $event->getDetails()
        ->getDetailByLanguage(culturefeed_search_get_preferred_language())
        ->getTitle() : NULL,
    '#required' => TRUE,
  );

  // Eventtypes
  $form['basic']['what'] = array('#type' => 'container');

  $types = array_merge(
    array(CULTUREFEED_ENTRY_UI_EVENTTYPE_CAT_ID_KIJKEN_EN_LUISTEREN => culturefeed_search_get_eventtype_categories(array('parent' => CULTUREFEED_ENTRY_UI_EVENTTYPE_CAT_ID_KIJKEN_EN_LUISTEREN))),
    array(CULTUREFEED_ENTRY_UI_EVENTTYPE_CAT_ID_DOEN => culturefeed_search_get_eventtype_categories(array('parent' => CULTUREFEED_ENTRY_UI_EVENTTYPE_CAT_ID_DOEN))),
    array(CULTUREFEED_ENTRY_UI_EVENTTYPE_CAT_ID_BEZOEKEN => culturefeed_search_get_eventtype_categories(array('parent' => CULTUREFEED_ENTRY_UI_EVENTTYPE_CAT_ID_BEZOEKEN)))
  );

  foreach ($types as $cluster_id => $cluster_value) {

    $cluster_name = culturefeed_search_get_eventtype_categories(array('tid' => $cluster_id));
    $form['basic']['what'][$cluster_id] = array(
      '#type' => 'fieldset',
      '#title' => $cluster_name[$cluster_id],
    );

    foreach ($cluster_value as $type_id => $type_name) {
      $form['basic']['what'][$cluster_id][$type_name] = array(
        '#type' => 'radio',
        '#title' => $type_name,
        '#return_value' => $type_id,
        '#default_value' => $eventtype_default,
        '#parents' => array('what'),
      );
      
      //$themas = culturefeed_search_get_themetype_categories(array('tid' => $type_id));
      //dsm($themas);
    }
  }
  

  $form['basic']['location'] = array(
    '#prefix' => '<div id="location-ajax-wrapper">',
    '#suffix' => '</div>',
    '#type' => 'container',
    '#tree' => TRUE,
  );

  if (!$form_state['#location_needed']) {
    $form['basic']['location']['actor'] = array(
      '#tree' => TRUE,
      '#type' => 'container',
      '#prefix' => '<div id="known-location-wrapper">',
      '#suffix' => '</div>',
    );

    $form['basic']['location']['actor']['location_actor_label'] = array(
      '#type' => 'textfield',
      '#title' => t('Location'),
      '#autocomplete_path' => 'autocomplete/culturefeed/actor-suggestions',
      '#required' => FALSE,
      '#default_value' => isset($default_location_label) ? $default_location_label : '',
      '#size' => 80,
    );

    $form['basic']['location']['actor']['location_actor_id'] = array(
      '#id' => 'location_actor_id',
      '#type' => 'hidden',
      '#default_value' => isset($default_location_id) ? $default_location_id : '',
      '#ajax' => array(
        'callback' => 'culturefeed_entry_ui_add_more_js',
        'wrapper' => 'location-ajax-wrapper',
        'event' => 'location_eventsearch',
      ),
      '#attributes' => array(
        'id' => 'location_actor_id',
      ),
    );
  }
  
  $form['basic']['location']['new'] = array(
    '#tree' => TRUE,
    '#type' => 'container',
    '#prefix' => '<div id="new-location-wrapper">',
    '#suffix' => '</div>',
  );
    
  if (!$form_state['#location_needed']) {  
    $form['basic']['location']['new']['add_new_location'] = array(
      '#type' => 'submit',
      '#name' => 'location_add_more',
      '#value' => t('Location not found, add location'),
      '#attributes' => array(
        'class' => array('add-more-link btn-link'),
        'style' => array('display:none'),
      ),
      '#ajax' => array(
        'callback' => 'culturefeed_entry_ui_add_more_js',
        'wrapper' => 'location-ajax-wrapper',
        'effect' => 'fade',
      ),
      '#limit_validation_errors' => array(array('location')),
      '#submit' => array('culturefeed_entry_ui_add_more_submit'),
    );
  }

  if ($form_state['#location_needed']) {
    $form['basic']['location']['new']['location_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Location Title'),
      '#default_value' => isset($custom_location_title) ? $custom_location_title : '',
      '#required' => FALSE,
      '#size' => 80,
    );

    $form['basic']['location']['new']['street'] = array(
      '#type' => 'textfield',
      '#title' => t('Street'),
      '#default_value' => isset($custom_location_street) ? $custom_location_street : '',
      '#required' => FALSE,
      '#size' => 60,
    );

    $form['basic']['location']['new']['house_number'] = array(
      '#type' => 'textfield',
      '#title' => t('House Number'),
      '#default_value' => isset($custom_location_number) ? $custom_location_number : '',
      '#required' => FALSE,
      '#size' => 10,
    );

    $form['basic']['location']['new']['citywrapper'] = array(
      '#tree' => TRUE,
      '#type' => 'container',
      '#prefix' => '<div id="citywrapper-ajax-wrapper">',
      '#suffix' => '</div>',
    );

    $form['basic']['location']['new']['citywrapper']['zip_code'] = array(
      '#type' => 'textfield',
      '#title' => '',
      '#name' => 'zip_code',
      '#default_value' => isset($custom_location_zip) ? $custom_location_zip : '',
      '#required' => FALSE,
      '#size' => 10,
      '#attributes' => array(
        'style' => array(
          'display:none;'
        ),
      ),
    );

    $form['basic']['location']['new']['citywrapper']['city'] = array(
      '#type' => 'textfield',
      '#title' => t('City'),
      '#default_value' => isset($custom_location_city) ? $custom_location_city : '',
      '#autocomplete_path' => 'autocomplete/culturefeed/city-suggestion',
      '#required' => FALSE,
      '#size' => 60,
    );

    $form['basic']['location']['new']['citywrapper']['country'] = array(
      '#type' => 'select',
      '#name' => 'country_select',
      '#title' => t('Country'),
      '#options' => country_get_list(),
      '#default_value' => isset($custom_location_country) ? $custom_location_country : 'BE',
      '#required' => FALSE,
      '#ajax' => array(
        'callback' => 'culturefeed_entry_ui_add_more_js',
        'wrapper' => 'citywrapper-ajax-wrapper',
        'effect' => 'fade',
      ),
    );
  }

  // Date.
  $form['basic']['when'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="when-ajax-wrapper">',
    '#suffix' => '</div>',
    '#title' => t('Dates'),
  );

  $form['basic']['when']['datetimes'] = array(
    '#tree' => TRUE,
    '#type' => 'container',
    '#attributes' => array('class' => array('when-datetimes')),
  );

  $date_count = (isset($form_state['#dates_needed'])) ? $form_state['#dates_needed'] : (count($timestamps) ? count($timestamps) : 1);

  $triggering_element = isset($form_state['input']['_triggering_element_name']) ? $form_state['input']['_triggering_element_name'] : '';

  // $when can change from datetime to period and vice versa.

  // Is it a timestamp ?
  if (
    ($date_count > 1) ||
    (isset($form_state['values']['datetimes'][0]['begin']['date']) &&
    (!isset($form_state['values']['datetimes'][0]['end']['date']) || $triggering_element == 'datetimes_remove_end_' . 0)) ||
    (isset($form_state['values']['permanent']) && !$form_state['values']['permanent']) ||
    (isset($form_state['values']['datetimes'][0]['begin']))
  ) {
    $when = 'timestamps';
  }
  // Is it a period ?
  if (
    (isset($form_state['values']['datetimes'][0]['begin']['date']) && isset($form_state['values']['datetimes'][0]['end']['date']) &&
    $form_state['values']['datetimes'][0]['begin']['date'] != $form_state['values']['datetimes'][0]['end']['date'] &&
    $triggering_element != 'datetimes_remove_end_' . 0)
  ) {
    $when = 'period';
  }
  // Is it permanent
  if (isset($form_state['values']['permanent']) && $form_state['values']['permanent']) {
    $when = 'permanent';
  }

  $form['basic']['when']['datetype'] = array(
    '#type' => 'hidden',
    '#value' => $when,
  );

  if ($when != 'permanent') {

    for ($i = 0; $i < $date_count; $i++) {

      // Timestamps / Period wrapper.
      $form['basic']['when']['datetimes'][$i] = array(
        '#type' => 'container',
        '#prefix' => '<div id="datetime-child-' . $i . '-ajax-wrapper" class="datetime-child clearfix">',
        '#suffix' => '</div>',
      );

      // Timestamps / Period begin wrapper.
      $form['basic']['when']['datetimes'][$i]['begin'] = array(
        '#type' => 'container',
        '#prefix' => '<div id="datetime-child-' . $i . '-begin-ajax-wrapper">',
        '#suffix' => '</div>',
      );

      // Remove timestamp.
      if ($date_count > 1) {

        $form['basic']['when']['datetimes'][$i]['begin']['datetimes_remove_child'] = array(
          '#type' => 'submit',
          '#value' => t('X'),
          '#name' => 'datetimes_remove_child_' . $i,
          '#attributes' => array('class' => array('add-more-link btn-link')),
          '#ajax' => array(
            'callback' => 'culturefeed_entry_ui_add_more_js',
            'wrapper' => 'when-ajax-wrapper',
            'effect' => 'fade',
          ),
          '#limit_validation_errors' => array(array('datetimes')),
          '#submit' => array('culturefeed_entry_ui_add_more_submit'),
        );

      }

      // Timestamp / Period begin date.
      $begin_date = ($when == 'timestamps') ? (isset($timestamps[$i]['date']) ? $timestamps[$i]['date'] : '') : $period_start;

      $form['basic']['when']['datetimes'][$i]['begin']['date'] = array(
        '#type' => 'date_popup',
        '#date_label_position' => 'none',
        '#title' => t('Start date'),
        '#default_value' => (!$triggering_element) ? $begin_date : '',
        '#date_format' => 'Y-m-d',
        '#ajax' => array(
          'callback' => 'culturefeed_entry_ui_add_more_js',
          'wrapper' => 'when-ajax-wrapper',
          'effect' => 'fade',
        ),
        '#attributes' => array('class' => array('start-date')),
      );

      // Timestamp begin hour.
      if ($when == 'timestamps') {

        $begin_time = isset($timestamps[$i]['begintime']) ? $timestamps[$i]['date'] . ' ' . $timestamps[$i]['begintime'] : '';

        $form['basic']['when']['datetimes'][$i]['begin']['hour'] = array(
          '#type' => 'date_select',
          '#name' => 'begin_hour',
          '#default_value' => (!$triggering_element) ? $begin_time : '',
          '#date_format' => 'H:i',
          '#date_increment' => 30,
        );

      }

      // Check if the date has an end.
      // First check if we are dealing with a period.
      // Next check if we are in the initial form build and a timestamp
      // endtime exists.
      // Next check if there is a submitted end.
      // Next check if a new end is being added.
      $has_end = (
        ($when == 'period') ||
        (!$triggering_element && isset($timestamps[$i]['endtime'])) ||
        (isset($form_state['values']['datetimes'][$i]['end'])) ||
        ($triggering_element == 'datetimes_add_end_' . $i)
      );

      // Timestamp / Period add end.
      if (!$has_end) {

        $form['basic']['when']['datetimes'][$i]['begin']['datetimes_add_end'] = array(
          '#type' => 'submit',
          '#value' => t('+ add end'),
          '#name' => 'datetimes_add_end_' . $i,
          '#attributes' => array('class' => array('add-more-link btn-link')),
          '#ajax' => array(
            'callback' => 'culturefeed_entry_ui_add_more_js',
            'wrapper' => 'when-ajax-wrapper',
            'effect' => 'fade',
          ),
          '#limit_validation_errors' => array(array('datetimes')),
          '#submit' => array('culturefeed_entry_ui_add_more_submit'),
        );

      }

      // Timestamp / Period end wrapper.
      if ($has_end) {

        $form['basic']['when']['datetimes'][$i]['end'] = array(
          '#type' => 'container',
          '#prefix' => '<div id="datetime-child-' . $i . '-end-ajax-wrapper">',
          '#suffix' => '</div>',
        );

        if ($when == 'period' || ($date_count == 1 && !isset($form_state['values']['datetimes'][$i]['end']['date']))) {

          $form['basic']['when']['datetimes'][$i]['end']['date'] = array(
            '#type' => 'date_popup',
            '#title' => t('End date'),
            '#date_label_position' => 'none',
            '#default_value' => (!$triggering_element) ? $period_end : '',
            '#date_format' => 'Y-m-d',
            '#ajax' => array(
              'callback' => 'culturefeed_entry_ui_add_more_js',
              'wrapper' => 'when-ajax-wrapper',
              'effect' => 'fade',
            ),
          );

        }

        if ($when == 'timestamps') {

          $end_time = isset($timestamps[$i]['endtime']) ? $timestamps[$i]['date'] . ' ' . $timestamps[$i]['endtime'] : '';

          $form['basic']['when']['datetimes'][$i]['end']['hour'] = array(
            '#type' => 'date_select',
            '#default_value' => (!$triggering_element) ? $end_time : '',
            '#date_format' => 'H:i',
            '#date_increment' => 30,
          );

        }

        $form['basic']['when']['datetimes'][$i]['end']['datetimes_remove_end'] = array(
          '#type' => 'submit',
          '#value' => t('+ remove end'),
          '#name' => 'datetimes_remove_end_' . $i,
          '#attributes' => array('class' => array('add-more-link btn-link')),
          '#ajax' => array(
            'callback' => 'culturefeed_entry_ui_add_more_js',
            'wrapper' => 'when-ajax-wrapper',
            'effect' => 'fade',
          ),
          '#limit_validation_errors' => array(array('datetimes')),
          '#submit' => array('culturefeed_entry_ui_add_more_submit'),
        );

      }

    }

    if ($when == 'timestamps') {

      $form['basic']['when']['datetimes']['datetimes_add_child'] = array(
        '#type' => 'submit',
        '#name' => 'datetimes_add_child',
        '#value' => t('+ add date'),
        '#attributes' => array('class' => array('add-more-link btn-link')),
        '#ajax' => array(
          'callback' => 'culturefeed_entry_ui_add_more_js',
          'wrapper' => 'when-ajax-wrapper',
          'effect' => 'fade',
        ),
        '#limit_validation_errors' => array(array('datetimes')),
        '#submit' => array('culturefeed_entry_ui_add_more_submit'),
      );

    }

  }

  $form['basic']['when']['permanent'] = array(
    '#type' => 'checkbox',
    '#default_value' => ($when == 'permanent'),
    '#title' => t('Permanent'),
    '#ajax' => array(
      'callback' => 'culturefeed_entry_ui_add_more_js',
      'wrapper' => 'when-ajax-wrapper',
      'effect' => 'fade',
    ),
  );

  if ($when == 'period' || $when == 'permanent') {
    
    // TODO put in function cos we need it for permanent as well
    // Period / permanent elements.
    $form['basic']['when']['period_or_permanent'] = array(
      '#tree' => TRUE,
      '#type' => 'container',
      '#states' => array(
        'visible' => array(
          ':input[name=when]' => array(
            array('value' => 'period'),
            array('value' => 'permanent'),
          ),
        ),
      ),
    );
  
    $form['basic']['when']['period_or_permanent']['all_day'] = array(
      '#type' => 'radios',
      '#title' => t('Open all day?'),
      '#options' => array(1 => t('Yes'), 0 => t('No')),
      '#default_value' => ($weekscheme == NULL) ? 1 : 0,
    );
  
    $form['basic']['when']['period_or_permanent']['opening_times'] = array(
      '#type' => 'container',
      '#states' => array(
        'visible' => array(':input[name="period_or_permanent[all_day]"]' => array('value' => '0')),
      ),
      '#prefix' => '<div id="opening-times-ajax-wrapper">',
      '#suffix' => '</div>',
      '#theme' => 'culturefeed_entry_ui_opening_times_table',
    );
  
    // Add opening times information for every day.
    foreach (CultureFeed_Cdb_Data_Calendar_SchemeDay::$allowedDays as $day) {
  
      $form['basic']['when']['period_or_permanent']['opening_times'][$day] = array(
        '#tree' => TRUE,
        '#type' => 'fieldset',
        '#title' => $day_labels[$day],
      );
  
      $opening_times = array();
      if ($weekscheme && $data = $weekscheme->getDay($day)) {
        $opening_times = $data->getOpeningTimes();
      }
  
      $opening_count = 0;
      if ($opening_times) {
        foreach ($opening_times as $i => $opening_time) {
          culturefeed_entry_ui_attach_opening_time_elements($form, $day, $i, $opening_time);
          $opening_count++;
        }
      }
      else {
        culturefeed_entry_ui_attach_opening_time_elements($form, $day, 0);
        $opening_count++;
      }
  
    }

    // Add more button was clicked. Check how many extra rows need to be added.
    if (isset($form_state['#opening_times_needed'])) {
      for (; $opening_count < $form_state['#opening_times_needed']; $opening_count++) {
        culturefeed_entry_ui_attach_opening_time_elements($form, $day, $opening_count);
      }
    }


    /*$form['basic']['when']['period_or_permanent']['add_more_opening_times'] = array(
      '#type' => 'submit',
      '#name' => 'opening_times_add_more',
      '#value' => t('+ add another opening time'),
      '#attributes' => array('class' => array('opening-times-add-more-submit add-more-link btn-link')),
      '#ajax' => array(
        'callback' => 'culturefeed_entry_ui_add_more_js',
        'wrapper' => 'opening-times-ajax-wrapper',
        'effect' => 'fade',
      ),
      '#limit_validation_errors' => array(array('period_or_permanent')),
      '#submit' => array('culturefeed_entry_ui_add_more_submit'),
      '#states' => array(
        'visible' => array(':input[name="period_or_permanent[all_day]"]' => array('value' => '0')),
      ),
    );*/
  }

  // Extra.
  $form['extra'] = array(
    '#type' => 'fieldset',
    '#title' => t('Extra info'),
  );

  $form['extra']['description'] = array(
    '#tree' => TRUE,
    '#type' => 'container',
    '#prefix' => '<div id="description-ajax-wrapper">',
    '#suffix' => '</div>',
    '#title' => t('Description'),
  );

  $form['extra']['description']['sd'] = array(
    '#tree' => TRUE,
    '#type' => 'container',
    '#prefix' => '<div id="description-sd-ajax-wrapper">',
    '#suffix' => '</div>',
    '#title' => t('Description'),
  );

  $form['extra']['description']['sd']['short_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Short description'),
    '#rows' => 4,
    '#default_value' => $default_short_description,
  );

  $form['extra']['description']['ld'] = array(
    '#tree' => TRUE,
    '#type' => 'container',
    '#prefix' => '<div id="description-ld-ajax-wrapper">',
    '#suffix' => '</div>',
    '#title' => t('Description'),
  );

  //Long Description
  if (!isset($form_state['#ld_needed'])) {
    $form['extra']['description']['ld']['add_long_description'] = array(
      '#type' => 'submit',
      '#name' => 'ld_add_more',
      '#value' => t('+ add long description'),
      '#attributes' => array('class' => array('add-more-link btn-link')),
      '#ajax' => array(
        'callback' => 'culturefeed_entry_ui_add_more_js',
        'wrapper' => 'description-ld-ajax-wrapper',
        'effect' => 'fade',
      ),
      '#limit_validation_errors' => array(array('ld')),
      '#submit' => array('culturefeed_entry_ui_add_more_submit'),
    );
  }

  if (isset($form_state['#ld_needed'])) {
    $form['extra']['description']['ld']['long_description'] = array(
      '#type' => 'text_format',
      '#title' => t('Long description'),
      '#rows' => 10,
      '#default_value' => $default_long_description,
      '#name' => 'long_description',
      '#after_build' => array('culturefeed_entry_ui_remove_filter_input'),
      '#format' => filter_default_format(),
    );
  }

  // Tags todo
  $form['extra']['keywords'] = array(
    '#type' => 'textfield',
    '#title' => t('Tags'),
    '#description' => t('Add multiple tags seperated by a ";"'),
    '#default_value' => $keywords,
  );

  // Links
  $form['extra']['links'] = array(
    '#tree' => TRUE,
    '#type' => 'container',
    '#prefix' => '<div id="links-ajax-wrapper">',
    '#suffix' => '</div>',
    '#title' => t('Links'),
  );

  /*if ($default_links) {
    $links_count = count($default_links);
  }
  else {*/
    $links_count = 0;
  //}
  // Add link type selections.
  if ($default_links) {
    foreach ($default_links as $i => $info) {
      culturefeed_entry_ui_attach_link_info_elements($form, $i, $info, $default_links_reservation[$i]);
      $links_count++;
    }
  }
  else {
    culturefeed_entry_ui_attach_link_info_elements($form, 0);
    $links_count++;
  }

  // Add more button was clicked. Check how many extra rows need to be added.
  if (isset($form_state['#links_needed'])) {
    for (; $links_count < $form_state['#links_needed']; $links_count++) {
      culturefeed_entry_ui_attach_link_info_elements($form, $links_count);
    }
  }

  $form['extra']['add_more_links'] = array(
    '#type' => 'submit',
    '#name' => 'links_add_more',
    '#value' => t('+ add another link'),
    '#attributes' => array('class' => array('add-more-link btn-link')),
    '#ajax' => array(
      'callback' => 'culturefeed_entry_ui_add_more_js',
      'wrapper' => 'links-ajax-wrapper',
      'effect' => 'fade',
    ),
    '#limit_validation_errors' => array(array('links')),
    '#submit' => array('culturefeed_entry_ui_add_more_submit'),
  );

  //Photo
  $form['extra']['photo'] = array(
    '#tree' => TRUE,
    '#type' => 'container',
    '#prefix' => '<div id="photo-ajax-wrapper">',
    '#suffix' => '</div>',
    '#title' => t('Image'),
  );

  $form['extra']['photo']['upload'] = array(
    '#type' => 'managed_file',
    '#name' => 'image_upload',
    '#title' => t('Image upload'),
    '#default_value' => isset($default_image->fid) ? $default_image->fid : '',
    '#description' => t('Allowed extensions: gif png jpg jpeg'),
    '#upload_location' => 'public://uploads/',
    '#upload_validators' => array(
      'file_validate_extensions' => array('gif png jpg jpeg'),
      // Pass the maximum file size in bytes
      'file_validate_size' => array(1024 * 1024 * 1024),
    ),
  );

  if (isset($default_image->fid)) {
    $form['extra']['photo']['current_file'] = array(
      '#type' => 'hidden',
      '#value' => $default_image->fid,
    );
    $form['extra']['photo']['current_file_hlink'] = array(
      '#type' => 'hidden',
      '#value' => $default_image_hlink,
    );
  }

  $form['extra']['photo']['copyright'] = array(
    '#type' => 'checkbox',
    '#default_value' => isset($default_image->fid) ? 1 : 0,
    '#title' => t('I agree to the <a href="#">general conditions of UiTdatabank</a> and I declare that I have the necessary rights or permissions to distribute the image through UiTdatabank.'),
  );

  // Price  
  $form['extra']['price'] = array(
    '#tree' => TRUE,
    '#type' => 'container',
    '#title' => t('Price'),
  );

  $form['extra']['price']['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Price'),
    '#default_value' => isset($default_price) ? $default_price : '',
    '#attributes' => array(
      'class' => array(
        'price-amount'
      ),
      'placeholder' => array(
        '€'
      ),
    ),
    '#size' => 15,
  );

  $form['extra']['price']['free'] = array(
    '#type' => 'checkbox',
    '#title' => t('Free event'),
    '#default_value' => isset($default_free_price) ? $default_free_price : 0,
    '#states' => array(
      'checked' => array(
        'input[name="price_amount"]' => array('value' => '0'),
      ),
      'invisible' => array(
        'input[name="price_amount"]' => array('filled' => TRUE),
      ),
    ),
  );

  $form['extra']['price']['extra'] = array(
    '#tree' => TRUE,
    '#type' => 'container',
    '#prefix' => '<div id="price-extra-ajax-wrapper">',
    '#suffix' => '</div>',
    '#title' => t('Price'),
  );

  if (!isset($form_state['#price_needed'])) {
    $form['extra']['price']['extra']['add_extra_info'] = array(
      '#type' => 'submit',
      '#name' => 'price_add_more',
      '#value' => t('+ add price info'),
      '#attributes' => array(
        'class' => array(
          'add-more-link btn-link'
        ),
        'style' => array(
          'display:block; clear:left;'
        ),
      ),
      '#ajax' => array(
        'callback' => 'culturefeed_entry_ui_add_more_js',
        'wrapper' => 'price-extra-ajax-wrapper',
        'effect' => 'fade',
      ),
      '#limit_validation_errors' => array(array('price')),
      '#submit' => array('culturefeed_entry_ui_add_more_submit'),
      '#states' => array(
        'visible' => array(
          'input[name="price_amount"]' => array('filled' => TRUE),
        ),
      ),
    );
  }

  if (isset($form_state['#price_needed'])) {
    $form['extra']['price']['extra']['extra_info'] = array(
      '#type' => 'textarea',
      '#title' => t('Extra price info'),
      '#default_value' => isset($default_price_description) ? $default_price_description : '',
      '#states' => array(
        'visible' => array(
          'input[name="price_free"]' => array('checked' => FALSE),
        ),
        'invisible' => array(
          'input[name="price_amount"]' => array('empty' => TRUE),
        ),
      ),
    );
  }

  // Organiser add label (organisatie niet gevonden)
  $form['extra']['organiser'] = array(
    '#tree' => TRUE,
    '#type' => 'container',
    '#prefix' => '<div id="organiser-ajax-wrapper">',
    '#suffix' => '</div>',
  );

  $form['extra']['organiser']['actor'] = array(
    '#prefix' => '<div id="organiser-actor-ajax-wrapper">',
    '#suffix' => '</div>',
    '#type' => 'container',
  );
  if (!isset($form_state['#actor_needed'])) {
    $form['extra']['organiser']['actor']['organiser_actor_label'] = array(
      '#type' => 'textfield',
      '#title' => t('Organisation'),
      '#autocomplete_path' => 'autocomplete/culturefeed/actor-suggestions',
      '#required' => FALSE,
      '#default_value' => $default_organiser_label,
    );
  }

  $form['extra']['organiser']['actor']['organiser_actor_id'] = array(
    '#id' => 'organiser_actor_id',
    '#type' => 'hidden',
    '#default_value' => $default_organiser_id,
    '#ajax' => array(
      'callback' => 'culturefeed_entry_ui_add_more_js',
      'wrapper' => 'organiser-actor-ajax-wrapper',
      'event' => 'organiser_eventsearch',
    ),
    '#attributes' => array(
      'id' => 'organiser_actor_id',
    ),
  );

  $form['extra']['organiser']['add_new_actor'] = array(
    '#type' => 'submit',
    '#name' => 'actor_add_more',
    '#value' => t('Organisation not found, add organisation'),
    '#attributes' => array(
      'class' => array('add-more-link btn-link'),
      'style' => array('display:none'),
    ),
    '#ajax' => array(
      'callback' => 'culturefeed_entry_ui_add_more_js',
      'wrapper' => 'organiser-ajax-wrapper',
      'effect' => 'fade',
    ),
    '#limit_validation_errors' => array(array('price')),
    '#submit' => array('culturefeed_entry_ui_add_more_submit'),
  );

  if (isset($form_state['#actor_needed'])) {
    $form['extra']['organiser']['new_actor'] = array(
      '#type' => 'textfield',
      '#title' => t('Name new organiser'),
    );
  }

  // Age
  $form['extra']['age_category'] = array(
    '#type' => 'select',
    '#title' => t('Age'),
    '#options' => array(
      'everyone' => t('For everyone'),
      '1-12' => t('For children (between 1 and 12 years)'),
      '12-18' => t('For youth (between 12 and 18 years)'),
      '18+' => t('For adults (18+)'),
    ),
    '#default_value' => $age_category,
  );

  $form['extra']['age'] = array(
    '#title' => t('Age from'),
    '#type' => 'textfield',
    '#states' => array(
      'invisible' => array(
        ':input[name=age_category]' => array(
          array('value' => 'everyone'),
        ),
      ),
    ),
    '#default_value' => $age,
  );

  $form['extra']['fly'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        ':input[name=age_category]' => array(
          array('value' => '1-12'),
        ),
      ),
    ),
  );
  $form['extra']['fly']['value'] = array(
    '#markup' => t("Activities for children will get the 'Fly' label"),
  );

  // Start vertical tabs
  $form['extra']['vertical_tabs'] = array(
    '#type' => 'vertical_tabs',
  );

  // Contact
  $form['extra']['contacts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Contacts'),
    '#group' => 'vertical_tabs',
  );

  $form['extra']['contacts']['wrapper'] = array(
    '#tree' => TRUE,
    '#type' => 'container',
    '#prefix' => '<div id="contacts-ajax-wrapper">',
    '#suffix' => '</div>',
  );

  $contact_count = 0;
  // Add contacts type.
  if ($default_contacts) {
    foreach ($default_contacts as $i => $info) {
      culturefeed_entry_ui_attach_contacts_elements($form, $i, $info);
      $contact_count++;
    }
  }
  else {
    culturefeed_entry_ui_attach_contacts_elements($form, 0);
    $contact_count++;
  }
  // Add more button was clicked. Check how many extra rows need to be added.
  if (isset($form_state['#contacts_needed'])) {
    for (; $contact_count < $form_state['#contacts_needed']; $contact_count++) {
      culturefeed_entry_ui_attach_contacts_elements($form, $contact_count);
    }
  }

  $form['extra']['contacts']['wrapper']['add_more_contacts'] = array(
    '#type' => 'submit',
    '#name' => 'contacts_add_more',
    '#value' => t('+ add another contact'),
    '#attributes' => array('class' => array('add-more-link btn-link')),
    '#ajax' => array(
      'callback' => 'culturefeed_entry_ui_add_more_js',
      'wrapper' => 'contacts-ajax-wrapper',
      'effect' => 'fade',
    ),
    '#limit_validation_errors' => array(array('contacts')),
    '#submit' => array('culturefeed_entry_ui_add_more_submit'),
  );

  // Performers todo
  $form['extra']['performers'] = array(
    '#type' => 'fieldset',
    '#title' => t('Performers'),
    '#group' => 'vertical_tabs',
  );

  $form['extra']['performers']['wrapper'] = array(
    '#tree' => TRUE,
    '#type' => 'container',
    '#prefix' => '<div id="performers-ajax-wrapper">',
    '#suffix' => '</div>',
  );


  $performers_count = 0;
  // Add performer type.
  if ($performers_info) {
    foreach ($performers_info as $i => $info) {
      culturefeed_entry_ui_attach_performers_elements($form, $i, $info);
      $performers_count++;
    }
  }
  else {
    culturefeed_entry_ui_attach_performers_elements($form, 0);
    $performers_count++;
  }

  // Add more button was clicked. Check how many extra rows need to be added.
  if (isset($form_state['#performers_needed'])) {
    for (; $performers_count < $form_state['#performers_needed']; $performers_count++) {
      culturefeed_entry_ui_attach_performers_elements($form, $performers_count);
    }
  }

  $form['extra']['performers']['wrapper']['add_more_performers'] = array(
    '#type' => 'submit',
    '#name' => 'performers_add_more',
    '#value' => t('+ add another performer'),
    '#attributes' => array('class' => array('add-more-link btn-link')),
    '#ajax' => array(
      'callback' => 'culturefeed_entry_ui_add_more_js',
      'wrapper' => 'performers-ajax-wrapper',
      'effect' => 'fade',
    ),
    '#limit_validation_errors' => array(array('performers')),
    '#submit' => array('culturefeed_entry_ui_add_more_submit'),
  );

  //language
  $form['extra']['language'] = array(
    '#type' => 'fieldset',
    '#title' => t('Language'),
    '#group' => 'vertical_tabs',
  );
  
  $form['extra']['language']['wrapper'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="languages-ajax-wrapper">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );
  
  $language_count = 0;
  // Add language
  if ($language_count) {
    foreach ($language_count as $i => $info) {
	  culturefeed_entry_ui_attach_language_elements($form, $i, $info);
	  $language_count++;
    }
  }
  else {
    culturefeed_entry_ui_attach_language_elements($form, 0);
  }
  
  // Add more button was clicked. Check how many rows need to be added.
  if (isset($form_state['#languages_needed'])) {
    for (; $language_count < $form_state['#languages_needed']; $language_count++) {
	  culturefeed_entry_ui_attach_language_elements($form, $language_count);
    }
  }
  
  $form['extra']['language']['wrapper']['add_more_languages'] = array(
    '#type' => 'submit',
    '#name' => 'languages_add_more',
    '#value' => t('+ add more language info'),
    '#attributes' => array('class' => array('add-more-link btn-link')),
    '#ajax' => array(
      'callback' => 'culturefeed_entry_ui_add_more_js',
      'wrapper' => 'languages-ajax-wrapper',
      'effect' => 'fade',
    ),
    '#limit_validation_errors' => array(array('languages')),
    '#submit' => array('culturefeed_entry_ui_add_more_submit'),
  );

  //entrance
  //$form['extra']['target_group'] = array(
  $form['extra']['entrance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Entrance'),
    '#group' => 'vertical_tabs',
  );

  $form['extra']['entrance']['members'] = array(
    '#type' => 'checkbox',
    '#title' => t('Entrance'),
    '#description' => t('Only for members'),
    '#default_value' => $is_private,
  );

  // translations
  $form['extra']['translations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Translations'),
    '#group' => 'vertical_tabs',
  );

  // Dutch
  if (culturefeed_search_get_preferred_language() != 'nl') {
    $form['extra']['translations']['dutch'] = array(
      '#type' => 'fieldset',
      '#title' => t('Dutch'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,
    );

    $form['extra']['translations']['dutch']['language'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#default_value' => $nl_title,
    );

    $form['extra']['translations']['dutch']['short_description'] = array(
      '#type' => 'textarea',
      '#title' => t('Short description'),
      '#default_value' => $nl_short_description,
    );

    $form['extra']['translations']['dutch']['long_description'] = array(
      '#type' => 'text_format',
      '#title' => t('Long description'),
      '#default_value' => $nl_long_description,
      '#after_build' => array('culturefeed_entry_ui_remove_filter_input'),
    );
  }

  //english
  if (culturefeed_search_get_preferred_language() != 'en') {
    $form['extra']['translations']['english'] = array(
      '#type' => 'fieldset',
      '#title' => t('English'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,
    );

    $form['extra']['translations']['english']['language'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#default_value' => $en_title,
    );

    $form['extra']['translations']['english']['short_description'] = array(
      '#type' => 'textarea',
      '#title' => t('Short description'),
      '#default_value' => $en_short_description,
    );

    $form['extra']['translations']['english']['long_description'] = array(
      '#type' => 'text_format',
      '#title' => t('Long description'),
      '#default_value' => $en_long_description,
      '#after_build' => array('culturefeed_entry_ui_remove_filter_input'),
    );
  }

  //french
  if (culturefeed_search_get_preferred_language() != 'fr') {
    $form['extra']['translations']['french'] = array(
      '#type' => 'fieldset',
      '#title' => t('French'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,
    );

    $form['extra']['translations']['french']['language'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#default_value' => $fr_title,
    );

    $form['extra']['translations']['french']['short_description'] = array(
      '#type' => 'textarea',
      '#title' => t('Short description'),
      '#default_value' => $fr_short_description,
    );

    $form['extra']['translations']['french']['long_description'] = array(
      '#type' => 'text_format',
      '#title' => t('Long description'),
      '#default_value' => $fr_long_description,
      '#after_build' => array('culturefeed_entry_ui_remove_filter_input'),
    );
  }

  //german
  if (culturefeed_search_get_preferred_language() != 'de') {
    $form['extra']['translations']['german'] = array(
      '#type' => 'fieldset',
      '#title' => t('German'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,
    );

    $form['extra']['translations']['german']['language'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#default_value' => $de_title,
    );

    $form['extra']['translations']['german']['short_description'] = array(
      '#type' => 'textarea',
      '#title' => t('Short description'),
      '#default_value' => $de_short_description,
    );

    $form['extra']['translations']['german']['long_description'] = array(
      '#type' => 'text_format',
      '#title' => t('Long description'),
      '#default_value' => $de_long_description,
      '#after_build' => array('culturefeed_entry_ui_remove_filter_input'),
    );
  }
  //Publication & Private
  $form['extra']['publication_date'] = array(
    '#type' => 'fieldset',
    '#title' => t('Publication'),
    '#group' => 'vertical_tabs',
  );

  $form['extra']['publication_date']['wrapper'] = array(
    '#tree' => TRUE,
    '#type' => 'container',
    '#prefix' => '<div id="pub-date-ajax-wrapper">',
    '#suffix' => '</div>',
  );

  $form['extra']['publication_date']['wrapper']['date'] = array(
    '#type' => 'textfield',
    '#title' => t('Publication date'),
    '#default_value' => $publication_date,
    '#description' => t('Format') . ': ' . date('Y-m-d'),
  );

  $form['extra']['publication_date']['wrapper']['private'] = array(
    '#type' => 'checkbox',
    '#title' => t('Public'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#validate' => array('culturefeed_entry_ui_event_save_validate'),
  );

  $form['#attached']['css'][] = drupal_get_path('module', 'culturefeed_entry_ui') . '/css/culturefeed_entry_ui.css';
  $form['#attached']['js'][] = drupal_get_path('module', 'culturefeed_entry_ui') . '/js/culturefeed_entry_ui.js';
  return $form;

}


/**
 * Attach a new datetime element.
 *
 */
function culturefeed_entry_ui_attach_begindate_elements(&$form, $i, $info = NULL) {

  $date = date("Y-m-d");
  $hour = date("H:i");
  $format = 'Y-m-d';

  $form['basic']['when']['datetimes'][$i] = array(
    '#type' => 'container',
    '#prefix' => '<div id="datetime-child-' . $i . '-ajax-wrapper" class="datetime-child">',
    '#suffix' => '</div>',
  );

  $form['basic']['when']['datetimes'][$i]['begin'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="datetime-child-' . $i . '-begin-ajax-wrapper">',
    '#suffix' => '</div>',
  );

  $form['basic']['when']['datetimes'][$i]['begin']['date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Start date'),
    '#default_value' => '',
    '#date_format' => 'Y-m-d',
    '#attributes' => array('class' => array('start-date')),
  );

  $form['basic']['when']['datetimes'][$i]['begin']['hour'] = array(
    '#type' => 'date_select',
    '#name' => 'begin_hour',
    '#title' => t('Start time'),
    '#default_value' => '',
    '#date_format' => 'H:i',
    '#date_increment' => 30,
  );

  culturefeed_entry_ui_attach_enddate_elements($form, $i, $info);

  /*$enddates = array();
  $enddate_count = 0;

  if ($enddates) {
    foreach ($enddates as $j => $jinfo) {
      culturefeed_entry_ui_attach_enddate_elements($form, $j, $jinfo);
      $enddate_count++;
    }
  }
  else {
    culturefeed_entry_ui_attach_enddate_elements($form, 0);

    $enddate_count++;
  }

  // Add more button was clicked. Check how many extra rows need to be added.
  if (isset($form_state['#enddates_needed'])) {

    for (; $enddate_count < $form_state['#enddates_needed']; $enddate_count++) {
            culturefeed_entry_ui_attach_enddate_elements($form, $enddate_count);
    }
  }

  $form['basic']['when']['datetimes'][$i] = array(
    '#type' => 'submit',
    '#name' => 'enddates_add_more',
    '#value' => t('+ add end'),
    '#attributes' => array('class' => array('add-more-link btn-link add-endtime-link')),
    '#ajax' => array(
      'callback' => 'culturefeed_entry_ui_add_more_js',
      'wrapper' => 'datetime-child'. $i .'-ajax-wrapper',
      'effect' => 'fade',
    ),
    '#limit_validation_errors' => array(array('datetimes')),
    '#submit' => array('culturefeed_entry_ui_add_more_submit'),
  );*/

}

function culturefeed_entry_ui_attach_enddate_elements(&$form, $i, $info = NULL) {

  $date = date("Y-m-d");
  $hour = date("H:i");
  $format = 'Y-m-d';

  $form['basic']['when']['datetimes'][$i]['end'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="datetime-child-' . $i . '-end-ajax-wrapper">',
    '#suffix' => '</div>',
  );

  $form['basic']['when']['datetimes'][$i]['end']['date'] = array(
    '#type' => 'date_popup',
    '#title' => t('End date'),
    '#default_value' => '',
    '#date_format' => 'Y-m-d',
  );

  $form['basic']['when']['datetimes'][$i]['end']['hour'] = array(
    '#type' => 'date_select',
    '#title' => t('End time'),
    '#default_value' => '',
    '#date_format' => 'H:i',
    '#date_increment' => 30,
  );
}

/**
 * Attach a new language element.
 *
 */
function culturefeed_entry_ui_attach_language_elements(&$form, $i, $info = NULL) {

  $form['extra']['language']['wrapper'][$i] = array(
    '#type' => 'container',
    '#tree' => TRUE,
    '#prefix' => '<div class="language-item-wrapper">',
    '#suffix' => '</div>',
  );
  
  $form['extra']['language']['wrapper'][$i]['spoken_language'] = array(
    '#type' => 'select',
    '#title' => t('Language'),
    '#options' => array(
      0 => t(''),
      1 => t('Spoken'),
      2 => t('Dubbed'),
      3 => t('Subtitled'),
    ),
  );

  $form['extra']['language']['wrapper'][$i]['in_language'] = array(
    '#type' => 'select',
    '#title' => t('in'),
    '#options' => array(
      0 => t(''),
      1 => t('English'),
      2 => t('Spanish'),
      3 => t('Dutch'),
      4 => t('German'),
      5 => t('French'),
    ),
  );
}

/**
 * Attach a new contact element.
 *
 */
function culturefeed_entry_ui_attach_contacts_elements(&$form, $i, $info = NULL) {

  $options = array(0 => 'Phone', 1 => 'Email');
  $contact_type = 0;

  if ($info[0] == 'phone') {
    $contact_type = 0;
  }

  if ($info[0] == 'E-mail') {
    $contact_type = 1;
  }

  $contact = $info[1];

  $form['extra']['contacts']['wrapper'][$i]['channel'] = array(
    '#type' => 'select',
    '#title' => t('Contact'),
    '#title_display' => 'invisible',
    '#options' => $options,
    '#default_value' => $contact_type,
    '#attributes' => array('class' => array('channel-field')),
  );

  $form['extra']['contacts']['wrapper'][$i]['channel_input'] = array(
    '#type' => 'textfield',
    '#title' => t('Input'),
    '#title_display' => 'invisible',
    '#default_value' => $contact,
    '#attributes' => array('class' => array('channel-input-field')),
  );

}

/**
 * Attach a new performer element.
 *
 */
function culturefeed_entry_ui_attach_performers_elements(&$form, $i, $info = NULL) {

  $form['extra']['performers']['wrapper'][$i]['performer'] = array(
    '#type' => 'textfield',
    '#title' => t('Performer'),
    '#display_title' => FALSE,
    '#default_value' => $info[1],
    '#attributes' => array('class' => array('performer-field')),
  );

  $form['extra']['performers']['wrapper'][$i]['role'] = array(
    '#type' => 'textfield',
    '#title' => t('Role'),
    '#default_value' => $info[0],
    '#attributes' => array('class' => array('role-field')),
  );

}

/**
 * Attach a new link element.
 *
 */
function culturefeed_entry_ui_attach_link_info_elements(&$form, $i, $info = NULL, $reserve = NULL) {

  $form['extra']['links'][$i]['URL'] = array(
    '#type' => 'textfield',
    '#title' => t('Link'),
    '#default_value' => $info,
    '#attributes' => array('class' => array('link-field')),
  );

  $form['extra']['links'][$i]['reservation'] = array(
    '#type' => 'checkbox',
    '#title' => t('for reservation'),
    '#default_value' => $reserve,
    '#attributes' => array('class' => array('reservation-checkbox')),
  );

}


/**
 * Attach opening time elements for a day.
 *
 * @param array $form
 *   Form to attach to.
 * @param string $day
 *   English name of the day to attach, for example 'monday'.
 * @param int $i
 *   Index to set.
 * @param CultureFeed_Cdb_Data_Calendar_OpeningTime $opening_time
 *   Openingtime information
 */
function culturefeed_entry_ui_attach_opening_time_elements(&$form, $day, $i, $opening_time = NULL) {

  $form['basic']['when']['period_or_permanent']['opening_times'][$day][$i]['open_from'] = array(
    '#type' => 'textfield',
    '#size' => 12,
    '#default_value' => $opening_time ? $opening_time->getOpenFrom() : '',
    '#description' => t('Format') . ': 12:00:00',
  );

  $form['basic']['when']['period_or_permanent']['opening_times'][$day][$i]['open_till'] = array(
    '#type' => 'textfield',
    '#size' => 12,
    '#default_value' => $opening_time ? $opening_time->getOpenTill() : '',
    '#description' => t('Format') . ': 12:00:00',
  );

}

/**
 * Validate the event form.
 */
function culturefeed_entry_ui_event_save_validate($form, &$form_state) {

  $values = $form_state['values'];
  // Validate publication date.
  if ($values['wrapper']['date']) {
    try {
      CultureFeed_Cdb_Data_Calendar::validateDate($values['publication_date']);
    } catch (Exception $e) {
      form_set_error('publication_date', t('Please fill in correct publication date'));
    }
  }

  // Validate age.
  if ($values['age'] && !is_numeric($values['age'])) {
    form_set_error('age', t('Please fill in correct age'));
  }

  // Validate datetime
  // Validate timestamps.
  if ($values['datetype'] == 'timestamps') {
  
    // Only one timestamp saved for one day.
    if (count($values['datetimes']) == 1) {

      $form_state['values']['datetimes'] = array($values['datetimes'][0]);
      $values['datetimes'] = $form_state['values']['datetimes'];
    }

    foreach ($values['datetimes'] as $key => $timestamp) {
      try {

        // Empty start dates are ignored.
        if (empty($timestamp['begin']['date'])) {
          continue;
        }

        CultureFeed_Cdb_Data_Calendar::validateDate($timestamp['begin']['date']);

        if (!empty($timestamp['end']['date'])) {
          CultureFeed_Cdb_Data_Calendar::validateDate($timestamp['end']['date']);
        }
      }
      catch (UnexpectedValueException $e) {
        form_set_error('datetimes][' . $key, t('Please fill in correct date and time'));
      }
    }

    // Check if this is duplicate for above CultureFeed_Cdb_Data_Calendar::validateTime.
    if (empty($values['datetimes'][0]['begin']['date'])) {
      form_set_error('datetimes', t('A date is required.'));
    }
    else {

      for ($i = 0; $i < count($values['datetimes']); $i++) {
        $begindate = '';
        $begintime = '';
        $enddate = '';
        $endtime = '';
        if (isset($values['datetimes'][$i]['begin']['date'])){
          $begindate = strtotime($values['datetimes'][$i]['begin']['date']);
        }
        if (isset($values['datetimes'][$i]['begin']['hour'])) {
          $begintime = strtotime($values['datetimes'][$i]['begin']['hour']);
        }
        if (isset($values['datetimes'][$i]['end'])) {
          if (isset($values['datetimes'][$i]['end']['date'])) {
            $enddate = strtotime($values['datetimes'][$i]['end']['date']);
          }
          if(isset($values['datetimes'][$i]['end']['hour'])) {
            $endtime = strtotime($values['datetimes'][$i]['end']['hour']);
          }
        }

        if ($enddate){
          if ($begindate > $enddate) {
            form_set_error('datetimes', t('Date field is invalid. The end date is earlier than the beginning date.'));
          }
          if ($begintime > $endtime) {
            form_set_error('datetimes', t('Date field is invalid. The end time is earlier than the beginning time.'));
          }
        } else {
          if ($endtime && $begintime > $endtime) {
            form_set_error('datetimes', t('Date field is invalid. The end time is earlier than the beginning time.'));
          }
        }
      }
    }
  }

  if ($values['datetype'] == 'period' || $values['datetype'] == 'permanent') {

    // Validate the weekscheme.
    if (isset($values['period_or_permanent']) && !$values['period_or_permanent']['all_day']) {
      foreach ($values['period_or_permanent']['opening_times'] as $day => $opening_times) {
        foreach ($opening_times as $key => $opening_time) {

          if (!empty($opening_time['open_from']) || !empty($opening_time['open_till'])) {
            try {
              CultureFeed_Cdb_Data_Calendar::validateTime($opening_time['open_from']);
              CultureFeed_Cdb_Data_Calendar::validateTime($opening_time['open_till']);
            }
            catch (Exception $e) {
              form_set_error('period_or_permanent][opening_times][' . $day . '][' . $key, t('Please fill in correct date'));
            }
          }

        }
      }
    }

  }

  // Validate copyright
  if (!empty($values['photo']['upload']) && $values['photo']['copyright'] != 1) {
    form_set_error('photo', t('Please agree to the general conditions of UiTdatabank and declare that you have the necessary rights or permissions to distribute the image through UiTdatabank.'));
  }
  
  // Validate categories
  if (empty($values['what'])) {
    form_set_error('what', t('A category is required.'));
  }

  // Validate location
  if (isset($form_state['#location_needed']) && $form_state['#location_needed']) {
    if (empty($values['location']['new']['location_title'])) {
      form_set_error('location_title', t('A title is required for a custom location'));
    }
    if ($values['location']['new']['citywrapper']['country'] != 'BE') {
      if (empty($values['location']['new']['citywrapper']['zip_code'])) {
        form_set_error('basic[zip_code]', t('A zip code is required for a custom location'));
      }
    }
    if (empty($values['location']['new']['citywrapper']['city'])) {
      form_set_error('city', t('A city is required for a custom location'));
    }
    $location = NULL;
  }
  else {
    try {
      if ($values['location']['actor']['location_actor_label'] == '') {
	    form_set_error('location', t('A location is required.'));
      }
      else {
        $location = culturefeed_search_item_load($values['location']['actor']['location_actor_id'], 'actor');
        if (!$location) {
          form_set_error('location', t('We could not validate the location'));
        }
      }
    } catch (Exception $e) {
      form_set_error('location', t('We could not validate the location'));
    }
  }
  

    
  /*if (!empty($values['location']['actor']['location_actor_id'])) {

    try {
      $location = culturefeed_search_item_load($values['location']['actor']['location_actor_id'], 'actor');
      if (!$location) {
        form_set_error('location', t('We could not validate the location'));
      }
    } catch (Exception $e) {
      form_set_error('location', t('We could not validate the location'));
    }
  }
  else {
    if (empty($values['location']['location_title'])) {
      form_set_error('location_title', t('A title is required for a custom location'));
    }
    if ($values['location']['citywrapper']['country'] != 'BE') {
      if (empty($values['location']['citywrapper']['zip_code'])) {
        form_set_error('basic[zip_code]', t('A zip code is required for a custom location'));
      }
    }
    if (empty($values['location']['citywrapper']['city'])) {
      form_set_error('city', t('A city is required for a custom location'));
    }
    $location = NULL;
  }*/
  
  // Validate Links
  foreach ($values['links'] as $link_data) {
    if(!empty($link_data['URL'])) {
      if(!valid_url($link_data['URL'], TRUE)) {
	    form_set_error('links', t('Not a valid URL'));
      }
    }
  }

  // Validate organiser.
  if (!empty($values['organiser']['actor']['organiser_actor_id'])) {

    try {
      $organiser = culturefeed_search_item_load($values['organiser']['actor']['organiser_actor_id'], 'actor');
      if (!$organiser) {
        form_set_error('organiser', t('We could not validate the organization'));
      }
    } catch (Exception $e) {
      watchdog_exception('culturefeed_entry_ui', $e);
      form_set_error('organiser', t('We could not validate the organization'));
    }

  }
  else {
    $organiser = NULL;
  }
  
  foreach ($values['wrapper'] as $extra) {
    if (is_array($extra)) {
      
      // Contacts
      if (!empty($extra['channel_input'])) {
        //if mail is selected
        if($extra['channel'] == 1 ) {
          if (!valid_email_address($extra['channel_input'])) {
            form_set_error('channel_input', t('Not a valid email address'));
          }
        }
      }
    }
  }

  $errors = form_get_errors();
  if (empty($errors)) {
    _culturefeed_entry_ui_ui_event_form_save_event($form, $form_state, $location, $organiser);
  }

}

/**
 * Save the event form to the entity api.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 * @param CultuurNet\Search\ActivityStatsExtendedEntity $location
 *   The location of the event.
 * @param CultuurNet\Search\ActivityStatsExtendedEntity $organiser
 *   The organiser of the event.
 */
function _culturefeed_entry_ui_ui_event_form_save_event($form, &$form_state, CultuurNet\Search\ActivityStatsExtendedEntity $location = NULL, CultuurNet\Search\ActivityStatsExtendedEntity $organiser = NULL) {
  $values = $form_state['values'];

  $performerList = new CultureFeed_Cdb_Data_PerformerList();
  $mails = array();
  $phones = array();
  $links = array();
  $performers_count = 0;
  $language_list = new CultureFeed_Cdb_Data_LanguageList();

  foreach ($values['wrapper'] as $extra) {
    if (is_array($extra)) {
      // Performer
      if (!empty($extra['performer']) || !empty($extra['role'])) {
        $performer = new CultureFeed_Cdb_Data_Performer();
        $performer->setLabel($extra['performer']);
        $performer->setRole($extra['role']);
        $performerList->add($performer);
        $performers_count++;
      }

      // Contacts
      if (isset($extra['channel_input'])) {

        switch ($extra['channel']) {
          case '0':
            $phone = new CultureFeed_Cdb_Data_Phone($extra['channel_input'], CultureFeed_Cdb_Data_Phone::PHONE_TYPE_PHONE, FALSE, FALSE);
            array_push($phones, $phone);
            break;
          case '1':
            $mail = new CultureFeed_Cdb_Data_Mail($extra['channel_input'], FALSE, FALSE);
            array_push($mails, $mail);
            break;
        }
      }

      if (!empty($extra['spoken_language']) && !empty($extra['in_language'])) {
	    switch ($extra['spoken_language']) {
		  case 1:
            $spoken_language = 'spoken';
            break;
          case 2:
            $spoken_language = 'dubbed';
            break;
          case 3:
            $spoken_language = 'subtitles';
            break;  
	    }
	    
	    switch ($extra['in_language']) {
		  case 1:
            $in_language = 'Engels';
            break;
          case 2:
<<<<<<< HEAD
            $in_language = 'Spaans'; // zit niet in UDB
=======
            $in_language = 'Spaans'; //Zitniet in UDB, dus werkt niet
>>>>>>> languages
            break;
          case 3:
            $in_language = 'Nederlands';
           break;
          case 4:
            $in_language = 'Duits';
            break;
          case 5:
<<<<<<< HEAD
            $in_language = 'Frans';    
=======
            $in_language = 'Frans';
>>>>>>> languages
	    }

        $language_list->add(new CultureFeed_Cdb_Data_Language($in_language, $spoken_language));
      }
    }
  }


  // Links
  $media_links = array();
  foreach ($values['links'] as $link_data) {

    if ($link_data['URL']) {
      
      if (!preg_match("@^https?://@", $link_data['URL'])) {
	      $link_data['URL'] = 'http://' . $link_data['URL'];
      }
      
      $mediatype = CultureFeed_Cdb_Data_File::MEDIA_TYPE_WEBRESOURCE;
      
      if (strpos($link_data['URL'],'plus.google.com')) {
        $mediatype = CultureFeed_Cdb_Data_File::MEDIA_TYPE_GOOGLEPLUS;
      }
      
      if (strpos($link_data['URL'],'facebook.com')) {
        $mediatype = CultureFeed_Cdb_Data_File::MEDIA_TYPE_FACEBOOK;
      }
      
      if (strpos($link_data['URL'],'twitter.com')) {
        $mediatype = CultureFeed_Cdb_Data_File::MEDIA_TYPE_TWITTER;
      }
      
      if (strpos($link_data['URL'],'youtube.com') || strpos($link_data['URL'],'youtu.be')) {
        $mediatype = CultureFeed_Cdb_Data_File::MEDIA_TYPE_YOUTUBE;
      }
      
      if ($link_data['reservation']) {
        
        // Make reservation link for contact element
        $link = new CultureFeed_Cdb_Data_Url($link_data['URL'], FALSE, $link_data['reservation']);
        array_push($links, $link);

        // Make reservation link for media element
        $mediatype = CultureFeed_Cdb_Data_File::MEDIA_TYPE_RESERVATIONS;
      
      }
      
      $link = new CultureFeed_Cdb_Data_File();
      $link->setHLink($link_data['URL']);
      $link->setMediaType($mediatype);
      if ($link_data['reservation']) {
        $link->setTitle(t('Order tickets'));
      }
      array_push($media_links, $link);
    }
  }
  
  

  $update = FALSE;
  if (isset($form['#event'])) {
    $update = TRUE;
    $event = $form['#event'];
  }
  else {
    $event = new CultureFeed_Cdb_Item_Event();
  }

  // Publication date.
  if ($values['wrapper']['date']) {
    $event->setPublicationDate($values['wrapper']['date']);
  }

  // Age
  if ($values['age']) {
    $event->setAgeFrom(($values['age'] ? $values['age'] : 0));
  }
  else {
    // Age Category
    switch ($values['age_category']) {
      case '1-12':
        $event->setAgeFrom(1);
        break;
      case '12-18':
        $event->setAgeFrom(12);
        break;
      case '18+':
        $event->setAgeFrom(18);
        break;
      case 'everyone':
      default:
        break;
    }
  }

  // Datetimes as calendar.
  if ($values['datetype'] == 'timestamps') {
    _culturefeed_entry_ui_event_save_timestamps($event, $values);
  }
  
  // Datetimes as period.
  if ($values['datetype'] == 'period') {
    _culturefeed_entry_ui_event_save_period($event, $values);
  }

  // Weekscheme for period or permanent.
  if ($values['datetype'] == 'period' || $values['datetype'] == 'permanent') {
    _culturefeed_entry_ui_event_save_weekscheme($event, $values);
  }

  // Categories.
  $category_options = array();

  $category_options[$values['what']] = culturefeed_search_get_eventtype_categories(array('tid' => $values['what']));

  $categories = new CultureFeed_Cdb_Data_CategoryList();
  foreach ($category_options as $key => $value) {
    if ($value) {
      $categories->add(new CultureFeed_Cdb_Data_Category(CultureFeed_Cdb_Data_Category::CATEGORY_TYPE_EVENT_TYPE, $key, $value[$key]));
    }
  }
  $event->setCategories($categories);

  // Event details.
  $detail = new CultureFeed_Cdb_Data_EventDetail();
  $detail->setTitle($values['title']);

  if (!empty($values['description']['sd']['short_description'])) {
    $detail->setShortDescription($values['description']['sd']['short_description']);
  }
  if (!empty($values['description']['ld']['long_description']['value'])) {
    $detail->setLongDescription($values['description']['ld']['long_description']['value']);
  }

  // Photo
  if ($values['photo']['upload']) {

    // Save to Drupal
    $drupal_file = file_load($values['photo']['upload']);
    $drupal_file->status = FILE_STATUS_PERMANENT;
    file_save($drupal_file);
    file_usage_add($drupal_file, 'culturefeed_entry_ui', 'event', $drupal_file->fid);

    // Add to detail
    $file = new CultureFeed_Cdb_Data_File();
    $file->setMediaType($file::MEDIA_TYPE_IMAGEWEB);
    $file->setMain(TRUE);
    switch ($drupal_file->filemime) {
      case 'image/gif':
        $file->setFileType($file::FILE_TYPE_GIF);
        break;
      case 'image/jpg':
      case 'image/jpeg':
        $file->setFileType($file::FILE_TYPE_JPEG);
        break;
      case 'image/png':
        $file->setFileType($file::FILE_TYPE_PNG);
        break;
    }
    $file->setHLink(file_create_url($drupal_file->uri));
    $file->setFilename($drupal_file->filename);
    $detail->getMedia()->add($file);
    // Remove old image
    // Bug on UDB ... will be fixed soon
    if (isset($values['photo']['current_file_hlink'])) {
      try {
        Drupalculturefeed_EntryApi::removeLinkFromEvent($event, $values['photo']['current_file_hlink']);

      } catch (Exception $e) {
        watchdog_exception('culturefeed_entry_ui', $e);
      }
    }
  }
  
  // Media links
  if ($media_links) {
    foreach ($media_links as $media_link) {
      $detail->getMedia()->add($media_link);
    }
  }


  // Price
  if ($values['price']['free']) {
    $price = new CultureFeed_Cdb_Data_Price(0);
    $detail->setPrice($price);
  }
  else {
    if (!empty($values['price']['amount'])) {
      $price = new CultureFeed_Cdb_Data_Price(floatval($values['price']['amount']));
      if (!empty($values['price']['extra']['extra_info'])) {
        $price->setDescription($values['price']['extra']['extra_info']);
      }
      $detail->setPrice($price);
    }
  }

  /*if (isset($values['price']['free'])) {
    $price = new CultureFeed_Cdb_Data_Price(0);
    $detail->setPrice($price);
  }
  else {
    if ($values['price']['amount']) {
      $price = new CultureFeed_Cdb_Data_Price(floatval($values['price']['amount']));
      if (isset($values['price']['extra']['extra_info'])) {
        $price->setDescription($values['price']['extra']['extra_info']);
      }
      $detail->setPrice($price);
    }
  }*/
  // Performers
  if ($performers_count) {
    $detail->setPerformers($performerList);
  }
  
  $detail->setLanguage(culturefeed_entry_ui_get_preferred_language());

  $details = new CultureFeed_Cdb_Data_EventDetailList();
  $details->add($detail);

  // Translations Dutch.
  if (culturefeed_search_get_preferred_language() != 'nl') {
    if ($values['dutch']['language'] || $values['dutch']['short_description'] || $values['dutch']['long_description']['value']) {

      $detail = new CultureFeed_Cdb_Data_EventDetail();
      if ($values['dutch']['language']) {
        $detail->setTitle($values['dutch']['language']);
      }
      if (!empty($values['dutch']['short_description'])) {
        $detail->setShortDescription($values['dutch']['short_description']);
      }
      if (!empty($values['dutch']['long_description']['value'])) {
        $detail->setLongDescription($values['dutch']['long_description']['value']);
      }

      $detail->setLanguage("nl");
      $details->add($detail);
    }
  }

  // Translations English.
  if (culturefeed_search_get_preferred_language() != 'en') {
    if ($values['english']['language'] || $values['english']['short_description'] || $values['english']['long_description']['value']) {

      $detail = new CultureFeed_Cdb_Data_EventDetail();
      if ($values['english']['language']) {
        $detail->setTitle($values['english']['language']);
      }
      if (!empty($values['english']['short_description'])) {
        $detail->setShortDescription($values['english']['short_description']);
      }
      if (!empty($values['english']['long_description']['value'])) {
        $detail->setLongDescription($values['english']['long_description']['value']);
      }

      $detail->setLanguage("en");
      $details->add($detail);
    }
  }

  // Translations French.
  if (culturefeed_search_get_preferred_language() != 'fr') {
    if ($values['french']['language'] || $values['french']['short_description'] || $values['french']['long_description']['value']) {

      $detail = new CultureFeed_Cdb_Data_EventDetail();
      if ($values['french']['language']) {
        $detail->setTitle($values['french']['language']);
      }
      if (!empty($values['french']['short_description'])) {
        $detail->setShortDescription($values['french']['short_description']);
      }
      if (!empty($values['french']['long_description']['value'])) {
        $detail->setLongDescription($values['french']['long_description']['value']);
      }

      $detail->setLanguage("fr");
      $details->add($detail);
    }
  }

  // Translations German.
  if (culturefeed_search_get_preferred_language() != 'de') {
    if ($values['german']['language'] || $values['german']['short_description'] || $values['german']['long_description']['value']) {

      $detail = new CultureFeed_Cdb_Data_EventDetail();
      if ($values['german']['language']) {
        $detail->setTitle($values['german']['language']);
      }
      if (!empty($values['german']['short_description'])) {
        $detail->setShortDescription($values['german']['short_description']);
      }
      if (!empty($values['german']['long_description']['value'])) {
        $detail->setLongDescription($values['german']['long_description']['value']);
      }

      $detail->setLanguage("de");
      $details->add($detail);
    }
  }

  $event->setDetails($details);

  // Asset Location / Address.
  if ($location) {
    $addresses = $location->getEntity()->getContactInfo()->getAddresses();

    /* @var CultureFeed_Cdb_Data_Address $address */
    $address = $addresses[0];
    $location_detail = $location->getEntity()->getDetails()
      ->getDetailByLanguage(culturefeed_search_get_preferred_language());
    if (!$location_detail) {
      $location_detail = $location->getEntity()->getDetails()
        ->getDetailByLanguage("nl");
    }

    $cdb_location = new CultureFeed_Cdb_Data_Location($address);
    $cdb_location->setLabel($location_detail->getTitle());
    $cdb_location->setCdbid($location->getEntity()->getCdbId());
    $event->setLocation($cdb_location);
  }
  else {
    $address = new CultureFeed_Cdb_Data_Address();
    $physical_address = new CultureFeed_Cdb_Data_Address_PhysicalAddress();
    $physical_address->setStreet($values['location']['new']['street']);
    $physical_address->setHouseNumber($values['location']['new']['house_number']);
    if ($values['location']['new']['citywrapper']['country'] == 'BE') {
      $physical_address->setZip(substr($values['location']['new']['citywrapper']['city'], 0, 4));
      $physical_address->setCity(substr($values['location']['new']['citywrapper']['city'], 5));
    }
    else {
      $physical_address->setCity($values['location']['new']['citywrapper']['city']);
      $physical_address->setZip($values['location']['new']['citywrapper']['zip_code']);
    }
    $physical_address->setCountry($values['location']['new']['citywrapper']['country']);
    $address->setLabel($values['location']['new']['location_title']);
    $address->setPhysicalAddress($physical_address);
    $address->setLabel($values['location']['new']['location_title']);

    $cdb_location = new CultureFeed_Cdb_Data_Location($address);
    $cdb_location->setLabel($values['location']['new']['location_title']);
    $event->setLocation($cdb_location);
  }

  // Event organiser.
  if ($organiser) {
    $organiser_object = new CultureFeed_Cdb_Data_Organiser();
    $organiser_detail = $organiser->getEntity()->getDetails()
      ->getDetailByLanguage(culturefeed_search_get_preferred_language());
    if (!$organiser_detail) {
      $organiser_detail = $organiser->getEntity()->getDetails()
        ->getDetailByLanguage("nl");
    }
    $organiser_object->setLabel($organiser_detail->getTitle());
    $organiser_object->setCdbid($organiser->getEntity()->getCdbId());
    $event->setOrganiser($organiser_object);
  }
  else {
    if (isset($values['organiser']['new_actor'])) {
      $organiser_object = new CultureFeed_Cdb_Data_Organiser();
      $organiser_object->setLabel($values['organiser']['new_actor']);
      $event->setOrganiser($organiser_object);
    }
  }

  // Contact info.
  $physical_address = $address->getPhysicalAddress();
  $contact_object = new CultureFeed_Cdb_Data_ContactInfo();
  $contact_object->addAddress(new CultureFeed_Cdb_Data_Address($physical_address));

  foreach ($mails as $mail) {
    $contact_object->addMail($mail);
  }
  foreach ($phones as $phone) {
    $contact_object->addPhone($phone);
  }
  foreach ($links as $link) {
    $contact_object->addUrl($link);
  }

  $event->setContactInfo($contact_object);

  // Keywords.
  if ($values['keywords']) {
    $keywords = explode(';', $values['keywords']);
    foreach ($keywords as $keyword) {
      $event->addKeyword($keyword);
    }
  }

  // Members
  if ($values['members']) {
    $event->setPrivate(TRUE);
  }
  
  //if ($language_list) {
    $event->setLanguages($language_list);

  //}

  try {
    if ($update) {
      Drupalculturefeed_EntryApi::updateEvent($event);
      $form_state['#event_id'] = $event->getCdbId();
      cache_clear_all('culturefeed:results:detail:event:' . $event->getCdbId(), 'cache_culturefeed_search');
    }
    else {
      $form_state['#event_id'] = Drupalculturefeed_EntryApi::createEvent($event);
    }

    $form_state['#update_event'] = $update;

  } catch (Exception $e) {
    watchdog_exception('culturefeed_entry_ui', $e);
    form_set_error('', t('An error occurred while saving the event'));
  }

  // Delete files from file system 
  if ($form_state['values']['photo']['upload']) {
    $file = file_load($form_state['values']['photo']['upload']);
    file_delete($file);
  }
  if (isset($form_state['values']['photo']['current_file'])) {
    $file = file_load($form_state['values']['photo']['current_file']);
    file_delete($file);
  }

}

/**
 * Save helper function that creates calendar object for timestamps.
 */
function _culturefeed_entry_ui_event_save_timestamps(&$event, $values) {
  $calendar = new CultureFeed_Cdb_Data_Calendar_TimestampList();

  foreach ($values['datetimes'] as $timestamp) {

    // Empty start dates are ignored.
    if (empty($timestamp['begin']['date'])) {
      continue;
    }

    $timestamp_object = new CultureFeed_Cdb_Data_Calendar_Timestamp($timestamp['begin']['date']);
    if (!empty($timestamp['begin']['hour'])) {
      $timestamp_object->setStartTime($timestamp['begin']['hour'] . ':00');
    }
    if (!empty($timestamp['end']['hour'])) {
      $timestamp_object->setEndTime($timestamp['end']['hour'] . ':00');
    }
    $calendar->add($timestamp_object);
  }
  $event->setCalendar($calendar);

}

/**
 * Save helper function that creates calendar object for timestamps.
 */
function _culturefeed_entry_ui_event_save_period(&$event, $values) {
  
  $period_list = new CultureFeed_Cdb_Data_Calendar_PeriodList();
  $period = new CultureFeed_Cdb_Data_Calendar_Period($values['datetimes']['0']['begin']['date'], $values['datetimes']['0']['end']['date']);
  $period_list->add($period);
  $event->setCalendar($period_list);
  
}

/**
 * Save helper function that creates calendar object for weekschemes.
 */
function _culturefeed_entry_ui_event_save_weekscheme(&$event, $values) {

  $weekscheme = NULL;
  // Construct the weekscheme.
  if (isset($values['period_or_permanent']) && !$values['period_or_permanent']['all_day']) {

    $weekscheme = new CultureFeed_Cdb_Data_Calendar_Weekscheme();
    foreach ($values['period_or_permanent']['opening_times'] as $day => $opening_times) {

      $opening_info = new CultureFeed_Cdb_Data_Calendar_SchemeDay($day);
      $open_type = CultureFeed_Cdb_Data_Calendar_SchemeDay::SCHEMEDAY_OPEN_TYPE_CLOSED;
      foreach ($opening_times as $opening_time) {
        if (!empty($opening_time['open_from']) && !empty($opening_time['open_till'])) {
          $open_type = CultureFeed_Cdb_Data_Calendar_SchemeDay::SCHEMEDAY_OPEN_TYPE_OPEN;
          $opening_info->addOpeningTime(new CultureFeed_Cdb_Data_Calendar_OpeningTime($opening_time['open_from'], $opening_time['open_till']));
        }
      }

      $opening_info->setOpenType($open_type);
      $weekscheme->setDay($day, $opening_info);

    }

  }

  if ($values['datetype'] == 'period') {
    $calendar = new CultureFeed_Cdb_Data_Calendar_PeriodList();
    $period = new CultureFeed_Cdb_Data_Calendar_Period($values['datetimes'][0]['begin']['date'], $values['datetimes'][0]['end']['date']);
    if ($weekscheme) {
      $period->setWeekScheme($weekscheme);
    }
    $calendar->add($period);
  }
  else {
    $calendar = new CultureFeed_Cdb_Data_Calendar_Permanent();
    if ($weekscheme) {
      $calendar->setWeekScheme($weekscheme);
    }
  }

  $event->setCalendar($calendar);

}

/**
 * Submit the event form: redirect form and show message.
 */
function culturefeed_entry_ui_event_form_submit($form, &$form_state) {

  if ($form_state['#update_event']) {
    $form_state['redirect'] = 'http://' . $_SERVER['HTTP_HOST'] . culturefeed_search_detail_url('event', $form_state['#event_id'], $form_state['values']['title']);
    drupal_set_message(t('Event successfully updated'));
  }
  else {
    $path = culturefeed_search_detail_url('event', $form_state['#event_id'], $form_state['values']['title']) . '/edit';
    $link_change_event = '<a href="' . $path . '">' . t('Click here to update') . '</a>';
    drupal_set_message(t('Event ' . $form_state['#event_id'] . ' successfully created') . ' ' . $link_change_event);
  }

}

/**
 * Form callback: delete an event.
 */
function culturefeed_entry_ui_delete_event_form($form, $form_state, $event) {

  $event = $event->getEntity();
  $form['id'] = array('#type' => 'value', '#value' => $event->getCdbId());

  return confirm_form($form,
    t('Are you sure you want to delete the event?'),
    '',
    t('This action cannot be undone'),
    t('Delete'),
    t('Cancel')
  );

}

/**
 * Submit the delete form.
 */
function culturefeed_entry_ui_delete_event_form_submit($form, &$form_state) {

  try {
    Drupalculturefeed_EntryApi::deleteEvent($form_state['values']['id']);
    drupal_set_message(t('Event successfully deleted'));
    $form_state['redirect'] = '<front>';
  } catch (Exception $e) {
    watchdog_exception(WATCHDOG_ERROR, $e);
    drupal_set_message(t('An error occurred while deleting the event'), 'error');
  }

}

/**
 * Form callback: Show the manage tags form for a cdb object.
 */
function culturefeed_entry_ui_edit_tags_form($form, $form_state, $event) {

  $form = array();
  $event = $event->getEntity();
  $event_tags = $event->getKeywords();
  $form['#old_tags'] = $event_tags;

  $custom_tags = array();

  // Get options from variable.
  if (variable_get('culturefeed_entry_ui_custom_tags')) {
    $custom_options = explode("\n", variable_get('culturefeed_entry_ui_custom_tags'));
  }
  
  // Filter out custom tags.
  if ($event_tags) {
    foreach ($event_tags as $tag) {
      if ($custom_options && array_key_exists($tag, $custom_options)) {
        unset($event_tags[$tag]);
        $custom_tags[$tag] = $tag;
      }
    }
  }
  else {
    $event_tags = array();
  }

  $form['#object_id'] = $event->getCdbId();

  if (user_access('manage custom tags') && !empty($custom_options)) {
    $form['custom_tags'] = array(
      '#type' => 'checkboxes',
      '#options' => $custom_options,
      '#title' => t('Custom tags'),
      '#default_value' => $custom_tags,
    );
  }

  $form['tags'] = array(
    '#type' => 'textfield',
    '#title' => t('Tags'),
    '#default_value' => implode(';', $event_tags),
    '#description' => t('Seperate multiple tags by ;'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;

}

/**
 * Validate the tag form. Check duplicates with custom tags.
 */
function culturefeed_entry_ui_edit_tags_form_validate($form, &$form_state) {

  $tags = explode(';', $form_state['values']['tags']);
  foreach ($tags as $tag) {
    if (array_key_exists($tag, $form['custom_tags']['#options'])) {
      form_set_error(t("Tag @tag is not permitted because it's already a custom tag.", array('@tag' => $tag)));
    }
  }

}

/**
 * Submit the edit tags form.
 */
function culturefeed_entry_ui_edit_tags_form_submit($form, &$form_state) {

  // Merge custom tags into all tags.
  $tags = explode(';', $form_state['values']['tags']);
  foreach ($form_state['values']['custom_tags'] as $custom_tag) {
    if ($custom_tag) {
      $tags[] = $custom_tag;
    }
  }

  // Check what tags are removed.
  $removed_tags = array();
  if (!empty($form['#old_tags'])) {
    foreach ($form['#old_tags'] as $old_tag) {
      if (!in_array($old_tag, $tags)) {
        $removed_tags[] = $old_tag;
      }
    }
  }

  $event = new CultureFeed_Cdb_Item_Event();
  $event->setCdbId($form['#object_id']);

  try {

    // Add new tags.
    $tags = array_filter($tags);
    if ($tags) {
      Drupalculturefeed_EntryApi::addTagToEvent($event, $tags);
    }

    // Remove old tags.
    if ($removed_tags) {
      foreach ($removed_tags as $removed_tag) {
        Drupalculturefeed_EntryApi::removeTagFromEvent($event, $removed_tag);
      }
    }

    drupal_set_message("De tags werden aangepast. Het duurt echter een half uur eer deze
      aanpassingen op al onze kanalen (inclusief UitinVlaanderen onder de tab 'Weergeven'
      beschikbaar zullen zijn.");

  } catch (Exception $e) {

    if ($e->getCode() == culturefeed_entry_uiApi::CODE_KEYWORD_PRIVATE) {
      drupal_set_message("Je probeerde een tag toe te voegen die niet door iedereen mag
        worden toegevoegd. Indien je dit toch wil toevoegen of andere vragen hier rond
        hebt, mail je best naar 'vragen@uitdatabank.be'");
    }
    else {
      drupal_set_message('Er ging iets fout tijdens het bewaren.');
    }

    watchdog_exception(WATCHDOG_ERROR, $e);

  }

}
