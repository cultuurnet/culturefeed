<?php

/**
* @file
* Helper functions for UiTPAS event details advantages.
*/

/**
* Returns event details advantages.
*/
function culturefeed_uitpas_event_details_advantages_get() {

  $item = menu_get_object('culturefeed_agenda_event', 3);
  $event = NULL;

  if ($item) {

    $cdb_item = $item->getEntity();
    $cdbid = $cdb_item->getCdbid();

    $query = new CultureFeed_Uitpas_Event_Query_SearchEventsOptions();
    $query->cdbid = $cdbid;

    $cf = DrupalCultureFeed::getConsumerInstance();
    $result = $cf->uitpas()->searchEvents($query);

    if ($result->total) {
      $event = $result->objects[0];
    }

    //dpm($event);

  }




}
