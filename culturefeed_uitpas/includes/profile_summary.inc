<?php

/**
 * @file
 * Helper functions for UiTPAS my uitpas summary.
 */

/**
 * Returns my uitpas profile (summary).
 */
function culturefeed_uitpas_profile_summary_get() {

  $uitpas_user = culturefeed_uitpas_get_uitpas_user();

  if ($uitpas_user->user && $uitpas_user->passholder) {

    $user = $uitpas_user->user;
    // @var CultureFeed_Uitpas_Passholder $passholder.
    $passholder = $uitpas_user->passholder;
    $cf = $uitpas_user->cf;

    $advantages = array();
    $promotions = array();
    $upcoming_promotions = array();

    $card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');

    // Welcome advantages.
    try {
      $query = new CultureFeed_Uitpas_Passholder_Query_WelcomeAdvantagesOptions();
      $card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');
      if ($card_system_id &&
          isset($passholder->cardSystemSpecific[$card_system_id]) &&
          isset($passholder->cardSystemSpecific[$card_system_id]->currentCard)) {
        $query->uitpas_number = $passholder->cardSystemSpecific[$card_system_id]->currentCard->uitpasNumber;
      }
      $query->cashedIn = FALSE;
      $query->max = 2;
      if ($card_system_id) {
        $query->owningCardSystemId = $card_system_id;
      }
      $result = $cf->uitpas()->getWelcomeAdvantagesForPassholder($query);
      if ($result->total) {
        $advantages = $result->objects;
      }
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_uitpas_profile_summary_welcome_advantages', $e);
    }

    // Promotions.
    try {
      $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
      $query->maxPoints = $passholder->points;
      $query->cashingPeriodBegin = time();
      $query->sort = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::SORT_POINTS;
      $query->order = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::ORDER_DESC;
      $query->max = 4;
      if ($card_system_id) {
        $query->owningCardSystemId = $card_system_id;
      }
      $query->unexpired = TRUE;
      $result = $cf->uitpas()->getPromotionPoints($query);
      if ($result->total) {
        $promotions = $result->objects;
      }
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_uitpas_profile_summary_promotions', $e);
    }

    // Upcoming promotions.
    try {
      $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
      $query->minPoints = $passholder->points + 1;
      $query->max = 2;
      $query->cashingPeriodBegin = time();
      if ($card_system_id) {
        $query->owningCardSystemId = $card_system_id;
      }
      $query->unexpired = TRUE;
      $query->sort = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::SORT_POINTS;
      $query->order = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::ORDER_ASC;
      $result = $cf->uitpas()->getPromotionPoints($query);
      if ($result->total) {
        $upcoming_promotions = $result->objects;
      }
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_uitpas_profile_summary_upcoming_promotions', $e);
    }

    return array(
      '#theme' => 'culturefeed_uitpas_profile_summary',
      '#uitpas_user' => $user,
      '#passholder' => $passholder,
      '#advantages' => $advantages,
      '#promotions' => $promotions,
      '#upcoming_promotions' => $upcoming_promotions,
    );

  }

}

/**
 * Returns form elements for profile summary settings.
 */
function culturefeed_uitpas_profile_summary_settings_get(&$form) {

  $form['promotions_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title for the promotions'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_summary_promotions_title', t('Your UiTPAS advantages')),
  );
  $form['upcoming_promotions_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title for the upcoming promotions'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_summary_upcoming_promotions_title', t('Just a few points more for')),
  );
  $form['more_link_text'] = array(
    '#type' => 'textfield',
    '#title' => t('More link text'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_summary_more_link_text', t('Show all my promotions')),
  );

}

/**
 * Saves settings for my uitpas summary.
 */
function culturefeed_uitpas_profile_summary_settings_save($values) {

  variable_set('culturefeed_uitpas_profile_summary_promotions_title', $values['promotions_title']);
  variable_set('culturefeed_uitpas_profile_summary_upcoming_promotions_title', $values['upcoming_promotions_title']);
  variable_set('culturefeed_uitpas_profile_summary_more_link_text', $values['more_link_text']);

}
