<?php

/**
 * @file
 * Helper functions for UiTPAS event details advantages.
 */

/**
 * Returns event details advantages.
 */
function culturefeed_uitpas_event_details_advantages_get() {

  $item_list = array(
    'title' => '',
    'items' => array(),
    'type' => 'ul',
    'attributes' => array(),
  );

  $item = menu_get_object('culturefeed_agenda_event', 3);
  $event = culturefeed_uitpas_get_uitpas_event($item);

  if ($event) {

    $cf = DrupalCultureFeed::getConsumerInstance();

    // Save points.
    $puntsparen = ($event->numberOfPoints > 0.00);

    if ($puntsparen) {
      $item_list['items'][] = array(
        'data' => t('Collect points'),
        'class' => array('uitpas_collect_points'),
      );
    }

    // Opportunities tariff.
    $kansentarief = FALSE;
    foreach ($event->cardSystems as $card_system) {
      foreach ($card_system->distributionKeys as $key) {
        foreach ($key->conditions as $condition) {
          if ($condition->definition == $condition::DEFINITION_KANSARM) {
            $kansentarief = TRUE;
            break 3;
          }
        }
      }
    }

    if ($kansentarief) {
      $item_list['items'][] = array(
        'data' => t('Discount at underpriviliged tariff'),
        'class' => array('uitpas_underpriviliged_tariff'),
      );
    }

    // Other promotions.
    try {

      $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
      $query->balieConsumerKey = $event->locationId;
      $query->cashingPeriodBegin = time();
      $query->unexpired = TRUE;
      $result = $cf->uitpas()->getPromotionPoints($query);

      if ($result->total) {

        $item_list['items'][] = array(
          'data' => l(t('Show advantages at this location'), 'agenda/a/' . culturefeed_search_slug($event->locationName) . '/' . $event->locationId),
          'class' => array('uitpas_has_promotion'),
        );

      }

    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_uitpas_event_details_promotions', $e);
    }

    if (count($item_list['items'])) {
      return theme('item_list', $item_list);
    }

  }

}
