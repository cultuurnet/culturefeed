<?php
/**
 * @file
 * Page callbacks for the entry api ui.
 */

/**
 * Form callback: Create a new event, or edit an existing.
 */
function culturefeed_entry_ui_event_form($form, &$form_state, $event = NULL) {

  $form = array();

  $default_contacts = array();
  $performers_info = array();
  $languages_info = array();
  $category_defaults = array();
  $eventtype_default = '';
  $themetype_default = '';
  $default_organiser_label = '';
  $default_organiser_id = '';
  $publication_date = '';
  $timestamps = array();
  $period_start = '';
  $period_end = '';
  $permanent = FALSE;
  $weekscheme = NULL;
  $age = '';
  $age_category = 'everyone';
  $default_short_description = '';
  $default_long_description = '';
  $default_image = '';
  $default_links = array();
  $default_links_reservation = array();
  $phone_contacts = array();
  $mail_contacts = array();
  $is_private = FALSE;

  if (!isset($form_state['#themes_needed'])) {
    $form_state['#themes_needed'] = '';
  }

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

        if ($category->getType() == 'theme') {
          $themetype_default = $category->getId();
        }

      }
    }

    // Default organiser.
    if ($organiser = $event->getOrganiser()) {
      $default_organiser_label = $organiser->getLabel();
      $default_organiser_id = $organiser->getCdbId();
    }

    // Default calendar type, start / end date, weekscheme data.
    if ($calendar = $event->getCalendar()) {

      $calendar_class = get_class($calendar);
      $calendar->rewind();
      $today = strtotime(date('Y-m-d') . ' 00:00:00');

      if ($calendar_class == 'CultureFeed_Cdb_Data_Calendar_PeriodList') {
        $period_start = $calendar->current()->getDateFrom();
        $period_end = $calendar->current()->getDateTo();
        $weekscheme = $calendar->current()->getWeekScheme();
      }
      elseif ($calendar_class == 'CultureFeed_Cdb_Data_Calendar_Permanent') {
        $permanent = TRUE;
        $weekscheme = $calendar->getWeekScheme();
      }
      elseif ($calendar_class == 'CultureFeed_Cdb_Data_Calendar_TimestampList') {

        $timestamps_count = iterator_count($calendar);
        $calendar->rewind();
        $current_timestamp = 0;
        for ($i = 0; $i < $timestamps_count; $i++) {

          $timestamp = $calendar->current();
          $date = $timestamp->getDate();

          if (strtotime($date) >= $today) {
            $timestamps[$current_timestamp] = array(
              'date' => $date,
              'start_time' => $timestamp->getStartTime(),
              'end_time' => $timestamp->getEndTime(),
            );
            $current_timestamp++;
          }

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
      $source_text = '/<p class="uiv-source">(.*)<\/p>/';
      $default_long_description = preg_replace($source_text, '', $default_long_description);
      $allowed_tags = array('a', 'em', 'strong', 'ul', 'ol', 'li', 'hr');
      $default_long_description = filter_xss($default_long_description, $allowed_tags);
      //$default_long_description = substr($default_long_description, 0, -169);
      $form_state['#ld_needed'] = TRUE;
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
            CultureFeed_Cdb_Data_File::MEDIA_TYPE_WEBSITE,
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
      ->byMediaTypes(array(
          CultureFeed_Cdb_Data_File::MEDIA_TYPE_IMAGEWEB,
          CultureFeed_Cdb_Data_File::MEDIA_TYPE_PHOTO,
        )
      );

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
        $default_image = system_retrieve_file('http:' . $main_image->getHLink(), 'public://uploads/', TRUE);

        if ($default_image) {
          file_usage_add($default_image, 'culturefeed_entry_ui', 'event', $default_image->fid);
          $default_image_hlink = $main_image->getHLink();
          $default_copyright = $main_image->getCopyright();
        }
      } catch (Exception $e) {
        watchdog_exception('culturefeed_entry_ui', $e);
      }

    }

    // Default price
    if ($info = $event_details->getPrice()) {
      $default_price = str_replace('.',',', $info->getValue());
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
        $age_category = '18+';
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

    if ($info = $event->getLanguages()) {
      foreach ($info as $language) {
        $languages_info[] = array(
          $language->getType(),
          $language->getLanguage(),
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
    */
  }

  // Build the form.

  // Basic.
  $form['basic'] = array(
    '#type' => 'fieldset',
  );

  $form['basic']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Event title'),
    '#default_value' => ($event) ? $event->getDetails()
        ->getDetailByLanguage(culturefeed_search_get_preferred_language())
        ->getTitle() : NULL,
    '#required' => TRUE,
  );

  // Eventtypes
  $form['basic']['what'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="what-ajax-wrapper" class="clearfix">',
    '#suffix' => '</div>'
  );

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
      $form['basic']['what'][$cluster_id][$type_id] = array(
        '#type' => 'radio',
        '#title' => $type_name,
        '#return_value' => $type_id,
        '#default_value' => $eventtype_default,
        '#parents' => array('what'),
      );
    }
  }

  $form['basic']['what']['themes'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="themes-ajax-wrapper">',
    '#suffix' => '</div>',
    '#tree' => TRUE
  );

  $themesconcerts = culturefeed_search_get_theme_categories(array('tid_like' => 1.8));
  $form['basic']['what']['themes']['concerts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Concerts'),
    '#states' => array(
      'visible' => array(
        'input[name="what"]' => array('value' => '0.50.4.0.0'),
      ),
    ),
  );

  foreach ($themesconcerts as $theme_id => $theme_name) {
    $form['basic']['what']['themes']['concerts'][$theme_name] = array(
      '#type' => 'radio',
      '#title' => $theme_name,
      '#return_value' => $theme_id,
      '#default_value' => $themetype_default,
      '#parents' => array('themes'),
    );
  }

  $themesdance = culturefeed_search_get_theme_categories(array('tid_like' => 1.9));
  $form['basic']['what']['themes']['dance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Dance'),
    '#states' => array(
      'visible' => array(
        'input[name="what"]' => array('value' => '0.54.0.0.0'),
      ),
    ),
  );

  foreach ($themesdance as $theme_id => $theme_name) {
    $form['basic']['what']['themes']['dance'][$theme_name] = array(
      '#type' => 'radio',
      '#title' => $theme_name,
      '#return_value' => $theme_id,
      '#default_value' => $themetype_default,
      '#parents' => array('themes'),
    );
  }

  $themesfilm = culturefeed_search_get_theme_categories(array('tid_like' => 1.7));
  $form['basic']['what']['themes']['film'] = array(
    '#type' => 'fieldset',
    '#title' => t('Film'),
    '#states' => array(
      'visible' => array(
        'input[name="what"]' => array('value' => '0.50.6.0.0'),
      ),
    ),
  );

  foreach ($themesfilm as $theme_id => $theme_name) {
    $form['basic']['what']['themes']['film'][$theme_name] = array(
      '#type' => 'radio',
      '#title' => $theme_name,
      '#return_value' => $theme_id,
      '#default_value' => $themetype_default,
      '#parents' => array('themes'),
    );
  }

  $themestheatre = culturefeed_search_get_theme_categories(array('tid_like' => '1.3.'));
  $themestheatre = array_merge($themestheatre, culturefeed_search_get_theme_categories(array('tid_like' => '0.52.0')));
  $themestheatre = array_merge($themestheatre, culturefeed_search_get_theme_categories(array('tid_like' => '1.4.')));
  $themestheatre = array_merge($themestheatre, culturefeed_search_get_theme_categories(array('tid_like' => '1.5.')));
  $form['basic']['what']['themes']['theatre'] = array(
    '#type' => 'fieldset',
    '#title' => t('Theatre'),
    '#states' => array(
      'visible' => array(
        'input[name="what"]' => array('value' => '0.55.0.0.0'),
      ),
    ),
  );

  foreach ($themestheatre as $theme_id => $theme_name) {
    $form['basic']['what']['themes']['theatre'][$theme_name] = array(
      '#type' => 'radio',
      '#title' => $theme_name,
      '#return_value' => $theme_id,
      '#default_value' => $themetype_default,
      '#parents' => array('themes'),
    );
  }

  $themessport = culturefeed_search_get_theme_categories(array('tid_like' => '1.51'));
  $themessport = array_merge($themessport, culturefeed_search_get_theme_categories(array('tid_like' => '1.58.8')));
  $form['basic']['what']['themes']['sport'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sport'),
    '#states' => array(
      'visible' => array(
        'input[name="what"]' => array('value' => '0.19.0.0.0'),
      ),
    ),
  );

  foreach ($themessport as $theme_id => $theme_name) {
    $form['basic']['what']['themes']['sport'][$theme_name] = array(
      '#type' => 'radio',
      '#title' => $theme_name,
      '#return_value' => $theme_id,
      '#default_value' => $themetype_default,
      '#parents' => array('themes'),
    );
  }

  // Location control.
  form_load_include($form_state, 'inc', 'culturefeed_entry_ui', 'includes/form/culturefeed_entry_ui.location');
  culturefeed_entry_ui_location_form($form, $form_state, $event);

  // Date control
  $form['basic']['when'] = array(
    '#title' => t('Dates'),
    '#type' => 'culturefeed_date_control',
    '#default_value' => array(
      'period' => array(
        'start_date' => $period_start,
        'end_date' => $period_end,
      ),
      'permanent' => $permanent,
      'timestamps' => $timestamps,
      'opening_times' => array('days' => $weekscheme),
    ),
    '#allowed_days' => CultureFeed_Cdb_Data_Calendar_SchemeDay::$allowedDays,
  );

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
    '#maxlength' => 400,
    '#description' => t('Maximum 400 characters'),
    '#default_value' => $default_short_description,
    '#element_validate' => array('culturefeed_entry_ui_validate_textarea_markup'),
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
        'effect' => 'none',
      ),
      '#limit_validation_errors' => array(array('ld')),
      '#submit' => array('culturefeed_entry_ui_add_more_submit'),
    );
  }

  if (isset($form_state['#ld_needed'])) {
    $form['extra']['description']['ld']['long_description'] = array(
      '#type' => 'textarea',
      '#title' => t('Long description'),
      '#rows' => 10,
      '#default_value' => $default_long_description,
      '#element_validate' => array('culturefeed_entry_ui_validate_textarea_markup'),
    );
  }

  // Tags
  form_load_include($form_state, 'inc', 'culturefeed_entry_ui', 'includes/form/culturefeed_entry_ui.tags');
  culturefeed_entry_ui_tags_form($form, $form_state, $event);

  // Links
  $form['extra']['links'] = array(
    '#tree' => TRUE,
    '#type' => 'container',
    '#prefix' => '<div id="links-ajax-wrapper">',
    '#suffix' => '</div>',
    '#title' => t('Links'),
  );

  if ($default_links) {
    $links_count = count($default_links);
  }
  else {
    $links_count = 0;
  }
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
      'effect' => 'none',
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
    '#after_build' => array('culturefeed_entry_ui_photo_after_build'),
    '#default_image' => isset($default_image->fid) ? $default_image : FALSE,
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

  // Copyright
  $form['extra']['photo']['copyright_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Copyright'),
    '#description' => t('Name of the copyright owner of this image.'),
    '#default_value' => isset($default_copyright) ? $default_copyright : '',
    '#size' => 30,
  );
  $form['extra']['photo']['copyright'] = array(
    '#type' => 'checkbox',
    '#default_value' => isset($default_image->fid) ? 1 : 0,
    '#title' => t('I agree to the <a href="http://www.uitdatabank.be/uitportal/legal.aspx">terms & conditions</a> and I declare that I have the necessary rights or permissions to distribute the image.'),
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
      '#value' => t('+ add conditions & discounts'),
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
        'effect' => 'none',
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
      '#title' => t('Price conditions & discounts'),
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
      '#title' => t('Organizer'),
      '#autocomplete_path' => 'autocomplete/culturefeed/actor-suggestions/all',
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
    '#value' => t('Organizer not found, add organizer'),
    '#attributes' => array(
      'class' => array('add-more-link btn-link'),
      'style' => array('display:none'),
    ),
    '#ajax' => array(
      'callback' => 'culturefeed_entry_ui_add_more_js',
      'wrapper' => 'organiser-ajax-wrapper',
      'effect' => 'none',
    ),
    '#limit_validation_errors' => array(array('price')),
    '#submit' => array('culturefeed_entry_ui_add_more_submit'),
  );

  if (isset($form_state['#actor_needed'])) {
    $default_actor = isset($form_state['#actor_label']) ? $form_state['#actor_label'] : '';
    $form['extra']['organiser']['new_actor'] = array(
      '#type' => 'textfield',
      '#title' => t('Name new organizer'),
      '#default_value' => $default_actor,
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
    '#title' => t('Suitable for ages from'),
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
    '#title' => t('Contact'),
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
    '#value' => t('+ add contact option'),
    '#attributes' => array('class' => array('add-more-link btn-link')),
    '#ajax' => array(
      'callback' => 'culturefeed_entry_ui_add_more_js',
      'wrapper' => 'contacts-ajax-wrapper',
      'effect' => 'none',
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
      'effect' => 'none',
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
  if ($languages_info) {
    foreach ($languages_info as $i => $info) {
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
      'effect' => 'none',
    ),
    '#limit_validation_errors' => array(array('languages')),
    '#submit' => array('culturefeed_entry_ui_add_more_submit'),
  );

  //entrance
  //$form['extra']['target_group'] = array(
  $form['extra']['entrance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Access'),
    '#group' => 'vertical_tabs',
  );

  $form['extra']['entrance']['members'] = array(
    '#type' => 'radios',
    '#options' => array(0 => t('Open for everyone'), 1 => t('Only for members')),
    '#default_value' => ($is_private) ? 1 : 0,
  );

  // translations
  $form['extra']['translations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Translations'),
    '#group' => 'vertical_tabs',
  );

  // Dutch.
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
      '#title' => t('Event title'),
      '#default_value' => $nl_title,
    );

    $form['extra']['translations']['dutch']['short_description'] = array(
      '#type' => 'textarea',
      '#title' => t('Short description'),
      '#default_value' => $nl_short_description,
      '#element_validate' => array('culturefeed_entry_ui_validate_textarea_markup'),
    );

    $form['extra']['translations']['dutch']['long_description'] = array(
      '#type' => 'textarea',
      '#title' => t('Long description'),
      '#default_value' => $nl_long_description,
      '#element_validate' => array('culturefeed_entry_ui_validate_textarea_markup'),
    );
  }

  // English.
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
      '#title' => t('Event title'),
      '#default_value' => $en_title,
    );

    $form['extra']['translations']['english']['short_description'] = array(
      '#type' => 'textarea',
      '#title' => t('Short description'),
      '#default_value' => $en_short_description,
      '#element_validate' => array('culturefeed_entry_ui_validate_textarea_markup'),
    );

    $form['extra']['translations']['english']['long_description'] = array(
      '#type' => 'textarea',
      '#title' => t('Long description'),
      '#default_value' => $en_long_description,
      '#element_validate' => array('culturefeed_entry_ui_validate_textarea_markup'),
    );
  }

  // French.
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
      '#title' => t('Event title'),
      '#default_value' => $fr_title,
    );

    $form['extra']['translations']['french']['short_description'] = array(
      '#type' => 'textarea',
      '#title' => t('Short description'),
      '#default_value' => $fr_short_description,
      '#element_validate' => array('culturefeed_entry_ui_validate_textarea_markup'),
    );

    $form['extra']['translations']['french']['long_description'] = array(
      '#type' => 'textarea',
      '#title' => t('Long description'),
      '#default_value' => $fr_long_description,
      '#element_validate' => array('culturefeed_entry_ui_validate_textarea_markup'),
    );
  }

  // German.
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
      '#title' => t('Event title'),
      '#default_value' => $de_title,
    );

    $form['extra']['translations']['german']['short_description'] = array(
      '#type' => 'textarea',
      '#title' => t('Short description'),
      '#default_value' => $de_short_description,
      '#element_validate' => array('culturefeed_entry_ui_validate_textarea_markup'),
    );

    $form['extra']['translations']['german']['long_description'] = array(
      '#type' => 'textarea',
      '#title' => t('Long description'),
      '#default_value' => $de_long_description,
      '#element_validate' => array('culturefeed_entry_ui_validate_textarea_markup'),
    );
  }
  //Publication & Private
  $form['extra']['publication_date'] = array(
    '#type' => 'fieldset',
    '#title' => t('Publication date'),
    '#group' => 'vertical_tabs',
  );

  $form['extra']['publication_date']['wrapper'] = array(
    '#tree' => TRUE,
    '#type' => 'container',
    '#prefix' => '<div id="pub-date-ajax-wrapper">',
    '#suffix' => '</div>',
  );

  $form['extra']['publication_date']['wrapper']['date'] = array(
    '#type' => 'date_popup',
    '#default_value' => ($publication_date == '') ? date('Y-m-d') : $publication_date,
    '#date_format' => 'Y-m-d',
  );

  /*$form['extra']['publication_date']['wrapper']['private'] = array(
    '#type' => 'checkbox',
    '#title' => t('Public'),
  );*/

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#validate' => array('culturefeed_entry_ui_event_save_validate'),
  );

  $form['#attached']['css'][] = drupal_get_path('module', 'culturefeed_entry_ui') . '/css/culturefeed_entry_ui.css';
  $form['#attached']['js'][] = drupal_get_path('module', 'culturefeed_entry_ui') . '/js/culturefeed_entry_ui.js';
  $form['#attached']['js'][] = drupal_get_path('module', 'culturefeed_entry_ui') . '/js/maxlength/jquery.plugin.js';
  $form['#attached']['js'][] = drupal_get_path('module', 'culturefeed_entry_ui') . '/js/maxlength/jquery.maxlength.js';
  if (culturefeed_search_get_preferred_language() == 'nl') {
    $form['#attached']['js'][] = drupal_get_path('module', 'culturefeed_entry_ui') . '/js/maxlength/jquery.maxlength-nl.js';
  }
  elseif (culturefeed_search_get_preferred_language() == 'fr') {
    $form['#attached']['js'][] = drupal_get_path('module', 'culturefeed_entry_ui') . '/js/maxlength/jquery.maxlength-fr.js';
  }
  elseif (culturefeed_search_get_preferred_language() == 'de') {
    $form['#attached']['js'][] = drupal_get_path('module', 'culturefeed_entry_ui') . '/js/maxlength/jquery.maxlength-de.js';
  }
  return $form;

}

/**
 * Attach a new language element.
 *
 */
function culturefeed_entry_ui_attach_language_elements(&$form, $i, $info = NULL) {
  if ($info[0] == 'spoken') {
    $medium = 1;
  }
  else {
    if ($info[0] == 'dubbed') {
      $medium = 2;
    }
    else {
      if ($info[0] == 'subtitles') {
        $medium = 3;
      }
      else {
        $medium = 0;
      }
    }
  }

  if ($info[1] == 'Engels') {
    $lang = 1;
  }
  else {
    if ($info[1] == 'Spaans') {
      $lang = 2;
    }
    else {
      if ($info[1] == 'Nederlands') {
        $lang = 3;
      }
      else {
        if ($info[1] == 'Duits') {
          $lang = 4;
        }
        else {
          if ($info[1] == 'Frans') {
            $lang = 5;
          }
          else {
            $lang = 0;
          }
        }
      }
    }
  }

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
      0 => '',
      1 => t('Spoken'),
      2 => t('Dubbed'),
      3 => t('Subtitled'),
    ),
    '#default_value' => $medium,
  );

  $form['extra']['language']['wrapper'][$i]['in_language'] = array(
    '#type' => 'select',
    '#title' => t('in'),
    '#options' => array(
      0 => '',
      1 => t('English'),
      //2 => t('Spanish'),
      3 => t('Dutch'),
      4 => t('German'),
      5 => t('French'),
    ),
    '#default_value' => $lang,
  );
}

/**
 * Attach a new contact element.
 *
 */
function culturefeed_entry_ui_attach_contacts_elements(&$form, $i, $info = NULL) {

  $options = array(0 => t('Phone'), 1 => t('E-mail'));
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
 * Validate the event form.
 */
function culturefeed_entry_ui_event_save_validate($form, &$form_state) {

  $values = $form_state['values'];

  // Validate age.
  if ($values['age'] && !is_numeric($values['age'])) {
    form_set_error('age', t('Please fill in correct age'));
  }

  // Validate period.
  if ($values['when']['date_control']['type'] == 'period') {

    $period = $values['when']['date_control']['period'];

    // Form validation hasn't processed the dates values yet, so don't validate.
    if (!is_array($period['start_date']) && !is_array($period['end_date'])) {

      // The dates.
      try {
        CultureFeed_Cdb_Data_Calendar::validateDate($period['start_date']);
        CultureFeed_Cdb_Data_Calendar::validateDate($period['end_date']);
      }
      catch (UnexpectedValueException $e) {
        form_set_error('when][date_control][period', t('Please fill in correct date and time'));
      }

      // Order of dates.
      $start_date = strtotime($period['start_date']);
      $end_date = strtotime($period['end_date']);
      if ($start_date > $end_date) {
        form_set_error('when][date_control][period', t('Date field is invalid. The end date is earlier than the beginning date.'));
      }

    }

  }

  // Validate timestamps.
  if ($values['when']['date_control']['type'] == 'timestamps') {

    $timestamps = $values['when']['date_control']['timestamps']['stamps'];

    foreach ($timestamps as $key => $timestamp) {

      // Form validation hasn't processed the dates values yet, so don't validate.
      if (is_array($timestamp['date'])) {
        continue;
      }

      // The date.
      try {
        CultureFeed_Cdb_Data_Calendar::validateDate($timestamp['date']);
      } catch (UnexpectedValueException $e) {
        form_set_error('when][date_control][timestamps][stamps][' . $key, t('Please fill in correct date and time'));
      }

      // The times.
      $start_time = isset($timestamp['start_time']) ? strtotime($timestamp['start_time']) : '';
      $end_time = isset($timestamp['end_time']) ? strtotime($timestamp['end_time']) : '';
      if (($start_time && $end_time) && $start_time > $end_time) {
        form_set_error('when][date_control][timestamps][stamps][' . $key, t('Date field is invalid. The end time is earlier than the beginning time.'));
      }

    }

  }

  // Validate weekscheme.
  if ($values['when']['date_control']['type'] == 'period' || $values['when']['date_control']['type'] == 'permanent') {

    $opening_times = $values['when']['date_control']['opening_times'];
    // Validate the weekscheme.
    if (!$opening_times['all_day']) {

      foreach ($opening_times['days'] as $day => $opening_times) {

        foreach ($opening_times as $key => $opening_time) {

          if (!empty($opening_time['open_from']) || !empty($opening_time['open_till'])) {

            $parents = array('when', 'date_control', 'opening_times', 'days', $day, $key);
            if (substr_count($opening_time['open_from'], ':') == 1) {
              $opening_time['open_from'] .= ':00';
              form_set_value(array('#parents' => array_merge($parents, array('open_from'))), $opening_time['open_from'], $form_state);
            }
            if (substr_count($opening_time['open_till'], ':') == 1) {
              $opening_time['open_till'] .= ':00';
              form_set_value(array('#parents' => array_merge($parents, array('open_till'))), $opening_time['open_till'], $form_state);
            }

            try {
              CultureFeed_Cdb_Data_Calendar::validateTime($opening_time['open_from']);
            }
            catch (Exception $e) {
              form_set_error('when][date_control][opening_times][days][' . $day . '][' . $key . '][open_from', t('Please fill in correct time.'));
            }

            try {
              CultureFeed_Cdb_Data_Calendar::validateTime($opening_time['open_till']);
            }
            catch (Exception $e) {
              form_set_error('when][date_control][opening_times][days][' . $day . '][' . $key . '][open_till', t('Please fill in correct time.'));
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
  if (!empty($values['photo']['upload']) && empty($values['photo']['copyright_text'])) {
    form_set_error('photo', t('Copyright field is required.'));
  }

  // Validate categories
  if (empty($values['what'])) {
    form_set_error('what', t('A category is required.'));
  }

  // Validate location
  $location = culturefeed_entry_ui_location_form_validate($form, $form_state);

  // Validate Links
  $i = 0;
  foreach ($values['links'] as $link_data) {
    if (!empty($link_data['URL'])) {
      if (!preg_match("@^https?://@", $link_data['URL'])) {
        $link_data['URL'] = 'http://' . $link_data['URL'];
      }
      if (!valid_url($link_data['URL'], TRUE)) {
        form_set_error('links][' . $i . '][URL', t('Not a valid URL'));
      }
    }
    $i++;
  }

  // Validate organiser.
  if (!empty($values['organiser']['actor']['organiser_actor_id'])) {

    try {
      $organiser = culturefeed_search_item_load($values['organiser']['actor']['organiser_actor_id'], 'actor');
      if (!$organiser) {
        form_set_error('organiser', t('We could not validate the organizer'));
      }
    } catch (Exception $e) {
      watchdog_exception('culturefeed_entry_ui', $e);
      form_set_error('organiser', t('We could not validate the organizer'));
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
        if ($extra['channel'] == 1) {
          if (!valid_email_address($extra['channel_input'])) {
            form_set_error('channel_input', t('Not a valid email address'));
          }
        }
      }
    }
  }

  // Validate translations
  $available_translations = array(
    'de' => 'german',
    'en' => 'english',
    'fr' => 'french',
    'nl' => 'dutch',
  );

  foreach ($available_translations as $key => $language) {
    if (culturefeed_search_get_preferred_language() != $key) {
      if ($values[$language]['short_description'] || $values[$language]['long_description']) {
        if (!$values[$language]['language']) {
          form_set_error('channel_input', t('If you translate something, you should also translate the title.'));
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
            if ($extra['channel_input']) {
              $mail = new CultureFeed_Cdb_Data_Mail($extra['channel_input'], FALSE, FALSE);
              array_push($mails, $mail);
            }
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
            $in_language = 'Spaans'; //Zit niet in UDB, dus werkt niet
            break;
          case 3:
            $in_language = 'Nederlands';
            break;
          case 4:
            $in_language = 'Duits';
            break;
          case 5:
            $in_language = 'Frans';
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

      if (strpos($link_data['URL'], 'plus.google.com')) {
        $mediatype = CultureFeed_Cdb_Data_File::MEDIA_TYPE_GOOGLEPLUS;
      }

      if (strpos($link_data['URL'], 'facebook.com')) {
        $mediatype = CultureFeed_Cdb_Data_File::MEDIA_TYPE_FACEBOOK;
      }

      if (strpos($link_data['URL'], 'twitter.com')) {
        $mediatype = CultureFeed_Cdb_Data_File::MEDIA_TYPE_TWITTER;
      }

      if (strpos($link_data['URL'], 'youtube.com') || strpos($link_data['URL'], 'youtu.be')) {
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
    $event->setAvailableFrom($values['wrapper']['date'] . 'T00:00:00');
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

  // Timestamps.
  if ($values['when']['date_control']['type'] == 'timestamps') {
    _culturefeed_entry_ui_event_save_timestamps($event, $values['when']['date_control']['timestamps']['stamps']);
  }

  // Period.
  if ($values['when']['date_control']['type'] == 'period') {
    _culturefeed_entry_ui_event_save_period($event, $values['when']['date_control']['period']);
  }

  // Weekscheme.
  if ($values['when']['date_control']['type'] == 'period' || $values['when']['date_control']['type'] == 'permanent') {
    _culturefeed_entry_ui_event_save_weekscheme($event, $values['when']['date_control']);
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

  if (isset($values['themes'])) {
    $theme_options[$values['themes']] = culturefeed_search_get_theme_categories(array('tid_like' => $values['themes']));
    foreach ($theme_options as $key => $value) {
      if ($value && isset($value[$key])) {
        $categories->add(new CultureFeed_Cdb_Data_Category(CultureFeed_Cdb_Data_Category::CATEGORY_TYPE_EVENT_TYPE, $key, $value[$key]));
      }
    }
  }

  $event->setCategories($categories);

  // Event details.
  $detail = new CultureFeed_Cdb_Data_EventDetail();
  $detail->setTitle($values['title']);

  if (!empty($values['description']['sd']['short_description'])) {
    $detail->setShortDescription($values['description']['sd']['short_description']);
  }
  if (!empty($values['description']['ld']['long_description'])) {
    $detail->setLongDescription($values['description']['ld']['long_description']);
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
    $file->setMediaType($file::MEDIA_TYPE_PHOTO);
    $file->setCopyright($values['photo']['copyright_text']);
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
      $price = new CultureFeed_Cdb_Data_Price(floatval(str_replace(',','.', $values['price']['amount'])));
      if (!empty($values['price']['extra']['extra_info'])) {
        $price->setDescription($values['price']['extra']['extra_info']);
      }
      $detail->setPrice($price);
    }
  }

  // Performers
  if ($performers_count) {
    $detail->setPerformers($performerList);
  }

  $detail->setLanguage(culturefeed_entry_ui_get_preferred_language());

  $details = new CultureFeed_Cdb_Data_EventDetailList();
  $details->add($detail);

  // Translations Dutch.
  if (culturefeed_search_get_preferred_language() != 'nl') {
    if ($values['dutch']['language'] || $values['dutch']['short_description'] || $values['dutch']['long_description']) {

      $detail = new CultureFeed_Cdb_Data_EventDetail();
      if ($values['dutch']['language']) {
        $detail->setTitle($values['dutch']['language']);
      }
      if (!empty($values['dutch']['short_description'])) {
        $detail->setShortDescription($values['dutch']['short_description']);
      }
      if (!empty($values['dutch']['long_description'])) {
        $detail->setLongDescription($values['dutch']['long_description']);
      }

      $detail->setLanguage("nl");
      $details->add($detail);
    }
  }

  // Translations English.
  if (culturefeed_search_get_preferred_language() != 'en') {
    if ($values['english']['language'] || $values['english']['short_description'] || $values['english']['long_description']) {

      $detail = new CultureFeed_Cdb_Data_EventDetail();
      if ($values['english']['language']) {
        $detail->setTitle($values['english']['language']);
      }
      if (!empty($values['english']['short_description'])) {
        $detail->setShortDescription($values['english']['short_description']);
      }
      if (!empty($values['english']['long_description'])) {
        $detail->setLongDescription($values['english']['long_description']);
      }

      $detail->setLanguage("en");
      $details->add($detail);
    }
  }

  // Translations French.
  if (culturefeed_search_get_preferred_language() != 'fr') {
    if ($values['french']['language'] || $values['french']['short_description'] || $values['french']['long_description']) {

      $detail = new CultureFeed_Cdb_Data_EventDetail();
      if ($values['french']['language']) {
        $detail->setTitle($values['french']['language']);
      }
      if (!empty($values['french']['short_description'])) {
        $detail->setShortDescription($values['french']['short_description']);
      }
      if (!empty($values['french']['long_description'])) {
        $detail->setLongDescription($values['french']['long_description']);
      }

      $detail->setLanguage("fr");
      $details->add($detail);
    }
  }

  // Translations German.
  if (culturefeed_search_get_preferred_language() != 'de') {
    if ($values['german']['language'] || $values['german']['short_description'] || $values['german']['long_description']) {

      $detail = new CultureFeed_Cdb_Data_EventDetail();
      if ($values['german']['language']) {
        $detail->setTitle($values['german']['language']);
      }
      if (!empty($values['german']['short_description'])) {
        $detail->setShortDescription($values['german']['short_description']);
      }
      if (!empty($values['german']['long_description'])) {
        $detail->setLongDescription($values['german']['long_description']);
      }

      $detail->setLanguage("de");
      $details->add($detail);
    }
  }

  $event->setDetails($details);

  // Location.
  $address = culturefeed_entry_ui_location_form_save($event, $location, $form_state);

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
  culturefeed_entry_ui_tags_form_save($event, $form_state);

  // Members.
  if ($values['members']) {
    $event->setPrivate(TRUE);
  }

  //if ($language_list) {
  $event->setLanguages($language_list);

  //}

  $form_state['submit_time'] = time();

  // Allow alterations by other modules.
  drupal_alter('culturefeed_entry_ui_event_pre_save', $event, $form, $form_state);

  try {
    if ($update) {
      Drupalculturefeed_EntryApi::updateEvent($event);
      $form_state['#event_id'] = $event->getCdbId();
      watchdog('culturefeed_entry_ui', 'Event %eventid updated.', array('%eventid' => $form_state['#event_id']));
      cache_clear_all('culturefeed:results:detail:event:' . $event->getCdbId(), 'cache_culturefeed_search');
    }
    else {
      $form_state['#event_id'] = Drupalculturefeed_EntryApi::createEvent($event);
      watchdog('culturefeed_entry_ui', 'Event %eventid created.', array('%eventid' => $form_state['#event_id']));
    }

    module_invoke_all('culturefeed_entry_ui_event_post_save', $event, $form, $form_state);
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
function _culturefeed_entry_ui_event_save_timestamps(&$event, $timestamps) {

  $calendar = new CultureFeed_Cdb_Data_Calendar_TimestampList();

  foreach ($timestamps as $timestamp) {

    $timestamp_object = new CultureFeed_Cdb_Data_Calendar_Timestamp($timestamp['date']);
    if (!empty($timestamp['start_time'])) {
      $timestamp_object->setStartTime($timestamp['start_time'] . ':00');
    }
    if (!empty($timestamp['end_time'])) {
      $timestamp_object->setEndTime($timestamp['end_time'] . ':00');
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
  $period = new CultureFeed_Cdb_Data_Calendar_Period($values['start_date'], $values['end_date']);
  $period_list->add($period);
  $event->setCalendar($period_list);

}

/**
 * Save helper function that creates calendar object for weekschemes.
 */
function _culturefeed_entry_ui_event_save_weekscheme(&$event, $date_control) {

  $weekscheme = NULL;
  // Construct the weekscheme.
  if (isset($date_control['opening_times']) && !$date_control['opening_times']['all_day']) {

    $weekscheme = new CultureFeed_Cdb_Data_Calendar_Weekscheme();
    foreach ($date_control['opening_times']['days'] as $day => $opening_times) {

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

  if ($date_control['type'] == 'period') {
    $calendar = new CultureFeed_Cdb_Data_Calendar_PeriodList();
    $period = new CultureFeed_Cdb_Data_Calendar_Period($date_control['period']['start_date'], $date_control['period']['end_date']);
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
 * Submit the event form, create a batch that waits and redirects.
 */
function culturefeed_entry_ui_event_form_submit($form, &$form_state) {

  $batch = array(
    'title' => t('Saving the event'),
    'operations' => array(
      array(
        'culturefeed_entry_ui_event_form_submission_timeout_batch',
        array($form_state),
      ),
    ),
    'finished' => 'culturefeed_entry_ui_event_form_submission_timeout_batch_finished',
    'file' => drupal_get_path('module', 'culturefeed_entry_ui') . '/includes/culturefeed_entry_ui.pages.inc',
  );

  $form_state['redirect'] = 'agenda/e/' . $form_state['#event_id'] . '/timeout_redirect';
  batch_set($batch);

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
    $form_state['redirect'] = 'agenda/edit';
  } catch (Exception $e) {
    watchdog_exception(WATCHDOG_ERROR, $e);
    drupal_set_message(t('An error occurred while deleting the event'), 'error');
  }

}

/**
 * Form callback: Show the manage tags form for a cdb object.
 */
function culturefeed_entry_ui_edit_tags_form($form, $form_state, $event) {

  /** @var \CultuurNet\Search\ActivityStatsExtendedEntity $event */
  $event = $event->getEntity();

  /** @var \CultureFeed_Cdb_Item_Event $event */
  $event_tags = $event->getKeywords();
  $form['#old_tags'] = $event_tags;
  $custom_tags = array();

  // Get options from variable.
  if (variable_get('culturefeed_entry_ui_custom_tags')) {
    $custom_options = explode("\n", variable_get('culturefeed_entry_ui_custom_tags'));
    $custom_options = array_combine ($custom_options, $custom_options);
  }

  // Filter out custom tags.
  if ($event_tags) {
    foreach ($event_tags as $tag) {
      if (isset($custom_options) && array_key_exists($tag, $custom_options)) {
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
    '#attributes' => array(
      'class' => array('main-submit'),
    ),
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

    drupal_set_message(t('The tags were adjusted.'));
    cache_clear_all('culturefeed:results:detail:event:' . $event->getCdbId(), 'cache_culturefeed_search');
    drupal_goto(culturefeed_search_detail_path('event', $event->getCdbId(), ''));

  } catch (Exception $e) {

    if ($e->getCode() == CultureFeed_EntryApi::CODE_KEYWORD_PRIVATE) {
      drupal_set_message(t('You tried to add a tag that should not be added by anyone. If you still wish to add this or have other questions about this, please email to "vragen@uitdatabank.be".'));
    }
    else {
      drupal_set_message(t('Error occured while saving the tags'));
    }

    watchdog_exception(WATCHDOG_ERROR, $e);

  }

}

/**
 * Adds thumbnail to photo field.
 */
function culturefeed_entry_ui_photo_after_build($element) {
  $image = '';
  if ($element['#file']) {
    $image = $element['#file'];
  }
  if ($element['#default_image']) {
    $image = $element['#default_image'];
  }
  if ($image) {
    $markup = theme('image', array('path' => $image->uri, 'width' => 100, 'attributes' => array('class' => 'photo_thumbnail')));
    $element['filename']['#markup'] = $markup;
  }
  return $element;
}

/**
 * Batch operation function to wait before redirecting to the event page.
 *
 * @param array $form_state
 *   Contains the information of the submitted event form.
 * @param array $context
 *   Contains information about the status of the batch.
 */
function culturefeed_entry_ui_event_form_submission_timeout_batch($form_state, &$context) {

  $event_cdbid = $form_state['#event_id'];
  $operation = (!empty($form_state['#update_event'])) ? 'update' : 'add';

  if (empty($context['sandbox'])) {

    $context['sandbox']['form_submitted_time'] = $form_state['submit_time'];
    $context['sandbox']['start'] = time();
    $context['sandbox']['max_duration'] = variable_get('culturefeed_entry_ui_sync_max_duration', CULTUREFEED_ENTRY_UI_SYNC_MAX_DURATION_DEFAULT);

    $context['results']['event_cdbid'] = $event_cdbid;
    $context['results']['event_title'] = $form_state['values']['title'];
    $context['results']['operation'] = $operation;

  }

  // Set the batch status first.
  $context['sandbox']['duration'] = time() - $context['sandbox']['start'];
  $context['finished'] = $context['sandbox']['duration'] / $context['sandbox']['max_duration'];

  // Load the search api item and check if the batch can be finished.
  $search_api_extended_entity = culturefeed_search_item_load($event_cdbid, 'event', FALSE);

  if ($search_api_extended_entity) {

    /* @var CultureFeed_Cdb_Item_Event $search_api_event */
    $search_api_event = $search_api_extended_entity->getEntity();
    $search_api_updated_time = new DateTime($search_api_event->getLastUpdated(), new DateTimeZone('Europe/Brussels'));

    if (
      // When an event is new and can be loaded from the search api.
      $operation == 'add' ||
      // When updating an event and the updated time on the search api
      // corresponds.
      $operation == 'update' && $search_api_updated_time->getTimestamp() >= $context['sandbox']['form_submitted_time']
    ) {
      $context['finished'] = 1;
      $context['results']['status'] = 'sync';

    }

  }

  // Check if the maximum duration is reached.
  if ($context['sandbox']['duration'] >= $context['sandbox']['max_duration']) {
    $context['finished'] = 1;
    $context['results']['status'] = 'timeout';
  }

}

/**
 * Finish function for the batch API.
 */
function culturefeed_entry_ui_event_form_submission_timeout_batch_finished($success, $results, $operations) {

  $result_messages = array(
    'sync' => array(
      'add' => t('The event %title has been created.', array('%title' => check_plain($results['event_title']))),
      'update' => t('The event %title has been updated.', array('%title' => check_plain($results['event_title']))),
    ),
    'timeout' => array(
      'add' => t('The event %title has been created, but might not have been synced yet.  This should happen shortly.', array('%title' => check_plain($results['event_title']))),
      'update' => t('The event %title has been updated, but might not have been synced yet.  This should happen shortly.', array('%title' => check_plain($results['event_title']))),
    ),
  );

  drupal_set_message($result_messages[$results['status']][$results['operation']], $results['status'] == 'sync' ? 'status' : 'warning');

  if ($results['status'] == 'timeout') {
    $message = t('Event !cdbid synchronisation failed.', array('!cdbid' => $results['event_cdbid']));
    watchdog('culturefeed_entry_ui', $message, array(), WATCHDOG_WARNING);
  }
  else {
    $message = t('Event !cdbid was synchronised successfully.', array('!cdbid' => $results['event_cdbid']));
    watchdog('culturefeed_entry_ui', $message);
  }

}

/**
 * Timeout redirect callback.
 * The timeout batch uses this redirect in case a new event has not been synced
 * yet.  In that case we redirect to the form.
 *
 * @param string $event_id
 *   The event id.
 */
function culturefeed_entry_ui_event_timeout_redirect($event_id) {

  $extended_entity = culturefeed_search_item_load($event_id, 'event', FALSE);

  if ($extended_entity) {
    $url = culturefeed_search_detail_url('event', $extended_entity->getId(), $extended_entity->getTitle('nl'));
    drupal_goto($url);
  }
  else {
    $url = 'agenda/e/add';
    drupal_alter('culturefeed_entry_ui_event_timeout_new_no_sync_redirect', $url);
    drupal_goto($url);
  }

}
