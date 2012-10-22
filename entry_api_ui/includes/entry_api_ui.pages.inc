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
  if ($event) {

    $form['#event'] = $event;

    $publication_date = $event->getPublicationDate();

    if (($info = $event->getContactInfo())) {
      $contact_info = $info->getMails();
      $contact_info = array_merge($contact_info, $info->getPhones());
      $contact_info += array_merge($contact_info, $info->getUrls());
    }

    if ($event && ($categories = $event->getCategories())) {
      foreach ($categories as $category) {
        $category_defaults[] = $category->getId();
      }
    }

    if ($location = $event->getLocation()) {
      $address = $location->getAddress()->getPhysicalAddress();
      $default_location = $location->getLabel() . ' - ' . $address->getZip() . ' ' . $address->getCity();
    }

  }

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

  $timestamps = array();
  $when = 'one_day';
  $calendar_class = '';
  if ($event) {
    $calendar = $event->getCalendar();
    $calendar_class = get_class($calendar);
    if ($calendar_class == 'CultureFeed_Cdb_Calendar_PeriodList') {
      $when = 'period';
    }
    elseif ($calendar_class == 'CultureFeed_Cdb_Calendar_Permanent') {
      $when = 'permanent';
    }
    elseif ($calendar_class == 'CultureFeed_Cdb_Calendar_TimestampList') {
      $timestamps = $event->getCalendar();
      $when = count($timestamps) > 1 ? 'multiple_days' : 'one_day';
    }
  }

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
    '#states' => array(
      'visible' => array(
        ':input[name=when]' => array(
          array('value' => 'one_day'),
          array('value' => 'multiple_days'),
        ),
      ),
    ),
  );

  if ($timestamps) {
    foreach ($timestamps as $i => $timestamp) {
      entry_api_ui_add_timestamp_elements($form, $i, $timestamp);
    }
  }
  else {
    entry_api_ui_add_timestamp_elements($form, 0);
  }

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

  $default_start = '';
  $default_end = '';
  $all_day = FALSE;
  if ($calendar_class == 'CultureFeed_Cdb_Calendar_PeriodList') {
    $period = $event->getCalendar()->current();
    $default_start = $period->getDateFrom();
    $default_end = $period->getDateTill();
    $all_day = $period->getWeekScheme() != NULL;
  }
  elseif ($calendar_class == 'CultureFeed_Cdb_Calendar_Permanent') {
    $calendar = $event->getCalendar();
    $all_day = $calendar->getWeekScheme() != NULL;
  }

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
    '#default_value' => $all_day,
  );

  $form['contacts'] = array('#tree' => TRUE, '#type' => 'container',);
  // Add contact type selections.
  if ($contact_info) {
    foreach ($contact_info as $i => $info) {
      entry_api_ui_add_contact_info_elements($form, $i, $info);
    }
  }
  else {
    entry_api_ui_add_contact_info_elements($form, 0);
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
 * Add a new contact info element. This contains 3 form elements.
 * @param array $form
 *   Form to extend.
 * @param int $i
 *   Index for the elements.
 * @param $info
 *   ContactInfo property to be edited.
 */
function entry_api_ui_add_contact_info_elements(&$form, $i, $info = NULL) {

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
 * Add a new timestamp element. This contains 3 elements.
 *
 * @param array $form
 *   Form to extend.
 * @param int $i
 *   Index for the elements.
 * @param $timestamp
 *   Timestamp object to edit.
 */
function entry_api_ui_add_timestamp_elements(&$form, $i, $timestamp = NULL) {

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
 * Validate the event form.
 */
function entry_api_ui_event_form_validate($form, &$form_state) {

  // Validate publication date.
  if ($form_state['values']['publication_date']) {
    try {
      CultureFeed_Cdb_Calendar::validateDate($form_state['values']['publication_date']);
    }
    catch (Exception $e) {
      form_set_error('publication_date', 'Gelieve een geldige publicatie datum in te geven');
    }
  }

  // Validate timestamps.
  if ($form_state['values']['when'] == 'one_day' || $form_state['values']['when'] == 'multiple_days') {
    foreach ($form_state['values']['timestamps'] as $timestamp) {
      try {

        CultureFeed_Cdb_Calendar::validateTime($timestamp['start_date']);

        if (!empty($timestamp['end_time'])) {
          CultureFeed_Cdb_Calendar::validateTime($timestamp['end_time']);
        }
        if (!empty($timestamp['start_time'])) {
          CultureFeed_Cdb_Calendar::validateTime($timestamp['start_time']);
        }
      }
      catch (Exception $e) {
        form_set_error('timestamps', 'Gelieve een geldige tijd / datum in te geven.');
      }
    }
  }

  // Validate location.
  $location_label = $form_state['values']['location'];
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

  $update = FALSE;
  if (isset($form['#event'])) {
    $update = TRUE;
    $event = $form['#event'];
  }
  else {
    $event = new CultureFeed_Cdb_Event();
  }

  // Publication date
  if ($form_state['values']['publication_date']) {
    $event->setPublicationDate($form_state['values']['publication_date']);
  }

  // Calendar

  // Timestamps as calendar.
  if ($form_state['values']['when'] == 'one_day' || $form_state['values']['when'] == 'multiple_days') {
    $calendar = new CultureFeed_Cdb_Calendar_TimestampList();

    foreach ($form_state['values']['timestamps'] as $timestamp) {
      $timestamp_object = new CultureFeed_Cdb_Calendar_Timestamp($timestamp['start_date']);
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

  // Categories.
  $category_values = $form_state['values']['kijken_en_luisteren'];
  $category_values += $form_state['values']['doen'];
  $category_values += $form_state['values']['bezoeken'];
  $category_options = $form['what']['kijken_en_luisteren']['#options'];
  $category_options += $form['what']['doen']['#options'];
  $category_options += $form['what']['bezoeken']['#options'];

  $categories = new CultureFeed_Cdb_CategoryList();
  foreach ($category_values as $key => $value) {
    if ($value) {
      $categories->add(new CultureFeed_Cdb_Category(CultureFeed_Cdb_Category::CATEGORY_TYPE_EVENT_TYPE, $value, $category_options[$value]));
    }
  }
  $event->setCategories($categories);

  // Event details.
  $detail = new Culturefeed_Cdb_EventDetail();
  $detail->setTitle($form_state['values']['title']);
  $detail->setShortDescription($form_state['values']['short_description']);
  $detail->setLanguage('nl');

  $details = new CultureFeed_Cdb_EventDetailList();
  $details->add($detail);
  $event->setDetails($details);

  // Location / Address.
  $location_label = $form_state['values']['location'];
  $country = 'BE';
  $location = $form_state['#location'];

  // Actor is at the moment done, the old way.
  $actor = cnapi_get_actor($form_state['#location']['cdbid']);
  if ($actor) {
    $location = $actor['contactinfo']['address'];
    $country = $location['country'];
    $location['zip'] = $location['zipcode'];
  }

  $physical_address = new CultureFeed_Cdb_PhysicalAddress();
  $physical_address->setCity($location['city']);
  $physical_address->setCountry($country);
  $physical_address->setZip($location['zip']);
  if (isset($location['street'])) {
    $physical_address->setStreet($location['street']);
    $physical_address->setHouseNumber($location['housenr']);
  }
  $address = new CultureFeed_Cdb_Address($physical_address);

  $cdbLocation = new CultureFeed_Cdb_Location($address);
  $cdbLocation->setLabel($location_label);
  $cdbLocation->setCdbid($form_state['#location']['cdbid']);
  $event->setLocation($cdbLocation);

  // Contact info.
  $contact_object = new CultureFeed_Cdb_ContactInfo();
  $contact_object->addAddress(new CultureFeed_Cdb_Address($physical_address));

  foreach ($form_state['values']['contacts'] as $contact) {

    switch ($contact['type']) {

      case 'website':
        $contact_object->addUrl(new CultureFeed_Cdb_Url($contact['text'], FALSE, $contact['reservation']));
      break;

      case 'email':
        $contact_object->addMail(new CultureFeed_Cdb_Mail($contact['text'], FALSE, $contact['reservation']));
      break;

      case 'phone':
        $contact_object->addPhone(new CultureFeed_Cdb_Phone($number, $type, $isMain, $forReservations));
      break;

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
    debug($e->getMessage());
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
  foreach ($event_tags as $tag) {
    if (array_key_exists($tag, $lifestyle_options)) {
      unset($event_tags[$tag]);
      $lifestyle_tags[$tag] = $tag;
    }
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

  $event = new CultureFeed_Cdb_Event();
  $event->setExternalId($form['#object_id']);

  try {

    // Add new tags
    DrupalCultureFeed_EntryApi::addTagToEvent($event, $tags);

    // Remove old tags
    if ($removed_tags) {
      debug($removed_tags);
      foreach ($removed_tags as $removed_tag) {
        DrupalCultureFeed_EntryApi::removeTagFromEvent($event, $removed_tag);
      }
    }

    drupal_set_message("De tags werden aangepast. Het duurt echter een half uur eer deze
      aanpassingen op al onze kanalen (inclusief UitinVlaanderen onder de tab 'Weergeven'
      beschikbaar zullen zijn.");

  }
  catch (Exception $e) {
    watchdog_exception(WATCHDOG_ERROR, $e);
    drupal_set_message('Er ging iets fout tijdens het bewaren.');
  }

}
