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
  try {
    $event = DrupalCultureFeed_EntryApi::getEvent('cc83f911-2ad7-47c5-8bf3-14de3397099c');
  }
  catch (Excpeption $e) {
    watchdog_exception(WATCHDOG_ERROR, $e);
    drupal_set_message('Fout bij het laden van dit event', 'error');
    return $form;
  }

  $contact_info = array();
  if ($event && ($info = $event->getContactInfo())) {
    $contact_info += $info->getMails();
    $contact_info += $info->getPhones();
    $contact_info += $info->getUrls();
  }

  $timestamps = array();

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
  );

  $form['when'] = array(
    '#type' => 'radios',
    '#title' => 'Wanneer',
    '#options' => array(
      'one_day' => 'Eén dag',
      'multiple_days' => 'Meerdere dagen',
      'period' => 'Lange periode',
      'no_end' => 'Eindigt nooit',
    ),
  );

  $form['timestamps'] = array(
    '#tree' => TRUE,
    '#type' => 'container',
    /*'#states' => array(
      'visible' => array(
        ':input[name=when]' => array('value' => 'one_day'),
      ),
    ),*/
  );
  if ($timestamps) {
    foreach ($timestamps as $timestamp) {
      entry_api_ui_add_timestamp_elements($form, $i, $timestamp);
    }
  }
  else {
    entry_api_ui_add_timestamp_elements($form, 0);
  }

  $form['contacts'] = array('#tree' => TRUE);
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
        $text = $info->getMail();
        $type = 'email';
        $reservation = $info->getReservation();
      break;

      case 'CultureFeed_Cdb_Phone':
        $text = $info->getPhone();
        $type = 'phone';
        $reservation = $info->getReservation();
      break;

      case 'CultureFeed_Cdb_Url':
        $text = $info->getUrl();
        $type = 'url';
        $reservation = $info->getReservation();
      break;

    }
  }

  $form['contacts'][$i]['contact_type'] = array(
    '#type' => 'select',
    '#options' => array(
      'website' => 'Website',
      'phone' => 'Telefoonnummer',
      'email' => 'E-mailadres',
    ),
    '#default_value' => $type,
  );

  $form['contacts'][$i]['contact_text'] = array(
    '#type' => 'textfield',
    '#default_value' => $text,
  );

  $form['contacts'][$i]['contact_reservation'] = array(
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
  );

  $form['timestamps'][$i]['start_time'] = array(
    '#type' => 'textfield',
    '#title' => 'van',
    '#default_value' => ($timestamp) ? $timestamp->getStartTime() : '',
  );

  $form['timestamps'][$i]['end_time'] = array(
    '#type' => 'textfield',
    '#title' => 'tot',
    '#default_value' => ($timestamp) ? $timestamp->getEndTime() : '',
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

  $location_label = $form_state['values']['location'];
  $location = db_query("SELECT * FROM {cnapi_actor} WHERE CONCAT(LOWER(name), ' - ', zip, ' ', LOWER(city)) = :location", array(':location' => strtolower($location_label)))->fetchAssoc();

  if (!$location) {
    form_set_error('location', 'Gelieve een correcte locatie in te voeren');
  }

  $form_state['#location'] = $location;

}

/**
 * Submit the object editing form.
 */
function entry_api_ui_event_form_submit($form, &$form_state) {

  $event = new CultureFeed_Cdb_Event();

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
  $location = $form_state['#location'];

  $physical_address = new CultureFeed_Cdb_PhysicalAddress();
  $physical_address->setCity($location['city']);
  $physical_address->setCountry('BE');
  $physical_address->setZip($location['zip']);
  $address = new CultureFeed_Cdb_Address($physical_address);

  $cdbLocation = new CultureFeed_Cdb_Location($address);
  $cdbLocation->setLabel($location['name']);
  $event->setLocation($cdbLocation);

  // Contact info.
  $contact = new CultureFeed_Cdb_ContactInfo();
  $contact->addAddress(new CultureFeed_Cdb_Address($physical_address));
  $event->setContactInfo($contact);

}

/**
 * Form callback: Show the manage tags form for a cnapi object.
 */
function entry_api_ui_edit_tags_form($form, $form_state, $node) {

  $form = array();
  try {
    $event = DrupalCultureFeed_EntryApi::getEvent('cc83f911-2ad7-47c5-8bf3-14de3397099c');
  }
  catch (Excpeption $e) {
    watchdog_exception(WATCHDOG_ERROR, $e);
    drupal_set_message('Fout bij het laden van dit event', 'error');
    return $form;
  }

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

  // Check what tags are removed
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
      foreach ($removed_tags as $removed_tag) {
        debug('removing' . $removed_tag);
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