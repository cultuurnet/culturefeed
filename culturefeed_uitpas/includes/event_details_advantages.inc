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

  $card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');
  foreach ($event->cardSystems as $card_system) {

    if ($card_system->id == $card_system_id) {

      foreach ($card_system->distributionKeys as $distribution_key) {

        foreach ($distribution_key->conditions as $condition) {

          // @TODO define constants.
          if ($condition->definition == 'SAME_REGION' && $condition->operator == 'EQUALS' && $condition->value == 'true') {
            // @TODO get passholder info?
            $item_list['items'][] = t('pasholders uit !city');
          }

        }

      }

    }

  }

  if (count($item_list['items'])) {
    return theme_item_list($item_list);
  }

}
