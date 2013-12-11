<?php

/**
 * @file
 * Helper functions for UiTPAS advantages promotions.
 */

/**
 * Returns advantages promotions.
 */
function culturefeed_uitpas_advantages_promotions_get() {

  $advantages = array();
  $advantages_max = variable_get('culturefeed_uitpas_advantages_promotions_advantages_max', 20);
  $advantages_pager_element = 0;
  $advantages_page = pager_find_page($advantages_pager_element);
  $advantages_total = 0;

  $promotions = array();
  $promotions_max = variable_get('culturefeed_uitpas_advantages_promotions_promotions_max', 20);
  $promotions_pager_element = 1;
  $promotions_page = pager_find_page($promotions_pager_element);
  $promotions_total = 0;

  try {

    $cf = DrupalCultureFeed::getConsumerInstance();

    // Advantages.
    $query = new CultureFeed_Uitpas_Promotion_Query_WelcomeAdvantagesOptions();
    $query->start = $advantages_page * $advantages_max;
    $query->max = $advantages_max;
    $query->cashingPeriodBegin = time();
    $card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');
    if ($card_system_id) {
      $query->applicableCardSystemId = $card_system_id;
    }
    $result = $cf->uitpas()->searchWelcomeAdvantages($query);
    pager_default_initialize($result->total, $advantages_max, $advantages_pager_element);
    $advantages = $result->objects;
    $advantages_total = $result->total;

    // Promotions.
    $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
    $query->start = $promotions_page * $promotions_max;
    $query->max = $promotions_max;
    $query->unexpired = TRUE;
    $query->cashingPeriodBegin = time();
    $query->cashInState = CultureFeed_Uitpas_Passholder_PointsPromotion::CASHIN_POSSIBLE;
    $card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');
    if ($card_system_id) {
      $query->applicableCardSystemId = $card_system_id;
    }
    $query->sort = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::SORT_POINTS;
    $query->order = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::ORDER_ASC;
    $result = $cf->uitpas()->getPromotionPoints($query);
    pager_default_initialize($result->total, $promotions_max, $promotions_pager_element);
    $promotions = $result->objects;
    $promotions_total = $result->total;

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_uitpas_advantages_promotions', $e);
  }

  return array(
    '#theme' => 'culturefeed_uitpas_advantages_promotions',
    '#advantages' => $advantages,
    '#advantages_total' => $advantages_total,
    '#advantages_pager_element' => $advantages_pager_element,
    '#promotions' => $promotions,
    '#promotions_total' => $promotions_total,
    '#promotions_pager_element' => $promotions_pager_element,
  );

}

/**
 * Returns form elements for advantages promotions settings.
 */
function culturefeed_uitpas_advantages_promotions_settings_get(&$form) {

  $form['advantages_promotions'] = array(
    '#type' => 'fieldset',
    '#title' => t('UiTPAS advantages promotions settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['advantages_promotions']['culturefeed_uitpas_advantages_promotions_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Page title'),
    '#default_value' => variable_get('culturefeed_uitpas_advantages_promotions_title', 'UiTPAS advantages'),
  );
  $form['advantages_promotions']['culturefeed_uitpas_advantages_promotions_info'] = array(
    '#type' => 'textarea',
    '#title' => t('Info text'),
    '#default_value' => variable_get('culturefeed_uitpas_advantages_promotions_info'),
  );
  $form['advantages_promotions']['culturefeed_uitpas_advantages_promotions_advantages_max'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of advantages to display'),
    '#default_value' => variable_get('culturefeed_uitpas_advantages_promotions_advantages_max', 20),
  );
  $form['advantages_promotions']['culturefeed_uitpas_advantages_promotions_promotions_max'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of promotions to display'),
    '#default_value' => variable_get('culturefeed_uitpas_advantages_promotions_promotions_max', 20),
  );

}
