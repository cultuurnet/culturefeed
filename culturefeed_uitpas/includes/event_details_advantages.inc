<?php

/**
 * @file
 * Helper functions for UiTPAS event details advantages.
 */

/**
 * Returns event details advantages.
 */
function culturefeed_uitpas_event_details_advantages_get() {

  $build = array();

  $default_item_list = array(
    '#items' => array(),
    '#theme' => 'item_list',
    '#type' => 'ul',
  );

  $item = menu_get_object('culturefeed_agenda_event', 3);
  $event = culturefeed_uitpas_get_uitpas_event($item);

  if ($event) {

    // Opportunities tariff.
    $distribution_keys = array();
    foreach ($event->cardSystems as $card_system) {
      foreach ($card_system->distributionKeys as $distribution_key) {
        foreach ($distribution_key->conditions as $condition) {
          if ($condition->definition == $condition::DEFINITION_KANSARM) {
            $distribution_keys[] = array(
              'tariff' => $distribution_key->tariff,
            );
          }
        }
      }
    }

    if (count($distribution_keys)) {

      $item_list = $default_item_list;
      foreach ($distribution_keys as $distribution_key) {
        $item_list['#items'][] = array(
          'data' => t('Price at underprivileged tariff: € @tariff', array('@tariff' => $distribution_key['tariff'])),
          'class' => array('uitpas_underprivileged_tariff'),
        );
      }

      $build[] = array(
        'title' => array('#markup' => '<h4>' . t('Underprivileged tariffs') . '</h4>'),
        'list' => $item_list,
      );

    }

    // Other promotions.
    $item_list = $default_item_list;

    if ($event->numberOfPoints > 0.00) {
      $item_list['#items'][] = array(
        'data' => t('Collect points'),
        'class' => array('uitpas_collect_points'),
      );
    }

    try {

      $cf = DrupalCultureFeed::getLoggedInUserInstance();
      $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
      $query->balieConsumerKey = $event->locationId;
      $query->cashingPeriodBegin = time();
      $query->unexpired = TRUE;
      $result = $cf->uitpas()->getPromotionPoints($query);

      if ($result->total) {

        foreach ($result->objects as $object) {
          $item_list['#items'][] = array(
            'data' => l($object->title , 'promotion/' . culturefeed_search_slug($object->title) . '/' . $object->id),
            'class' => array('uitpas_has_promotion'),
          );
        }

      }

      if ($event->locationId) {
        $item_list['#items'][] = array(
          'data' => l(t('Show advantages at') . ' ' .  $event->locationName , 'agenda/a/' . culturefeed_search_slug($event->locationName) . '/' . $event->locationId),
          'class' => array('uitpas_has_promotion'),
        );
      }
      else {
        $item_list['#items'][] = array(
          'data' => l(t('Show advantages at') . ' ' .  $event->organiserName , 'agenda/a/' . culturefeed_search_slug($event->organiserName) . '/' . $event->organiserId),
          'class' => array('uitpas_has_promotion'),
        );
      }

    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_uitpas_event_details_promotions', $e);
    }

    if (count($item_list['#items'])) {

      $build[] = array(
        'title' => array('#markup' => '<h4>' . t('Promotions') . '</h4>'),
        'list' => $item_list,
      );

    }

    if (count($build)) {
      return $build;
    }

  }

}
