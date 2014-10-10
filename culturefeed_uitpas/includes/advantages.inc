<?php

/**
 * @file
 * Helper functions for UiTPAS advantages.
 */

/**
 * Returns advantages.
 */
function culturefeed_uitpas_advantages_get() {

  $advantages = array();
  $advantages_max = variable_get('culturefeed_uitpas_promotions_advantages_max', 20);
  $advantages_pager_element = 0;
  $advantages_page = pager_find_page($advantages_pager_element);
  $advantages_total = 0;

  try {

    $cf = DrupalCultureFeed::getConsumerInstance();

    // Advantages.
    $query = new CultureFeed_Uitpas_Promotion_Query_WelcomeAdvantagesOptions();
    $query->start = $advantages_page * $advantages_max;
    $query->max = $advantages_max;
    $query->unexpired = TRUE;
    $query->cashingPeriodBegin = time();
    $card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');
    if ($card_system_id) {
      $query->applicableCardSystemId = $card_system_id;
    }
    $result = $cf->uitpas()->searchWelcomeAdvantages($query);
    pager_default_initialize($result->total, $advantages_max, $advantages_pager_element);
    $advantages = $result->objects;
    $advantages_total = $result->total;

  }
  catch (Exception $e) {

    watchdog_exception('culturefeed_uitpas_advantages', $e);
    pager_default_initialize(0, $advantages_max, $advantages_pager_element);

  }

  return array(
    '#theme' => 'culturefeed_uitpas_advantages',
    '#advantages' => $advantages,
    '#advantages_total' => $advantages_total,
    '#advantages_pager_element' => $advantages_pager_element,
  );

}

/**
 * Returns form elements for advantages promotions settings.
 */
function culturefeed_uitpas_advantages_settings_get(&$form) {

  $form['advantages'] = array(
    '#type' => 'fieldset',
    '#title' => t('UiTPAS advantages settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['advantages']['culturefeed_uitpas_advantages_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Page title'),
    '#default_value' => variable_get('culturefeed_uitpas_advantages_title', 'UiTPAS advantages'),
  );
  $form['advantages']['culturefeed_uitpas_advantages_info'] = array(
    '#type' => 'textarea',
    '#title' => t('Info text'),
    '#default_value' => variable_get('culturefeed_uitpas_advantages_info'),
  );
  $form['advantages']['culturefeed_uitpas_advantages_advantages_max'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of advantages to display'),
    '#default_value' => variable_get('culturefeed_uitpas_advantages_advantages_max', 20),
  );

}
