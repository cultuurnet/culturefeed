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
  $advantages_page = pager_find_page(0);
  $advantages_total = 0;

  $promotions = array();
  $promotions_max = variable_get('culturefeed_uitpas_advantages_promotions_promotions_max', 20);
  $promotions_page = pager_find_page(1);
  $promotions_total = 0;

  try {

    $cf = DrupalCultureFeed::getConsumerInstance();

    // Advantages.
    $query = new CultureFeed_Uitpas_Promotion_Query_WelcomeAdvantagesOptions();
    $query->start = $advantages_page * $advantages_max;
    $query->max = $advantages_max;
    $query->cashingPeriodBegin = time();
    $result = $cf->uitpas()->searchWelcomeAdvantages($query);
    $advantages_page = pager_default_initialize($result->total, $advantages_max, 0);
    $advantages = $result->objects;
    $advantages_total = $result->total;

    // Promotions.
    $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
    $query->start = $promotions_page * $promotions_max;
    $query->max = $promotions_max;
    $query->unexpired = TRUE;
    $query->cashingPeriodBegin = time();
    $query->cashInState = CultureFeed_Uitpas_Passholder_PointsPromotion::CASHIN_POSSIBLE;
    $query->sort = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::SORT_POINTS;
    $query->order = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::ORDER_ASC;
    $result = $cf->uitpas()->getPromotionPoints($query);
    $promotions_page = pager_default_initialize($result->total, $promotions_max, 1);
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
    '#promotions' => $promotions,
    '#promotions_total' => $promotions_total,
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
