<?php
/**
 * @file
 * Page callbacks for the entry api ui.
 */

/**
 * Form callback: Create a new event, or edit an existing.
 */
function entry_api_ui_event_form($form, $form_state, $event = NULL) {

  $form = array();
  $contact_info = array();
  $category_defaults = array();
  $default_location = '';
  $default_organiser = '';
  $publication_date = '';
  $when = 'one_day';
  $timestamps = array();
  $default_start = '';
  $default_end = '';
  $weekscheme = NULL;
  $day_labels = array(
    'monday' => 'ma',
    'tuesday' => 'di',
    'wednesday' => 'wo',
    'thursday' => 'don',
    'friday' => 'vrij',
    'saturday' => 'zat',
    'sunday' => 'zon',
  );
  $age = '';
  $age_category = 'everyone';

  // Load all default values.
  if ($event) {

    $form['#event'] = $event;

    // Default mails, phones, urls.
    if ($info = $event->getContactInfo()) {
      $contact_info = $info->getMails();
      $contact_info = array_merge($contact_info, $info->getPhones());
      $contact_info = array_merge($contact_info, $info->getUrls());
    }

    // Default categories.
    if ($event && ($categories = $event->getCategories())) {
      foreach ($categories as $category) {
        $category_defaults[] = $category->getId();
      }
    }

    // Default location.
    if ($location = $event->getLocation()) {
      $address = $location->getAddress()->getPhysicalAddress();
      $default_location = $location->getLabel() . ' - ' . $address->getZip() . ' ' . $address->getCity();
    }

    // Default organiser.
    if ($organiser = $event->getOrganiser()) {
      $default_organiser = $organiser->getLabel();
    }

    // Default calendar type, start / end date, weekscheme data.
    $calendar = $event->getCalendar();
    $calendar_class = get_class($calendar);
    if ($calendar_class == 'CultureFeed_Cdb_Data_Calendar_PeriodList') {
      $when = 'period';
      $period = $calendar->current();
      $default_start = $period->getDateFrom();
      $default_end = $period->getDateTo();
      $weekscheme = $period->getWeekScheme();
    }
    elseif ($calendar_class == 'CultureFeed_Cdb_Data_Calendar_Permanent') {
      $when = 'permanent';
      $weekscheme = $calendar->getWeekScheme();
    }
    elseif ($calendar_class == 'CultureFeed_Cdb_Data_Calendar_TimestampList') {
      $when = iterator_count($calendar) > 1 ? 'multiple_days' : 'one_day';
      $timestamps = $calendar;
    }

    // Set the total opening times needed. This need to be equal for every day.
    if (!isset($form_state['#opening_times_needed'])) {
      $opening_times_needed = 1;
      foreach (CultureFeed_Cdb_Data_Calendar_SchemeDay::$allowedDays as $day) {
        if ($weekscheme && $data = $weekscheme->getDay($day)) {
          $opening_times = $data->getOpeningTimes();
          if (count($opening_times) > $opening_times_needed) {
            $opening_times_needed = count($opening_times);
          }
        }
      }
      $form_state['#opening_times_needed'] = $opening_times_needed;
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


  }

  // Build the form.

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => 'Titel',
    '#default_value' => ($event) ? $event->getDetails()->current()->getTitle() : NULL,
    '#required' => TRUE,
  );

  $form['location'] = array(
    '#type' => 'textfield',
    '#title' => 'Locatie',
    '#autocomplete_path' => 'cnapi/autocomplete/actor',
    '#required' => TRUE,
    '#default_value' => $default_location,
  );

  $form['organiser'] = array(
    '#type' => 'textfield',
    '#title' => 'Organisatie',
    '#autocomplete_path' => 'cnapi/autocomplete/actor',
    '#required' => TRUE,
    '#default_value' => $default_organiser,
  );

  $form['what'] = array('#type' => 'container');
  $form['what']['kijken_en_luisteren'] = array(
    '#type' => 'checkboxes',
    '#options' => entry_api_ui_get_category_options('Kijken en luisteren'),
    '#title' => 'Kijken en luisteren',
    '#default_value' => $category_defaults,
  );

  $form['what']['doen'] = array(
    '#type' => 'checkboxes',
    '#options' => entry_api_ui_get_category_options('Doen'),
    '#title' => 'Doen',
    '#default_value' => $category_defaults,
  );

  $form['what']['bezoeken'] = array(
    '#type' => 'checkboxes',
    '#options' => entry_api_ui_get_category_options('Bezoeken'),
    '#title' => 'Bezoeken',
    '#default_value' => $category_defaults,
  );

  $form['when'] = array(
    '#type' => 'radios',
    '#title' => 'Wanneer',
    '#options' => array(
      'one_day' => 'Eén dag',
      'multiple_days' => 'Meerdere dagen',
      'period' => 'Lange periode',
      'permanent' => 'Eindigt nooit',
    ),
    '#default_value' => $when,
  );

  // Timestamp elements. Used for: one day or multiple days.
  $form['timestamps'] = array(
    '#tree' => TRUE,
    '#type' => 'container',
    '#prefix' => '<div id="timestamp-ajax-wrapper">',
    '#suffix' => '</div>',
    '#states' => array(
      'visible' => array(
        ':input[name=when]' => array(
          array('value' => 'one_day'),
          array('value' => 'multiple_days'),
        ),
      ),
    ),
  );

  // Add a timestamp row for every existing element.
  $timestamp_count = 0;
  if ($timestamps) {
    foreach ($timestamps as $i => $timestamp) {
      entry_api_ui_attach_timestamp_elements($form, $i, $timestamp);
      $timestamp_count++;
    }
  }
  else {
    entry_api_ui_attach_timestamp_elements($form, 0);
    $timestamp_count++;
  }

  // Add more button was clicked. Check how many extra rows that need to be added.
  if (isset($form_state['#timestamps_needed'])) {
    for (; $timestamp_count < $form_state['#timestamps_needed']; $timestamp_count++) {
      entry_api_ui_attach_timestamp_elements($form, $timestamp_count);
    }
  }

  $form['add_more_timestamps'] = array(
    '#type' => 'submit',
    '#name' => 'timestamps_add_more',
    '#value' => 'nog een dag toevoegen',
    '#attributes' => array('class' => array('timestamp-add-more-submit')),
    '#ajax' => array(
      'callback' => 'entry_api_ui_add_more_js',
      'wrapper' => 'timestamp-ajax-wrapper',
      'effect' => 'fade',
    ),
    '#limit_validation_errors' => array(array('timestamps')),
    '#submit' => array('entry_api_ui_add_more_submit'),
    '#states' => array(
      'visible' => array(
        ':input[name=when]' => array(
          array('value' => 'multiple_days'),
        ),
      ),
    ),
  );

  // Period / permanent elements
  $form['period_or_permanent'] = array(
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

  $form['period_or_permanent']['start_date'] = array(
    '#type' => 'textfield',
    '#title' => 'Startdatum',
    '#states' => array(
      'visible' => array(':input[name=when]' => array('value' => 'period')),
    ),
    '#default_value' => $default_start,
    '#description' => 'Formaat: ' . date('Y-m-d'),
  );

  $form['period_or_permanent']['end_date'] = array(
    '#type' => 'textfield',
    '#title' => 'Einddatum',
    '#states' => array(
      'visible' => array(':input[name=when]' => array('value' => 'period')),
    ),
    '#default_value' => $default_end,
    '#description' => 'Formaat: ' . date('Y-m-d'),
  );

  $form['period_or_permanent']['all_day'] = array(
    '#type' => 'radios',
    '#title' => 'Alle dagen open / toegankelijk?',
    '#options' => array(1 => 'Ja', 0 => 'Nee'),
    '#default_value' => ($weekscheme == NULL) ? 1 : 0,
  );

  $form['period_or_permanent']['opening_times'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(':input[name="period_or_permanent[all_day]"]' => array('value' => '0')),
    ),
    '#prefix' => '<div id="opening-times-ajax-wrapper">',
    '#suffix' => '</div>',
    '#theme' => 'entry_api_ui_opening_times_table',
  );

  // Add opening times information for every day.
  foreach (CultureFeed_Cdb_Data_Calendar_SchemeDay::$allowedDays as $day) {

    $form['period_or_permanent']['opening_times'][$day] = array(
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
        entry_api_ui_attach_opening_time_elements($form, $day, $i, $opening_time);
        $opening_count++;
      }
    }
    else {
      entry_api_ui_attach_opening_time_elements($form, $day, 0);
      $opening_count++;
    }

    // Add more button was clicked. Check how many extra rows that need to be added.
    if (isset($form_state['#opening_times_needed'])) {
      for (; $opening_count < $form_state['#opening_times_needed']; $opening_count++) {
        entry_api_ui_attach_opening_time_elements($form, $day, $opening_count);
      }
    }

  }

  $form['period_or_permanent']['add_more_opening_times'] = array(
    '#type' => 'submit',
    '#name' => 'opening_times_add_more',
    '#value' => 'nog een openingsuur toevoegen',
    '#attributes' => array('class' => array('opening-times-add-more-submit')),
    '#ajax' => array(
      'callback' => 'entry_api_ui_add_more_js',
      'wrapper' => 'opening-times-ajax-wrapper',
      'effect' => 'fade',
    ),
    '#limit_validation_errors' => array(array('period_or_permanent')),
    '#submit' => array('entry_api_ui_add_more_submit'),
    '#states' => array(
      'visible' => array(':input[name="period_or_permanent[all_day]"]' => array('value' => '0')),
    ),
  );

  $form['contacts'] = array(
    '#tree' => TRUE,
    '#type' => 'container',
    '#prefix' => '<div id="contacts-ajax-wrapper">',
    '#suffix' => '</div>',
  );
  $contacts_count = 0;
  // Add contact type selections.
  if ($contact_info) {
    foreach ($contact_info as $i => $info) {
      entry_api_ui_attach_contact_info_elements($form, $i, $info);
      $contacts_count++;
    }
  }
  else {
    entry_api_ui_attach_contact_info_elements($form, 0);
    $contacts_count++;
  }

  // Add more button was clicked. Check how many extra rows that need to be added.
  if (isset($form_state['#contacts_needed'])) {
    for (; $contacts_count < $form_state['#contacts_needed']; $contacts_count++) {
      entry_api_ui_attach_contact_info_elements($form, $contacts_count);
    }
  }

  $form['add_more_contacts'] = array(
    '#type' => 'submit',
    '#name' => 'contacts_add_more',
    '#value' => 'nog een website, tel, of email toevoegen',
    '#attributes' => array('class' => array('contacts-add-more-submit')),
    '#ajax' => array(
      'callback' => 'entry_api_ui_add_more_js',
      'wrapper' => 'contacts-ajax-wrapper',
      'effect' => 'fade',
    ),
    '#limit_validation_errors' => array(array('contacts')),
    '#submit' => array('entry_api_ui_add_more_submit'),
  );

  $form['short_description'] = array(
    '#type' => 'textarea',
    '#title' => 'Korte beschrijving',
    '#default_value' => ($event) ? $event->getDetails()->current()->getShortDescription() : NULL
  );

  $form['age_category'] = array(
    '#type' => 'select',
    '#title' => 'Leeftijd',
    '#options' => array(
      'everyone' => 'Voor iedereen',
      '1-12' => 'Voor kinderen (tussen 1 en 12 jaar)',
      '12-18' => 'Voor jongeren (tussen 12 en 18 jaar)',
      '18+' => 'Voor volwassenen (18+)',
    ),
    '#default_value' => $age_category,
  );

  $form['age'] = array(
    '#title' => 'Vanaf',
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

  $form['fly'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        ':input[name=age_category]' => array(
          array('value' => '1-12'),
        ),
      ),
    ),
  );
  $form['fly']['value'] = array(
    '#markup' => 'Activiteiten voor kinderen krijgen automatisch een vlieg',
  );

  $form['publication_date'] = array(
    '#type' => 'textfield',
    '#title' => 'Publicatiedatum',
    '#default_value' => $publication_date,
    '#description' => 'Formaat: ' . date('Y-m-d'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Opslaan',
  );

  $form['#attached']['css'][] = drupal_get_path('module', 'entry_api_ui') . '/css/entry_api_ui.css';

  return $form;

}

/**
 * Attach a new contact info element. This contains 3 form elements.
 * @param array $form
 *   Form to extend.
 * @param int $i
 *   Index for the elements.
 * @param $info
 *   ContactInfo property to be edited.
 */
function entry_api_ui_attach_contact_info_elements(&$form, $i, $info = NULL) {

  $text = '';
  $type = 'website';
  $reservation = FALSE;

  // Fill in correct default data.
  if ($info) {
    switch (get_class($info)) {

      case 'CultureFeed_Cdb_Data_Mail':
        $text = $info->getMailAddress();
        $type = 'email';
        $reservation = $info->isForReservations();
      break;

      case 'CultureFeed_Cdb_Data_Phone':
        $text = $info->getNumber();
        $type = 'phone';
        $reservation = $info->isForReservations();
      break;

      case 'CultureFeed_Cdb_Data_Url':
        $text = $info->getUrl();
        $type = 'url';
        $reservation = $info->isForReservations();
      break;

    }
  }

  $form['contacts'][$i]['type'] = array(
    '#type' => 'select',
    '#options' => array(
      'website' => 'Website',
      'phone' => 'Telefoonnummer',
      'email' => 'E-mailadres',
    ),
    '#default_value' => $type,
  );

  $form['contacts'][$i]['text'] = array(
    '#type' => 'textfield',
    '#default_value' => $text,
  );

  $form['contacts'][$i]['reservation'] = array(
    '#type' => 'checkbox',
    '#title' => 'voor reservatie',
    '#default_value' => $reservation,
  );

}

/**
 * Attach a new timestamp element. This contains 3 elements.
 *
 * @param array $form
 *   Form to extend.
 * @param int $i
 *   Index for the elements.
 * @param $timestamp
 *   Timestamp object to edit.
 */
function entry_api_ui_attach_timestamp_elements(&$form, $i, $timestamp = NULL) {

  $states = array();
  if ($i > 0) {
    $states =  array(
      'visible' => array(
        ':input[name=when]' => array(
          array('value' => 'multiple_days'),
        ),
      ),
    );
  }

  $form['timestamps'][$i]['start_date'] = array(
    '#type' => 'textfield',
    '#title' => 'Startdatum',
    '#default_value' => ($timestamp) ? $timestamp->getDate() : '',
    '#description' => 'Formaat: ' . date('Y-m-d'),
    '#states' => $states,
  );

  $form['timestamps'][$i]['start_time'] = array(
    '#type' => 'textfield',
    '#title' => 'van',
    '#default_value' => ($timestamp) ? $timestamp->getStartTime() : '',
    '#description' => 'Formaat: 12:00:00',
    '#states' => $states,
  );

  $form['timestamps'][$i]['end_time'] = array(
    '#type' => 'textfield',
    '#title' => 'tot',
    '#default_value' => ($timestamp) ? $timestamp->getEndTime() : '',
    '#description' => 'Formaat: 12:00:00',
    '#states' => $states,
  );

  $form['timestamps'][$i]['no_end_time'] = array(
    '#type' => 'checkbox',
    '#title' => 'Geen einduur',
    '#default_value' => ($timestamp) ? ($timestamp->getEndTime() == '') : FALSE,
    '#states' => $states,
  );

}

/**
 * Attach opening time elements for a day.
 * @param $form
 *   Form to attach to.
 * @param $day
 *   Day to attach.
 * @param $i
 *   Index to set.
 * @param CultureFeed_Cdb_Data_Calendar_OpeningTime $opening_time
 *   Openingtime information
 *
 */
function entry_api_ui_attach_opening_time_elements(&$form, $day, $i, $opening_time = NULL) {

  $form['period_or_permanent']['opening_times'][$day][$i]['open_from'] = array(
    '#type' => 'textfield',
    '#default_value' => $opening_time ? $opening_time->getOpenFrom() : '',
    '#description' => 'Formaat: 12:00:00',
  );

  $form['period_or_permanent']['opening_times'][$day][$i]['open_till'] = array(
    '#type' => 'textfield',
    '#default_value' => $opening_time ? $opening_time->getOpenTill() : '',
    '#description' => 'Formaat: 12:00:00',
  );

}

/**
 * Validate the event form.
 */
function entry_api_ui_event_form_validate($form, &$form_state) {

  $validation_failed = FALSE;
  $values = $form_state['values'];

  // Validate publication date.
  if ($values['publication_date']) {
    try {
      CultureFeed_Cdb_Data_Calendar::validateDate($values['publication_date']);
    }
    catch (Exception $e) {
      form_set_error('publication_date', 'Gelieve een geldige publicatie datum in te geven');
      $validation_failed = TRUE;
    }
  }

  if ($values['age'] && !is_numeric($values['age'])) {
    form_set_error('age', 'Gelieve een geldige leeftijd in te voeren');
    $validation_failed = TRUE;
  }

  // Validate timestamps.
  if ($values['when'] == 'one_day' || $values['when'] == 'multiple_days') {

    // Only one timestamp saved for one day.
    if ($values['when'] == 'one_day') {
      $form_state['values']['timestamps'] = array($values['timestamps'][0]);
      $values['timestamps'] = $form_state['values']['timestamps'];
    }

    foreach ($values['timestamps'] as $key => $timestamp) {
      try {

        // Empty start dates are ignored.
        if (empty($timestamp['start_date'])) {
          continue;
        }

        CultureFeed_Cdb_Data_Calendar::validateDate($timestamp['start_date']);

        if (!empty($timestamp['end_time'])) {
          CultureFeed_Cdb_Data_Calendar::validateTime($timestamp['end_time']);
        }
        if (!empty($timestamp['start_time'])) {
          CultureFeed_Cdb_Data_Calendar::validateTime($timestamp['start_time']);
        }
      }
      catch (UnexpectedValueException $e) {
        form_set_error('timestamps][' . $key, 'Gelieve een geldige tijd en datum in te geven.');
        $validation_failed = TRUE;
      }
    }
  }

  if ($values['when'] == 'period' || $values['when'] == 'permanent') {

    // Validate the weekscheme.
    if (!$values['period_or_permanent']['all_day']) {
      foreach ($values['period_or_permanent']['opening_times'] as $day => $opening_times) {
        foreach ($opening_times as $key => $opening_time) {

          if (!empty($opening_time['open_from']) || !empty($opening_time['open_till'])) {
            try {
              CultureFeed_Cdb_Data_Calendar::validateTime($opening_time['open_from']);
              CultureFeed_Cdb_Data_Calendar::validateTime($opening_time['open_till']);
            }
            catch (Exception $e) {
              form_set_error('period_or_permanent][opening_times][' . $day . '][' . $key, 'Gelieve een geldige tijd in te geven.');
              $validation_failed = TRUE;
            }
          }

        }
      }
    }

  }

  // Validate location.
  $location_label = $values['location'];
  $location = db_query("SELECT cdbid, city, zip FROM {cnapi_actor} WHERE CONCAT(LOWER(name), ' - ', zip, ' ', LOWER(city)) = :location", array(':location' => strtolower($location_label)))->fetchAssoc();

  if (!$location) {
    form_set_error('location', 'Gelieve een correcte locatie in te voeren');
    $validation_failed = TRUE;
  }

  // Validate organiser.
  $organiser_label = $values['organiser'];

  $organiser = db_query("SELECT cdbid, city, zip FROM {cnapi_actor} WHERE CONCAT(LOWER(name), ' - ', zip, ' ', LOWER(city)) = :organiser", array(':organiser' => strtolower($organiser_label)))->fetchAssoc();

  if (!$organiser) {
    form_set_error('organiser', 'Gelieve een correcte organisatie in te voeren');
    $validation_failed = TRUE;
  }

  // Validate organiser.
  $organiser_label = $values['organiser'];
  $organiser = db_query("SELECT cdbid, city, zip FROM {cnapi_actor} WHERE CONCAT(LOWER(name), ' - ', zip, ' ', LOWER(city)) = :organiser", array(':organiser' => strtolower($organiser_label)))->fetchAssoc();

  if (!$organiser) {
    form_set_error('organiser', 'Gelieve een correcte organisatie in te voeren');
    $validation_failed = TRUE;
  }

  if (!$validation_failed) {
    _entryapi_ui_event_form_save_event($form, $form_state, $location, $organiser);
  }

}

/**
 * Save the event form to the entity api
 */
function _entryapi_ui_event_form_save_event($form, &$form_state, $location, $organiser) {

  $values = $form_state['values'];

  $update = FALSE;
  if (isset($form['#event'])) {
    $update = TRUE;
    $event = $form['#event'];
  }
  else {
    $event = new CultureFeed_Cdb_Item_Event();
  }

  // Publication date
  if ($values['publication_date']) {
    $event->setPublicationDate($values['publication_date']);
  }

  // Age
  $event->setAgeFrom(($values['age'] ? $values['age'] : 0));

  // Timestamps as calendar.
  if ($values['when'] == 'one_day' || $values['when'] == 'multiple_days') {
    _entry_api_ui_event_save_timestamps($event, $values);
  }

  // Period or permanent as calendar.
  elseif ($values['when'] == 'period' || $values['when'] == 'permanent') {
    _entry_api_ui_event_save_weekscheme($event, $values);
  }

  // Categories.
  $category_values = $values['kijken_en_luisteren'];
  $category_values += $values['doen'];
  $category_values += $values['bezoeken'];
  $category_options = $form['what']['kijken_en_luisteren']['#options'];
  $category_options += $form['what']['doen']['#options'];
  $category_options += $form['what']['bezoeken']['#options'];

  $categories = new CultureFeed_Cdb_Data_CategoryList();
  foreach ($category_values as $key => $value) {
    if ($value) {
      $categories->add(new CultureFeed_Cdb_Data_Category(CultureFeed_Cdb_Data_Category::CATEGORY_TYPE_EVENT_TYPE, $value, $category_options[$value]));
    }
  }
  $event->setCategories($categories);

  // Event details.
  $detail = new Culturefeed_Cdb_Data_EventDetail();
  $detail->setTitle($values['title']);
  if (!empty($values['short_description'])) {
    $detail->setShortDescription($values['short_description']);
  }
  $detail->setLanguage('nl');

  $details = new CultureFeed_Cdb_Data_EventDetailList();
  $details->add($detail);
  $event->setDetails($details);

  // Location / Address.
  $location_label = $values['location'];
  $location_cdbid = $location['cdbid'];
  $country = 'BE';

  // Actor is at the moment done, the old way.
  $actor = cnapi_get_actor($location['cdbid']);
  if ($actor) {
    $location = $actor['contactinfo']['address'];
    $country = $location['country'];
    $location['zip'] = $location['zipcode'];
  }

  $physical_address = new CultureFeed_Cdb_Data_Address_PhysicalAddress();
  $physical_address->setCity($location['city']);
  $physical_address->setCountry($country);
  $physical_address->setZip($location['zip']);
  if (isset($location['street'])) {
    $physical_address->setStreet($location['street']);
    $physical_address->setHouseNumber($location['housenr']);
  }
  $address = new CultureFeed_Cdb_Data_Address($physical_address);

  $cdbLocation = new CultureFeed_Cdb_Data_Location($address);
  $cdbLocation->setLabel($location_label);
  $cdbLocation->setCdbid($location_cdbid);
  $event->setLocation($cdbLocation);

  // Organiser.
  $organiser_label = $values['organiser'];
  $organiser_cdbid = $organiser['cdbid'];

  $cdbOrganiser = new CultureFeed_Cdb_Data_Organiser($organiser_label, $organiser_cdbid);

  $event->setOrganiser($cdbOrganiser);

  // Organiser.
  $organiser_object = new CultureFeed_Cdb_Data_Organiser();
  $organiser_object->setLabel($values['organiser']);
  $organiser_object->setCdbid($organiser['cdbid']);
  $event->setOrganiser($organiser_object);

  // Contact info.
  $contact_object = new CultureFeed_Cdb_Data_ContactInfo();
  $contact_object->addAddress(new CultureFeed_Cdb_Data_Address($physical_address));

  foreach ($values['contacts'] as $contact) {

    if (!empty($contact['text'])) {
      switch ($contact['type']) {

        case 'website':
          $contact_object->addUrl(new CultureFeed_Cdb_Data_Url($contact['text'], FALSE, $contact['reservation']));
        break;

        case 'email':
          $contact_object->addMail(new CultureFeed_Cdb_Data_Mail($contact['text'], FALSE, $contact['reservation']));
        break;

        case 'phone':
          $contact_object->addPhone(new CultureFeed_Cdb_Data_Phone($contact['text'], CultureFeed_Cdb_Data_Phone::PHONE_TYPE_PHONE, FALSE, $contact['reservation']));
        break;

      }
    }


  }

  $event->setContactInfo($contact_object);

  try {

    if ($update) {
      DrupalCultureFeed_EntryApi::updateEvent($event);
      $form_state['#event_id'] = $event->getCdbId();
    }
    else {
      $form_state['#event_id'] = DrupalCultureFeed_EntryApi::createEvent($event);
    }

    $form_state['#update_event'] = $update;

  }
  catch (Exception $e) {
    watchdog_exception(WATCHDOG_ERROR, $e);
    form_set_error('', 'Er ging iets fout tijdens het bewaren.');
  }

}

/**
 * Save helper function that creates calendar object for timestamps.
 */
function _entry_api_ui_event_save_timestamps(&$event, $values) {

  $calendar = new CultureFeed_Cdb_Data_Calendar_TimestampList();

  foreach ($values['timestamps'] as $timestamp) {

    // Empty start dates are ignored.
    if (empty($timestamp['start_date'])) {
      continue;
    }

    $timestamp_object = new CultureFeed_Cdb_Data_Calendar_Timestamp($timestamp['start_date']);
    if (!empty($timestamp['end_time'])) {
      $timestamp_object->setEndTime($timestamp['end_time']);
    }
    if (!empty($timestamp['start_time'])) {
      $timestamp_object->setStartTime($timestamp['start_time']);
    }
    $calendar->add($timestamp_object);
  }

  $event->setCalendar($calendar);

}

/**
 * Save helper function that creates calendar object for weekschemes.
 */
function _entry_api_ui_event_save_weekscheme(&$event, $values) {

  $weekscheme = NULL;
  // Construct the weekscheme.
  if (!$values['period_or_permanent']['all_day']) {

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

  if ($values['when'] == 'period') {
    $calendar = new CultureFeed_Cdb_Data_Calendar_PeriodList();
    $period = new CultureFeed_Cdb_Data_Calendar_Period($values['period_or_permanent']['start_date'], $values['period_or_permanent']['end_date']);
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
function entry_api_ui_event_form_submit($form, &$form_state) {

  if ($form_state['#update_event']) {
    $request_detail = array('event' => $form_state['#event_id'], 'title' => $form_state['values']['title']);
    $url = cnapi_url_dp2dua($request_detail);
    $form_state['redirect'] = $url['path'];
    drupal_set_message('De activiteit werd aangepast. Het duurt echter een half uur eer het
    op alle kanalen (inclusief UitinVlaanderen) beschikbaar zal zijn. ');
  }
  else {
    $link_change_event = '<a href="/entryapi/event/' . $form_state['#event_id'] . '/edit">Klik hier om het event aan te passen</a>';
    drupal_set_message('De activiteit werd toegevoegd. Het duurt echter een half uur eer het
      op alle kanalen (inclusief UitinVlaanderen) beschikbaar zal zijn. ' . $link_change_event);
  }

}

/**
 * Form callback: delete an event.
 */
function entry_api_ui_delete_event_form($form, $form_state, $event) {

  $form['id'] = array('#type' => 'value', '#value' => $event->getCdbId());

  return confirm_form($form,
    'Bent u zeker dat u dit event wil verwijderen?',
    '',
    'Deze actie kan niet ongedaan worden gemaakt.',
    'Verwijderen',
    'Annuleren,'
  );

}

/**
 * Submit the delete form.
 */
function entry_api_ui_delete_event_form_submit($form, &$form_state) {

  try {
    DrupalCultureFeed_EntryApi::deleteEvent($form_state['values']['id']);
    drupal_set_message('Het event is succesvol verwijderd');
    $form_state['redirect'] = '<front>';
  }
  catch (Exception $e) {
    watchdog_exception(WATCHDOG_ERROR, $e);
    drupal_set_message('Er ging iets fout bij het verwijderen van dit event.', 'error');
  }

}

/**
 * Form callback: Show the manage tags form for a cnapi object.
 */
function entry_api_ui_edit_tags_form($form, $form_state, $event) {

  $form = array();
  $event_tags = $event->getKeywords();
  $form['#old_tags'] = $event_tags;

  $lifestyle_tags = array();

  $lifestyle_options = array(
    CultureFeed_User::LIFESTYLE_ONTDEKKER => 'Ontdekker',
    CultureFeed_User::LIFESTYLE_FIJNPROEVER => 'Fijnproever',
    CultureFeed_User::LIFESTYLE_ACTIE_ZOEKER => 'Actiezoeker',
    CultureFeed_User::LIFESTYLE_ACTIEVE_ONTSPANNER => 'Actieve ontspanner',
  );

  // Filter out lifestyle tags
  if ($event_tags) {
    foreach ($event_tags as $tag) {
      if (array_key_exists($tag, $lifestyle_options)) {
        unset($event_tags[$tag]);
        $lifestyle_tags[$tag] = $tag;
      }
    }
  }
  else {
    $event_tags = array();
  }

  $form['#object_id'] = $event->getCdbId();

  if (user_access('manage lifestyle profile tags')) {
    $form['lifestyle_profile'] = array(
      '#type' => 'checkboxes',
      '#options' => $lifestyle_options,
      '#title' => 'Leefstijlprofiel',
      '#default_value' => $lifestyle_tags,
    );
  }

  $form['tags'] = array(
    '#type' => 'textfield',
    '#title' => 'Tags',
    '#default_value' => implode(';', $event_tags),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Opslaan',
  );

  return $form;

}

/**
 * Validate the tag form. No lifestyle profiles are allowed as a tag.
 */
function entry_api_ui_edit_tags_form_validate($form, &$form_state) {

  $tags = explode(';', $form_state['values']['tags']);
  foreach ($tags as $tag) {
    if (array_key_exists($tag, $form['lifestyle_profile']['#options'])) {
      form_set_error('tags', $tag . ' is niet toegelaten als tag.');
    }
  }

}

/**
 * Submit the edit tags form.
 */
function entry_api_ui_edit_tags_form_submit($form, &$form_state) {

  // Merge lifestyle profile tags into all tags.
  $tags = explode(';', $form_state['values']['tags']);
  foreach ($form_state['values']['lifestyle_profile'] as $profile) {
    if ($profile) {
      $tags[] = $profile;
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

    // Add new tags
    $tags = array_filter($tags);
    if ($tags) {
      DrupalCultureFeed_EntryApi::addTagToEvent($event, $tags);
    }

    // Remove old tags
    if ($removed_tags) {
      foreach ($removed_tags as $removed_tag) {
        DrupalCultureFeed_EntryApi::removeTagFromEvent($event, $removed_tag);
      }
    }

    drupal_set_message("De tags werden aangepast. Het duurt echter een half uur eer deze
      aanpassingen op al onze kanalen (inclusief UitinVlaanderen onder de tab 'Weergeven'
      beschikbaar zullen zijn.");

  }
  catch (Exception $e) {

    if ($e->getCode() == CultureFeed_EntryApi::CODE_KEYWORD_PRIVATE) {
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
