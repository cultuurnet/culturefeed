<?php

/**
 * @file
 * Helper functions for UiTPAS profile advantages.
 */

/**
 * Returns profile advantages.
 */
function culturefeed_uitpas_profile_advantages_get() {

  $uitpas_user = culturefeed_uitpas_get_uitpas_user();

  $advantages = array();
  $advantages_max = variable_get('culturefeed_uitpas_profile_advantages_advantages_max', 20);
  $advantages_page = pager_find_page();
  $advantages_total = 0;

  if ($uitpas_user->user && $uitpas_user->passholder) {

    $card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');

    if ($card_system_id && isset($uitpas_user->passholder->cardSystemSpecific[$card_system_id]->currentCard->uitpasNumber)) {
      $uitpas_number = $uitpas_user->passholder->cardSystemSpecific[$card_system_id]->currentCard->uitpasNumber;
    }

    // Welcome advantages.
    try {
      $query = new CultureFeed_Uitpas_Passholder_Query_WelcomeAdvantagesOptions();
      $query->uitpas_number = $uitpas_number;
      $query->cashedIn = FALSE;
      $query->start = $advantages_page * $advantages_max;
      $query->max = $advantages_max;
      $result = $uitpas_user->cf->uitpas()->getWelcomeAdvantagesForPassholder($query);
      $advantages = $result->objects;
      pager_default_initialize($result->total, $advantages_max);
      $advantages_total = $result->total;
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_uitpas_profile_advantages', $e);
      pager_default_initialize(0, $advantages_max);

    }

    // Title.
    $singular = 'My advantages (1 point saved)';
    $plural = 'My advantages (@count points saved)';
    drupal_set_title(format_plural($uitpas_user->passholder->points, $singular, $plural), PASS_THROUGH);

  }

  // Solution csdco on from https://www.drupal.org/node/1049354
  $_GET['q'] = 'promotions';

  return array(
    array(
      '#theme' => 'culturefeed_uitpas_profile_advantages',
      '#advantages' => $advantages,
      '#advantages_total' => $advantages_total,
    ),
    array(
      '#theme' => 'pager',
    ),
  );

}

/**
 * Returns form elements for profile advantages settings.
 */
function culturefeed_uitpas_profile_advantages_settings_get(&$form) {

  $form['profile_advantages'] = array(
    '#type' => 'fieldset',
    '#title' => t('UiTPAS profile advantages settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['profile_advantages']['culturefeed_uitpas_profile_advantages_advantages_max'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of advantages to display'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_advantages_advantages_max', 20),
  );

}

/**
 * Title callback for the profile advantages page.
 */
function culturefeed_uitpas_profile_advantages_title() {

  return variable_get('culturefeed_uitpas_profile_details_menu_advantages_title', t('My advantages'));

}
