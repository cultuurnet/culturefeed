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
  $advantages_limit = $conf['advantages_limit'];
  $advantages_page = pager_find_page(0);
  $advantages_total = 0;

  $promotions = array();
  $promotions_limit = $conf['promotions_limit'];
  $promotions_page = pager_find_page(1);
  $promotions_total = 0;

  try {

    $cf = DrupalCultureFeed::getConsumerInstance();

    // Advantages.
    $query = new CultureFeed_Uitpas_Promotion_Query_WelcomeAdvantagesOptions();
    $query->start = $advantages_page * $advantages_limit;
    $query->max = $advantages_limit;
    $query->cashingPeriodBegin = time();
    $result = $cf->uitpas()->searchWelcomeAdvantages($query);
    $advantages_page = pager_default_initialize($result->total, $advantages_limit, 0);
    $advantages = $result->objects;
    $advantages_total = $result->total;

    // Promotions.
    $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
    $query->start = $promotions_page * $promotions_limit;
    $query->max = $promotions_limit;
    $query->unexpired = TRUE;
    $query->cashingPeriodBegin = time();
    $query->sort = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::SORT_POINTS;
    $query->order = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::ORDER_ASC;
    $result = $cf->uitpas()->getPromotionPoints($query);

    $promotions = array();
    foreach ($result->objects as $object) {
      if ($object->cashInState == "POSSIBLE") {
        $promotions[] = $object;
      }
    }

    $promotions_page = pager_default_initialize(count($promotions), $promotions_limit, 1);
    $promotions_total = count($promotions);

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

}
