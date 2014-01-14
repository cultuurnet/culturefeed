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

    // Default age.
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
  
  // Basic.
  $form['basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic info'),
  );

  $form['basic']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => ($event) ? $event->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language())->getTitle() : NULL,
    '#required' => TRUE,
  );

  $form['basic']['what'] = array('#type' => 'container');
  $form['basic']['what']['kijken_en_luisteren'] = array(
    '#type' => 'checkboxes',
    '#options' => culturefeed_search_get_eventtype_categories(array('parent' => CULTUREFEED_ENTRY_UI_EVENTTYPE_CAT_ID_KIJKEN_EN_LUISTEREN)),
    '#title' => 'Kijken en luisteren',
    '#default_value' => $category_defaults,
  );

  $form['basic']['what']['doen'] = array(
    '#type' => 'checkboxes',
    '#options' => culturefeed_search_get_eventtype_categories(array('parent' => CULTUREFEED_ENTRY_UI_EVENTTYPE_CAT_ID_DOEN)),
    '#title' => 'Doen',
    '#default_value' => $category_defaults,
  );

  $form['basic']['what']['bezoeken'] = array(
    '#type' => 'checkboxes',
    '#options' => culturefeed_search_get_eventtype_categories(array('parent' => CULTUREFEED_ENTRY_UI_EVENTTYPE_CAT_ID_BEZOEKEN)),
    '#title' => 'Bezoeken',
    '#default_value' => $category_defaults,
  );


  $form['basic']['basic']['location'] = array(
    '#type' => 'textfield',
    '#title' => t('Location'),
    '#autocomplete_path' => 'autocomplete/culturefeed/actor-suggestions',
    '#required' => TRUE,
    '#default_value' => $default_location,
  );
  
  // Date  
  $form['basic']['when']['datetimes'] = array(
    '#tree' => TRUE,
    '#type' => 'container',
    '#prefix' => '<div id="datetimes-ajax-wrapper">',
    '#suffix' => '</div>',
  );
  $datetimes = array();
  $datetime_count = 0;
  // Add contact type selections.
  if ($datetimes) {
    foreach ($datetimes as $i => $info) {
      culturefeed_entry_ui_attach_datetimes_elements($form, $i, $info);
      $datetime_count++;
    }
  }
  else {
    culturefeed_entry_ui_attach_datetimes_elements($form, 0);
    $datetime_count++;
  } 
  
  // Add more button was clicked. Check how many extra rows need to be added.
  if (isset($form_state['#datetimes_needed'])) {

    for (; $datetime_count < $form_state['#datetimes_needed']; $datetime_count++) {
      culturefeed_entry_ui_attach_datetimes_elements($form, $datetime_count);
    }
  }

  $form['basic']['when']['add_more_datetimes'] = array(
    '#type' => 'submit',
    '#name' => 'datetimes_add_more',
    '#value' => t('Add date'),
    '#attributes' => array('class' => array('datetimes-add-more-submit')),
    '#ajax' => array(
      'callback' => 'culturefeed_entry_ui_add_more_js',
      'wrapper' => 'datetimes-ajax-wrapper',
      'effect' => 'fade',
    ),
    '#limit_validation_errors' => array(array('datetimes')),
    '#submit' => array('culturefeed_entry_ui_add_more_submit'),
  );  

  /*

  $form['basic']['when_selector'] = array(
    '#type' => 'radios',
    '#title' => t('When'),
    '#options' => array(
      'one_day' => t('One single day'),
      'multiple_days' => t('Multiple days'),
      'period' => t('Long period'),
      'permanent' => t('Never ending'),
    ),
    '#default_value' => $when,
  );

  // Timestamp elements. Used for: one day or multiple days.
  $form['basic']['timestamps'] = array(
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
      culturefeed_entry_ui_attach_timestamp_elements($form, $i, $timestamp);
      $timestamp_count++;
    }
  }
  else {
    culturefeed_entry_ui_attach_timestamp_elements($form, 0);
    $timestamp_count++;
  }

  // Add more button was clicked. Check how many extra rows need to be added.
  if (isset($form_state['#timestamps_needed'])) {
    for (; $timestamp_count < $form_state['#timestamps_needed']; $timestamp_count++) {
      culturefeed_entry_ui_attach_timestamp_elements($form, $timestamp_count);
    }
  }

  $form['basic']['add_more_timestamps'] = array(
    '#type' => 'submit',
    '#name' => 'timestamps_add_more',
    '#value' => t('add another day'),
    '#attributes' => array('class' => array('timestamp-add-more-submit')),
    '#ajax' => array(
      'callback' => 'culturefeed_entry_ui_add_more_js',
      'wrapper' => 'timestamp-ajax-wrapper',
      'effect' => 'fade',
    ),
    '#limit_validation_errors' => array(array('timestamps')),
    '#submit' => array('culturefeed_entry_ui_add_more_submit'),
    '#states' => array(
      'visible' => array(
        ':input[name=when]' => array(
          array('value' => 'multiple_days'),
        ),
      ),
    ),
  );

  // Period / permanent elements.
  $form['basic']['period_or_permanent'] = array(
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

  $form['basic']['period_or_permanent']['start_date'] = array(
    '#type' => 'textfield',
    '#title' => t('Startdate'),
    '#states' => array(
      'visible' => array(':input[name=when]' => array('value' => 'period')),
    ),
    '#default_value' => $default_start,
    '#description' => t('Format') . ': ' . date('Y-m-d'),
  );

  $form['basic']['period_or_permanent']['end_date'] = array(
    '#type' => 'textfield',
    '#title' => t('Enddate'),
    '#states' => array(
      'visible' => array(':input[name=when]' => array('value' => 'period')),
    ),
    '#default_value' => $default_end,
    '#description' => t('Format') . ': ' . date('Y-m-d'),
  );

  $form['basic']['period_or_permanent']['all_day'] = array(
    '#type' => 'radios',
    '#title' => t('Open all day?'),
    '#options' => array(1 => t('Yes'), 0 => t('No')),
    '#default_value' => ($weekscheme == NULL) ? 1 : 0,
  );

  $form['basic']['period_or_permanent']['opening_times'] = array(
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

    $form['basic']['period_or_permanent']['opening_times'][$day] = array(
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

    // Add more button was clicked. Check how many extra rows need to be added.
    if (isset($form_state['#opening_times_needed'])) {
      for (; $opening_count < $form_state['#opening_times_needed']; $opening_count++) {
        culturefeed_entry_ui_attach_opening_time_elements($form, $day, $opening_count);
      }
    }

  }

  $form['basic']['period_or_permanent']['add_more_opening_times'] = array(
    '#type' => 'submit',
    '#name' => 'opening_times_add_more',
    '#value' => t('Add another opening time'),
    '#attributes' => array('class' => array('opening-times-add-more-submit')),
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
  );
  */

  // Extra.
  $form['extra'] = array(
    '#type' => 'fieldset',
    '#title' => t('Extra info'),
  );

  $form['extra']['short_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Short description'),
    '#default_value' => $event ? $event->getDetails()->current()->getShortDescription() : NULL,
  );
  // Long description todo

  // Keywords todo

  // Tags todo

  // Links todo

  // Photo todo

  // Price todo

  // Organiser add label (organisatie niet gevonden)
  $form['extra']['organiser'] = array(
    '#type' => 'textfield',
    '#title' => t('Organization'),
    '#autocomplete_path' => 'autocomplete/culturefeed/actor-suggestions',
    '#required' => TRUE,
    '#default_value' => $default_organiser,
  );

  $form['extra']['age_category'] = array(
    '#type' => 'select',
    '#title' => t('Age'),
    '#options' => array(
      'everyone' => t('For children'),
      '1-12' => t('For children (between 1 and 12 years)'),
      '12-18' => t('For youth (between 12 and 18 years)'),
      '18+' => t('For adults (18+)'),
    ),
    '#default_value' => $age_category,
  );

  $form['extra']['age'] = array(
    '#title' => t('From'),
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

  $form['extra']['contacts'] = array(
    '#tree' => TRUE,
    '#type' => 'container',
    '#prefix' => '<div id="contacts-ajax-wrapper">',
    '#suffix' => '</div>',
  );
  $contacts_count = 0;
  // Add contact type selections.
  if ($contact_info) {
    foreach ($contact_info as $i => $info) {
      culturefeed_entry_ui_attach_contact_info_elements($form, $i, $info);
      $contacts_count++;
    }
  }
  else {
    culturefeed_entry_ui_attach_contact_info_elements($form, 0);
    $contacts_count++;
  }

  // Add more button was clicked. Check how many extra rows need to be added.
  if (isset($form_state['#contacts_needed'])) {
    for (; $contacts_count < $form_state['#contacts_needed']; $contacts_count++) {
      culturefeed_entry_ui_attach_contact_info_elements($form, $contacts_count);
    }
  }

  $form['extra']['add_more_contacts'] = array(
    '#type' => 'submit',
    '#name' => 'contacts_add_more',
    '#value' => t('add another website, phone number or email address'),
    '#attributes' => array('class' => array('contacts-add-more-submit')),
    '#ajax' => array(
      'callback' => 'culturefeed_entry_ui_add_more_js',
      'wrapper' => 'contacts-ajax-wrapper',
      'effect' => 'fade',
    ),
    '#limit_validation_errors' => array(array('contacts')),
    '#submit' => array('culturefeed_entry_ui_add_more_submit'),
  );

  // Performers todo

  // Admission and event language todo

  // Translations todo

  $form['extra']['publication_date'] = array(
    '#type' => 'textfield',
    '#title' => t('Publication date'),
    '#default_value' => $publication_date,
    '#description' => t('Format') . ': ' . date('Y-m-d'),
  );
  // Private todo

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#validate' => array('culturefeed_entry_ui_event_save_validate'),
  );

  $form['#attached']['css'][] = drupal_get_path('module', 'culturefeed_entry_ui') . '/css/culturefeed_entry_ui.css';
  return $form;

}

/**
 * Attach a new contact info element. This contains 3 form elements.
 *
 * @param array $form
 *   Form to extend.
 * @param int $i
 *   Index for the elements.
 * @param mixed $info
 *   ContactInfo property to be edited.
 */
function culturefeed_entry_ui_attach_datetimes_elements(&$form, $i, $info = NULL) {
  
  $date = '';
  $time = '';
  
  //TODO if info than fill in $date and $time
  

  $form['basic']['when']['datetimes'][$i]['date'] = array(
    '#type' => 'textfield',
    '#default_value' => $date,
    '#description' => t('Format') . ': ' . date('Y-m-d'),
  );
  
  if  ($i == 0) {
    $form['basic']['when']['datetimes'][$i]['date']['#required'] = TRUE;
    $form['basic']['when']['datetimes'][$i]['date']['#title'] = t('Date'); 
  }

  $form['basic']['when']['datetimes'][$i]['time'] = array(
    '#type' => 'textfield',
    '#default_value' => $time,
    '#description' => t('Format') . ': 00:00',
  );


}



/**
 * Attach a new contact info element. This contains 3 form elements.
 *
 * @param array $form
 *   Form to extend.
 * @param int $i
 *   Index for the elements.
 * @param mixed $info
 *   ContactInfo property to be edited.
 */
function culturefeed_entry_ui_attach_contact_info_elements(&$form, $i, $info = NULL) {

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

  $form['extra']['contacts'][$i]['type'] = array(
    '#type' => 'select',
    '#options' => array(
      'website' => t('Website'),
      'phone' => t('Phone number'),
      'email' => t('E-mail address'),
    ),
    '#default_value' => $type,
  );

  $form['extra']['contacts'][$i]['text'] = array(
    '#type' => 'textfield',
    '#default_value' => $text,
  );

  $form['extra']['contacts'][$i]['reservation'] = array(
    '#type' => 'checkbox',
    '#title' => t('for reservation'),
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
 * @param CultureFeed_Cdb_Data_Calendar_Timestamp $timestamp
 *   Timestamp object to edit.
 */
function culturefeed_entry_ui_attach_timestamp_elements(&$form, $i, CultureFeed_Cdb_Data_Calendar_Timestamp $timestamp = NULL) {

  $states = array();
  if ($i > 0) {
    $states = array(
      'visible' => array(
        ':input[name=when]' => array(
          array('value' => 'multiple_days'),
        ),
      ),
    );
  }

  $form['basic']['timestamps'][$i]['start_date'] = array(
    '#type' => 'textfield',
    '#title' => t('Startdate'),
    '#default_value' => ($timestamp) ? $timestamp->getDate() : '',
    '#description' => t('Format') . ': ' . date('Y-m-d'),
    '#states' => $states,
  );

  $form['basic']['timestamps'][$i]['start_time'] = array(
    '#type' => 'textfield',
    '#title' => t('from'),
    '#default_value' => ($timestamp) ? $timestamp->getStartTime() : '',
    '#description' => t('Format') . ': 12:00:00',
    '#states' => $states,
  );

  $form['basic']['timestamps'][$i]['end_time'] = array(
    '#type' => 'textfield',
    '#title' => t('to'),
    '#default_value' => ($timestamp) ? $timestamp->getEndTime() : '',
    '#description' => t('Format') . ': 12:00:00',
    '#states' => $states,
  );

  $form['basic']['timestamps'][$i]['no_end_time'] = array(
    '#type' => 'checkbox',
    '#title' => t('no end hour'),
    '#default_value' => ($timestamp) ? ($timestamp->getEndTime() == '') : FALSE,
    '#states' => $states,
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

  $form['basic']['period_or_permanent']['opening_times'][$day][$i]['open_from'] = array(
    '#type' => 'textfield',
    '#default_value' => $opening_time ? $opening_time->getOpenFrom() : '',
    '#description' => t('Format') . ': 12:00:00',
  );

  $form['basic']['period_or_permanent']['opening_times'][$day][$i]['open_till'] = array(
    '#type' => 'textfield',
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
  if ($values['publication_date']) {
    try {
      CultureFeed_Cdb_Data_Calendar::validateDate($values['publication_date']);
    }
    catch (Exception $e) {
      form_set_error('publication_date', t('Please fill in correct publication date'));
    }
  }

  // Validate age.
  if ($values['age'] && !is_numeric($values['age'])) {
    form_set_error('age', t('Please fill in correct age'));
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
        form_set_error('timestamps][' . $key, t('Please fill in correct date and time'));
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
              form_set_error('period_or_permanent][opening_times][' . $day . '][' . $key, t('Please fill in correct date'));
            }
          }

        }
      }
    }

  }

  // Validate location.
  if (!empty($values['location'])) {

    $location_label = $search_string = $values['location'];
    if (preg_match("|-|", $location_label)) {
      list($search_string, $zip) = explode("-", $location_label);
    }

    // Perform a search on location to check if this is an existing actor.
    try {
      $location = culturefeed_search_item_load_by_title($search_string, 'actor');
      if (!$location) {
        form_set_error('location', t('Please select a location'));
      }
    }
    catch (Exception $e) {
      form_set_error('location', t('We could not validate the location'));
    }
  }

  // Validate organiser.
  if (!empty($values['organiser'])) {

    $organiser_label = $search_string = $values['organiser'];
    if (preg_match("|-|", $organiser_label)) {
      list($search_string, $zip) = explode("-", $organiser_label);
    }

    try {
      $organiser = culturefeed_search_item_load_by_title($search_string, 'actor');
      if (!$organiser) {
        form_set_error('organiser', t('Please select a organization'));
      }
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_entry_ui', $e);
      form_set_error('organiser', t('We could not validate the organization'));
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

  $update = FALSE;
  if (isset($form['#event'])) {
    $update = TRUE;
    $event = $form['#event'];
  }
  else {
    $event = new CultureFeed_Cdb_Item_Event();
  }

  // Publication date.
  if ($values['publication_date']) {
    $event->setPublicationDate($values['publication_date']);
  }

  // Age.
  $event->setAgeFrom(($values['age'] ? $values['age'] : 0));

  // Timestamps as calendar.
  if ($values['when'] == 'one_day' || $values['when'] == 'multiple_days') {
    _culturefeed_entry_ui_event_save_timestamps($event, $values);
  }

  // Period or permanent as calendar.
  elseif ($values['when'] == 'period' || $values['when'] == 'permanent') {
    _culturefeed_entry_ui_event_save_weekscheme($event, $values);
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
  $detail = new CultureFeed_Cdb_Data_EventDetail();
  $detail->setTitle($values['title']);
  if (!empty($values['short_description'])) {
    $detail->setShortDescription($values['short_description']);
  }
  $detail->setLanguage(culturefeed_entry_ui_get_preferred_language());

  $details = new CultureFeed_Cdb_Data_EventDetailList();
  $details->add($detail);
  $event->setDetails($details);

  // Location / Address.
  $addresses = $location->getEntity()->getContactInfo()->getAddresses();
  /* @var CultureFeed_Cdb_Data_Address $address */
  $address = $addresses[0];

  // Event location.
  $cdb_location = new CultureFeed_Cdb_Data_Location($address);
  $cdb_location->setLabel($values['location']);
  $cdb_location->setCdbid($location->getId());
  $event->setLocation($cdb_location);

  // Event organiser.
  $organiser_object = new CultureFeed_Cdb_Data_Organiser();
  $organiser_object->setLabel($values['organiser']);
  $organiser_object->setCdbid($organiser->getId());
  $event->setOrganiser($organiser_object);

  // Contact info.
  $physical_address = $address->getPhysicalAddress();
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
      Drupalculturefeed_EntryApi::updateEvent($event);
      $form_state['#event_id'] = $event->getCdbId();
    }
    else {
      $form_state['#event_id'] = Drupalculturefeed_EntryApi::createEvent($event);
    }

    $form_state['#update_event'] = $update;

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_entry_ui', $e);
    form_set_error('', t('An error occurred while saving the event'));
  }

}

/**
 * Save helper function that creates calendar object for timestamps.
 */
function _culturefeed_entry_ui_event_save_timestamps(&$event, $values) {

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
function _culturefeed_entry_ui_event_save_weekscheme(&$event, $values) {

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
function culturefeed_entry_ui_event_form_submit($form, &$form_state) {

  if ($form_state['#update_event']) {
    $form_state['redirect'] = culturefeed_search_detail_url('event', $form_state['#event_id'], $form_state['values']['title']);
    drupal_set_message(t('Event successfully created'));
  }
  else {
    $link_change_event = '<a href="/culturefeed_entry_ui/event/' . $form_state['#event_id'] . '/edit">' . t('Click here to update') . '</a>';
    drupal_set_message(t('Event successfully created') . ' ' . $link_change_event);
  }

}

/**
 * Form callback: delete an event.
 */
function culturefeed_entry_ui_delete_event_form($form, $form_state, $event) {

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
  }
  catch (Exception $e) {
    watchdog_exception(WATCHDOG_ERROR, $e);
    drupal_set_message(t('An error occurred while deleting the event'), 'error');
  }

}

/**
 * Form callback: Show the manage tags form for a cdb object.
 */
function culturefeed_entry_ui_edit_tags_form($form, $form_state, $event) {

  $form = array();
  $event_tags = $event->getKeywords();
  $form['#old_tags'] = $event_tags;

  $custom_tags = array();

  // @todo Get options from variable.
  $custom_options = variable_get('culturefeed_entry_ui_custom_tags');

  echo '<pre>';print_r($custom_options);echo '</pre>';

  // Filter out custom tags.
  if ($event_tags) {
    foreach ($event_tags as $tag) {
      if (array_key_exists($tag, $custom_options)) {
        unset($event_tags[$tag]);
        $custom_tags[$tag] = $tag;
      }
    }
  }
  else {
    $event_tags = array();
  }

  $form['#object_id'] = $event->getCdbId();

  if (user_access('manage custom tags')) {
    $form['custom_tags'] = array(
      '#type' => 'checkboxes',
      '#options' => $custom_options,
      '#title' => 'Leefstijlprofiel',
      '#default_value' => $custom_tags,
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

  }
  catch (Exception $e) {

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
