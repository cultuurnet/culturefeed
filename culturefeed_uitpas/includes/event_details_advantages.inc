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

  $puntsparen = ($event->numberOfPoints > 0.00);

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

  if ($puntsparen) {
    $item_list['items'][] = array(
      'data' => t('Collect points'),
      'class' => array('uitpas_collect_points'),
    );
  }

  if ($kansentarief) {
    $item_list['items'][] = array(
      'data' => t('Discount at underpriviliged tariff'),
      'class' => array('uitpas_underpriviliged_tariff'),
    );
  }

  if (count($item_list['items'])) {
    return theme('item_list', $item_list);
  }

}
