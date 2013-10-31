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
    $query = new CultureFeed_SearchActivitiesQuery();
    $query->type = CultureFeed_Activity::TYPE_UITPAS;
    $query->private = FALSE;
    $query->max = variable_get('culturefeed_uitpas_recent_actions_max', 5);
    $result = $cf->searchActivities($query);
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

}

/**
 * Saves settings for recent actions.
 */
function culturefeed_uitpas_recent_actions_settings_save($values) {

  variable_set('culturefeed_uitpas_recent_actions_max', $values['max']);

}
