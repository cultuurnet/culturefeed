<?php

/**
 * Contains culturefeed_entry_ui.location.inc.
 */

/**
 * Adds the location form element.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 * @param \CultureFeed_Cdb_Item_Event $event
 *   The event.
 */
function culturefeed_entry_ui_location_form(array &$form, array &$form_state, \CultureFeed_Cdb_Item_Event $event = NULL) {

  // Default values.
  $default_location_id = '';
  $default_location_label = '';
  $custom_location_title = '';
  $custom_location_street = '';
  $custom_location_number = '';
  $custom_location_zip = '';
  $custom_location_city = '';
  $custom_location_country = '';

  if ($event) {

    // Default location.
    if ($location = $event->getLocation()) {

      if ($location->getCdbId()) {
        $default_location_id = $location->getCdbId();
        $default_location_label = $location->getLabel();
      }
      else {

        $address = $location->getAddress()->getPhysicalAddress();
        $custom_location_title = $location->getLabel();
        $custom_location_street = $address->getStreet();
        $custom_location_number = $address->getHouseNumber();
        $custom_location_zip = $address->getZip();
        $custom_location_country = $address->getCountry();
        if ($custom_location_country == 'BE') {
          $custom_location_city = $custom_location_zip . ' ' . $address->getCity();
        }
        else {
          $custom_location_city = $address->getCity();
        }

      }

    }

  }

  // Form element.
  $form['basic']['location'] = array(
    '#type' => 'culturefeed_location_control',
    '#default_value' => array(
      'asset' => array(
        'id' => $default_location_id,
        'label' => $default_location_label,
      ),
      'custom' => array(
        'city' => $custom_location_city,
        'country' => $custom_location_country,
        'number' => $custom_location_number,
        'street' => $custom_location_street,
        'title' => $custom_location_title,
        'zip' => $custom_location_zip,
      ),
    ),
  );

}

/**
 * Validation handler for the location.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 *
 * @return mixed
 *   The location, NULL or \CultuurNet\Search\ActivityStatsExtendedEntity
 */
function culturefeed_entry_ui_location_form_validate(array $form, array &$form_state) {

  $values = array(
    'type' => '',
    'asset' => array(),
    'custom' => array(),
  );

  if (isset($form_state['values']['location']['location_control'])) {
    $values = $form_state['values']['location']['location_control'];
  }

  $location = NULL;

  // Custom.
  if ($values['type'] == 'custom') {

    
    if (empty($values['custom']['title'])) {
      form_set_error('location][location_control][custom][title', t('A title is required for a custom location'));
    }
     
    if ($values['custom']['country'] != 'BE') {
      if (empty($values['custom']['zip'])) {
        form_set_error('location][location_control][custom][zip', t('A zip code is required for a custom location'));
      }
      
      if (strlen($values['custom']['zip']) < 4 || strlen($values['custom']['zip']) > 15) {
        form_set_error('location][location_control][custom][zip', t('A zip code has minimum 4 characters'));
      }
      
      if (empty($values['custom']['city'])) {
        form_set_error('location][location_control][custom][city', t('A city is required for a custom location'));
      }
      
    }

    if ($values['custom']['country'] == 'BE') {

      // If an existing event is edited, the main city is not shown.
      $available_cities = culturefeed_search_get_city_suggestions($values['custom']['city']);
      $cities = array();
      $pattern = '/ \(.*?\)/';
      foreach ($available_cities as $available_city) {
        $cities[] = strtolower(preg_replace($pattern, '', $available_city));
      }
      $city = strtolower(preg_replace($pattern, '', $values['custom']['city']));
      if (!in_array($city, $cities)) {
        form_set_error('location][location_control][custom][city', t('Please select a city from the list.'));
      }

    }

  }

  elseif ($values['type'] == 'asset') {

    try {

      if ($values['asset']['label'] == '') {
        form_set_error('location', t('A location is required.'));
      }
      else {
        $location = culturefeed_search_item_load($values['asset']['id'], 'actor');
        if (!$location) {
          form_set_error('location', t('We could not validate the location'));
        }
      }

    }

    catch (Exception $e) {
      form_set_error('location', t('We could not validate the location'));
    }

  }

  return $location;

}

/**
 * Save handler for the location.
 *
 * @param \CultureFeed_Cdb_Item_Event $event
 *   The event.
 * @param mixed $location
 *   NULL or \CultuurNet\Search\ActivityStatsExtendedEntity.
 * @param array $form_state
 *   The form state.
 *
 * @return \CultureFeed_Cdb_Data_Address
 *   The address.
 */
function culturefeed_entry_ui_location_form_save(\CultureFeed_Cdb_Item_Event &$event, $location, array $form_state) {

  $values = array(
    'type' => '',
    'asset' => array(),
    'custom' => array(),
  );

  if (isset($form_state['values']['location']['location_control'])) {
    $values = $form_state['values']['location']['location_control'];
  }

  $address = NULL;

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

    // Street and number.
    $physical_address->setStreet($values['custom']['street']);
    if ($values['custom']['country'] == 'BE') {
      $physical_address->setHouseNumber($values['custom']['number']);
    }
    else {
      $physical_address->setHouseNumber('');
    }

    // Zip and city.
    if ($values['custom']['country'] == 'BE') {
      $physical_address->setZip(substr($values['custom']['city'], 0, 4));
      $physical_address->setCity(substr($values['custom']['city'], 5));
    }
    else {
      $physical_address->setCity($values['custom']['city']);
      if (empty($values['custom']['zip'])) {
        $physical_address->setZip('0000');
      }
      else {
        $physical_address->setZip($values['custom']['zip']);
      }
    }

    // Country.
    $physical_address->setCountry($values['custom']['country']);

    $address->setLabel($values['custom']['title']);
    $address->setPhysicalAddress($physical_address);

    $cdb_location = new CultureFeed_Cdb_Data_Location($address);
    $cdb_location->setLabel($values['custom']['title']);
    $event->setLocation($cdb_location);

  }

  return $address;

}
