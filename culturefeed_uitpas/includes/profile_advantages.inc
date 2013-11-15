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

  if ($uitpas_user->user && $uitpas_user->passholder) {

    $card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');
    $uitpas_number = 0;
    if ($card_system_id && isset($uitpas_user->passholder->cardSystemSpecific[$card_system_id]->currentCard->uitpasNumber)) {
      $uitpas_number = $uitpas_user->passholder->cardSystemSpecific[$card_system_id]->currentCard->uitpasNumber;
    }

    $advantages = array();
    $promotions = array();
    $upcoming_promotions = array();

    // Welcome advantages.
    try {

      $query = new CultureFeed_Uitpas_Passholder_Query_WelcomeAdvantagesOptions();
      $query->uitpas_number = $uitpas_number;
      $query->cashedIn = FALSE;
      $result = $uitpas_user->cf->uitpas()->getWelcomeAdvantagesForPassholder($query);
      if ($result->total) {
        $advantages = $result->objects;
      }

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
      $result = $uitpas_user->cf->uitpas()->getPromotionPoints($query);
      if ($result->total) {
        $promotions = $result->objects;
      }

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
      $result = $uitpas_user->cf->uitpas()->getPromotionPoints($query);
      if ($result->total) {
        $upcoming_promotions = $result->objects;
      }

    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_uitpas_profile_advantages_upcoming_promotions', $e);
    }

    // Title.
    $block = new stdClass();
    $singular = t('My advantages (1 point saved)');
    $plural = t('My advantages (!points points saved)', array('!points' => $uitpas_user->passholder->points));
    drupal_set_title(format_plural($uitpas_user->passholder->points, $singular, $plural), PASS_THROUGH);

    return array(
      '#theme' => 'culturefeed_uitpas_profile_advantages',
      '#advantages' => $advantages,
      '#promotions' => $promotions,
      '#upcoming_promotions' => $upcoming_promotions,
    );

  }

}
