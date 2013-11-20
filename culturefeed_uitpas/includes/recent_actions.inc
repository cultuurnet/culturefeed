<?php

/**
 * @file
 * Helper functions for UiTPAS recent actions.
 */

/**
 * Returns recent actions.
 */
function culturefeed_uitpas_recent_actions_get() {

  $actions = array();
  try {

    $cf = DrupalCultureFeed::getConsumerInstance();
    $query = new CultureFeed_Uitpas_Event_Query_SearchCheckinsOptions();
    $query->max = variable_get('culturefeed_uitpas_recent_actions_max', 5);
    $card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');
    if ($card_system_id) {
      $query->organiserCardSystemId = $card_system_id;
    }
    $result = $cf->uitpas()->searchCheckins($query, NULL, CultureFeed_Uitpas::CONSUMER_REQUEST);
    if ($result->total) {
      $actions = $result->objects;
    }

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_uitpas_block_recent_actions', $e);
  }

  if (count($actions)) {

    return array(
      '#theme' => 'culturefeed_uitpas_recent_actions',
      '#actions' => $actions,
    );

  }

}

/**
 * Returns form elements for recent actions settings.
 */
function culturefeed_uitpas_recent_actions_settings_get(&$form) {

  $form['max'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of actions to display'),
    '#default_value' => variable_get('culturefeed_uitpas_recent_actions_max', 5),
  );

  $form['default_image'] = array(
    '#type' => 'textfield',
    '#title' => t('Default image'),
    '#default_value' => variable_get('culturefeed_uitpas_user_default_image', ''),
    '#description' => t('Use full url or an absolute path.'),
  );

}

/**
 * Saves settings for recent actions.
 */
function culturefeed_uitpas_recent_actions_settings_save($values) {

  variable_set('culturefeed_uitpas_recent_actions_max', $values['max']);
  variable_set('culturefeed_uitpas_user_default_image', $values['default_image']);

}
