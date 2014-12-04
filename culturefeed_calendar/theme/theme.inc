<?php

/**
 * @file
 * Theming / preprocess functions for culturefeed_calendar.
 */

/**
 * Preprocess the variables for the calendar buttons.
 * @see culturefeed-calendar-button.tpl.php
 */
function culturefeed_calendar_preprocess_culturefeed_calendar_button(&$variables) {

  if ($variables['button']['action'] == 'view') {

    $variables['button']['url'] = 'culturefeed/calendar';
    if ($variables['button']['location'] == 'content') {
      $variables['button']['description'] = t('This activity is added to your OuTcalendar.');
      $variables['button']['text'] = t('View OuTcalendar');
    }
    elseif ($variables['button']['location'] == 'footer') {
      $variables['button']['text'] = t('To your OuTcalendar');
    }

  }
  elseif ($variables['button']['action'] == 'add') {

    $variables['button']['url'] = 'culturefeed/calendar/add/' . $variables['button']['item_id'] . '/ajax';
    if ($variables['button']['location'] == 'content') {
      $variables['button']['text'] = t('Add to my OuTcalendar');
    }
    elseif ($variables['button']['location'] == 'footer') {
      $variables['button']['text'] = t('Add');
    }

  }

}

/**
 * Preprocess the variables for one of the event templates on calendar page.
 */
function culturefeed_calendar_preprocess_culturefeed_calendar_activity_summary(&$variables) {

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

    $variables['event']['title'] = $activity->nodeTitle;
    $variables['event']['date'] = '';
    if(!empty($activity->value)) {
      $variables['event']['date'] = date('d-m-Y', strtotime(json_decode($activity->value)->date[0]));
    }

  }

  if ($event_detail) {

    // Link to detail.
    $variables['url'] = culturefeed_search_detail_url($variables['item']->getType(), $event->getCdbid(), $event_detail->getTitle());

    // When.
    $variables['when'] = check_plain($event_detail->getCalendarSummary());

  }

  // Personal calendar buttons.
  $variables['personal_calendar']['button_footer'] = '';
  $variables['personal_calendar']['button_content'] = '';

  if (module_exists('culturefeed_calendar')) {
    $variables['personal_calendar']['button_footer'] = culturefeed_calendar_render_button($variables['item'], 'footer');
    $variables['personal_calendar']['button_content'] = culturefeed_calendar_render_button($variables['item'],'content');
  }

}
