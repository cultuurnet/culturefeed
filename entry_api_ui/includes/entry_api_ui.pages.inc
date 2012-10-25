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

  // Load all default values.
  if ($event) {

    $form['#event'] = $event;

    $publication_date = $event->getPublicationDate();

    // Default mails, phones, urls.
    if (($info = $event->getContactInfo())) {
      $contact_info = $info->getMails();
      $contact_info = array_merge($contact_info, $info->getPhones());
      $contact_info += array_merge($contact_info, $info->getUrls());
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
      'callback' => 'entry_api_ui_timestamp_add_more_js',
      'wrapper' => 'timestamp-ajax-wrapper',
      'effect' => 'fade',
    ),
    '#limit_validation_errors' => array(array('timestamps')),
    '#submit' => array('entry_api_ui_timestamp_add_more_submit'),
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
  );

  $form['period_or_permanent']['end_date'] = array(
    '#type' => 'textfield',
    '#title' => 'Einddatum',
    '#states' => array(
      'visible' => array(':input[name=when]' => array('value' => 'period')),
    ),
    '#default_value' => $default_end,
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
  );

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

    if ($opening_times) {
      foreach ($opening_times as $i => $opening_time) {
        entry_api_ui_attach_opening_time_elements($form, $day, $i, $opening_time);
      }
    }
    else {
      entry_api_ui_attach_opening_time_elements($form, $day, 0);
    }

  }

  $form['contacts'] = array('#tree' => TRUE, '#type' => 'container',);
  // Add contact type selections.
  if ($contact_info) {
    foreach ($contact_info as $i => $info) {
      entry_api_ui_attach_contact_info_elements($form, $i, $info);
    }
  }
  else {
    entry_api_ui_attach_contact_info_elements($form, 0);
  }

  $form['short_description'] = array(
    '#type' => 'textarea',
    '#title' => 'Korte beschrijving',
    '#default_value' => ($event) ? $event->getDetails()->current()->getShortDescription() : NULL
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

      case 'CultureFeed_Cdb_Mail':
        $text = $info->getMailAddress();
        $type = 'email';
        $reservation = $info->isForReservations();
      break;

      case 'CultureFeed_Cdb_Phone':
        $text = $info->getNumber();
        $type = 'phone';
        $reservation = $info->isForReservations();
      break;

      case 'CultureFeed_Cdb_Url':
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

  $form['timestamps'][$i]['start_date'] = array(
    '#type' => 'textfield',
    '#title' => 'Startdatum',
    '#default_value' => ($timestamp) ? $timestamp->getDate() : '',
    '#description' => 'Formaat: ' . date('Y-m-d'),
  );

  $form['timestamps'][$i]['start_time'] = array(
    '#type' => 'textfield',
    '#title' => 'van',
    '#default_value' => ($timestamp) ? $timestamp->getStartTime() : '',
    '#description' => 'Formaat: 12:00:00',
  );

  $form['timestamps'][$i]['end_time'] = array(
    '#type' => 'textfield',
    '#title' => 'tot',
    '#default_value' => ($timestamp) ? $timestamp->getEndTime() : '',
    '#description' => 'Formaat: 12:00:00',
  );

  $form['timestamps'][$i]['no_end_time'] = array(
    '#type' => 'checkbox',
    '#title' => 'Geen einduur',
    '#default_value' => ($timestamp) ? ($timestamp->getEndTime() == '') : FALSE,
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
    '#title' => 'van',
    '#default_value' => $opening_time ? $opening_time->getOpenFrom() : '',
  );

  $form['period_or_permanent']['opening_times'][$day][$i]['open_till'] = array(
    '#type' => 'textfield',
    '#title' => 'tot',
    '#default_value' => $opening_time ? $opening_time->getOpenTill() : '',
  );

}

/**
 * Validate the event form.
 */
function entry_api_ui_event_form_validate($form, &$form_state) {

  $values = $form_state['values'];

  // Validate publication date.
  if ($values['publication_date']) {
    try {
      CultureFeed_Cdb_Data_Calendar::validateDate($values['publication_date']);
    }
    catch (Exception $e) {
      form_set_error('publication_date', 'Gelieve een geldige publicatie datum in te geven');
    }
  }

  // Validate timestamps.
  if ($values['when'] == 'one_day' || $values['when'] == 'multiple_days') {
    foreach ($values['timestamps'] as $key => $timestamp) {
      try {

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
  }

  $form_state['#location'] = $location;

}

/**
 * Submit the object editing form.
 */
function entry_api_ui_event_form_submit($form, &$form_state) {

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

  // Calendar

  // Timestamps as calendar.
  if ($values['when'] == 'one_day' || $values['when'] == 'multiple_days') {
    $calendar = new CultureFeed_Cdb_Data_Calendar_TimestampList();

    foreach ($values['timestamps'] as $timestamp) {
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

  // Period or permanent
  if ($values['when'] == 'period' || $values['when'] == 'permanent') {

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

      if ($values['when'] == 'period') {
        $calendar = new CultureFeed_Cdb_Data_Calendar_PeriodList();
        $period = new CultureFeed_Cdb_Data_Calendar_Period($values['period_or_permanent']['start_date'], $values['period_or_permanent']['end_date']);
        $period->setWeekScheme($weekscheme);
        $calendar->add($period);
      }
      else {
        $calendar = new CultureFeed_Cdb_Data_Calendar_Permanent();
        $calendar->setWeekScheme($weekscheme);
      }

      $event->setCalendar($calendar);

    }

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
  $detail->setShortDescription($values['short_description']);
  $detail->setLanguage('nl');

  $details = new CultureFeed_Cdb_Data_EventDetailList();
  $details->add($detail);
  $event->setDetails($details);

  // Location / Address.
  $location_label = $values['location'];
  $country = 'BE';
  $location = $form_state['#location'];

  // Actor is at the moment done, the old way.
  $actor = cnapi_get_actor($form_state['#location']['cdbid']);
  if ($actor) {
    $location = $actor['contactinfo']['address'];
    $country = $location['country'];
    $location['zip'] = $location['zipcode'];
  }

  $physical_address = new CultureFeed_Cdb_Data_PhysicalAddress();
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
  $cdbLocation->setCdbid($form_state['#location']['cdbid']);
  $event->setLocation($cdbLocation);

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
          $contact_object->addPhone(new CultureFeed_Cdb_Data_Phone($contact['text'], $type, $isMain, $forReservations));
        break;

      }
    }


  }

  $event->setContactInfo($contact_object);

  try {
    if ($update) {
      DrupalCultureFeed_EntryApi::updateEvent($event);
      drupal_set_message('De activiteit werd aangepast. Het duurt echter een half uur eer het
      op alle kanalen (inclusief UitinVlaanderen) beschikbaar zal zijn.');
    }
    else {
      $id = DrupalCultureFeed_EntryApi::createEvent($event);
      drupal_set_message('id van event:' . $id);
      drupal_set_message('De activiteit werd toegevoegd. Het duurt echter een half uur eer het
        op alle kanalen (inclusief UitinVlaanderen) beschikbaar zal zijn.');
    }
  }
  catch (Exception $e) {
    watchdog_exception(WATCHDOG_ERROR, $e);
    drupal_set_message('Er ging iets fout tijdens het bewaren.');
  }

}

/**
 * Form callback: delete an event.
 */
function entry_api_ui_delete_event_form($form, $form_state, $event) {

  $form['id'] = array('#type' => 'value', '#value' => $event->getExternalId());

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

  $form['#object_id'] = 'cc83f911-2ad7-47c5-8bf3-14de3397099c';

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
  foreach ($form['#old_tags'] as $old_tag) {
    if (!in_array($old_tag, $tags)) {
      $removed_tags[] = $old_tag;
    }
  }

  $event = new CultureFeed_Cdb_Item_Event();
  $event->setExternalId($form['#object_id']);

  try {

    // Add new tags
    DrupalCultureFeed_EntryApi::addTagToEvent($event, $tags);

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
