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
  $advantages_pager_element = 0;
  $advantages_page = pager_find_page($advantages_pager_element);
  $advantages_total = 0;

  $promotions = array();
  $promotions_max = variable_get('culturefeed_uitpas_profile_advantages_promotions_max', 20);
  $promotions_pager_element = 1;
  $promotions_page = pager_find_page($promotions_pager_element);
  $promotions_total = 0;

  $upcoming_promotions = array();
  $upcoming_promotions_max = variable_get('culturefeed_uitpas_profile_advantages_upcoming_promotions_max', 20);
  $upcoming_promotions_pager_element = 2;
  $upcoming_promotions_page = pager_find_page($upcoming_promotions_pager_element);
  $upcoming_promotions_total = 0;

  if ($uitpas_user->user && $uitpas_user->passholder) {

    $card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');
    $uitpas_number = 0;
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
      $advantages_page = pager_default_initialize($result->total, $advantages_max, $advantages_pager_element);
      $advantages_total = $result->total;
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_uitpas_profile_advantages_advantages', $e);
    }

    // Promotions.
    try {

      $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
      $query->uitpasNumber = $uitpas_number;
      $query->maxPoints = $uitpas_user->passholder->points;
      $query->cashingPeriodBegin = time();
      $query->unexpired = TRUE;
      $query->sort = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::SORT_POINTS;
      $query->order = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::ORDER_ASC;
      $query->start = $promotions_page * $promotions_max;
      $query->max = $promotions_max;
      $result = $uitpas_user->cf->uitpas()->getPromotionPoints($query);
      $promotions = $result->objects;
      $promotions_page = pager_default_initialize($result->total, $promotions_max, $promotions_pager_element);
      $promotions_total = $result->total;

    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_uitpas_profile_advantages_promotions', $e);
    }

    // Upcoming promotions.
    try {

      $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
      $query->uitpasNumber = $uitpas_number;
      $query->minPoints = $uitpas_user->passholder->points + 1;
      $query->cashingPeriodBegin = time();
      $query->unexpired = TRUE;
      $query->sort = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::SORT_POINTS;
      $query->order = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::ORDER_ASC;
      $query->start = $upcoming_promotions_page * $upcoming_promotions_max;
      $query->max = $upcoming_promotions_max;
      $result = $uitpas_user->cf->uitpas()->getPromotionPoints($query);
      $upcoming_promotions = $result->objects;
      $upcoming_promotions_page = pager_default_initialize($result->total, $upcoming_promotions_max, $upcoming_promotions_pager_element);
      $upcoming_promotions_total = $result->total;
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_uitpas_profile_advantages_upcoming_promotions', $e);
    }

    // Title.
    $singular = t('My advantages (1 point saved)');
    $plural = t('My advantages (!points points saved)', array('!points' => $uitpas_user->passholder->points));
    drupal_set_title(format_plural($uitpas_user->passholder->points, $singular, $plural), PASS_THROUGH);

  }

  return array(
    '#theme' => 'culturefeed_uitpas_profile_advantages',
    '#advantages' => $advantages,
    '#advantages_total' => $advantages_total,
    '#advantages_pager_element' => $advantages_pager_element,
    '#promotions' => $promotions,
    '#promotions_total' => $promotions_total,
    '#promotions_pager_element' => $promotions_pager_element,
    '#upcoming_promotions' => $upcoming_promotions,
    '#upcoming_promotions_total' => $upcoming_promotions_total,
    '#upcoming_promotions_pager_element' => $upcoming_promotions_pager_element,
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
  $form['profile_advantages']['culturefeed_uitpas_profile_advantages_promotions_max'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of promotions to display'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_advantages_promotions_max', 20),
  );
  $form['profile_advantages']['culturefeed_uitpas_profile_advantages_upcoming_promotions_max'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of upcoming promotions to display'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_advantages_upcoming_promotions_max', 20),
  );

}

/**
 * Title callback for the profile advantages page.
 */
function culturefeed_uitpas_profile_advantages_title() {

  return variable_get('culturefeed_uitpas_profile_details_menu_advantages_title', t('My advantages'));

}
