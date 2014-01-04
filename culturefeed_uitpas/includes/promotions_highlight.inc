<?php

/**
 * @file
 * Helper functions for UiTPAS promotions highlights.
 */

/**
 * Returns highlighted promotions.
 */
function culturefeed_uitpas_promotions_highlight_get() {

  $promotions = array();
  try {

    $cf = DrupalCultureFeed::getConsumerInstance();
    $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
    $query->sort = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::SORT_CASHING_PERIOD_END;
    $query->max = variable_get('culturefeed_uitpas_promotions_highlight_max', 5);
    $query->cashingPeriodBegin = time();
    $query->unexpired = TRUE;
    $card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');
    if ($card_system_id) {
      $query->owningCardSystemId = $card_system_id;
    }
    if (variable_get('culturefeed_uitpas_feature_implemented_in_spotlight', FALSE)) {
      $query->inSpotlight = TRUE;
    }
    $result = $cf->uitpas()->getPromotionPoints($query);
    if ($result->total) {
      $promotions = $result->objects;
    }

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_uitpas_block_promotions_highlight', $e);
  }

  if (count($promotions)) {

    return array(
      '#theme' => 'culturefeed_uitpas_promotions_highlight',
      '#promotions' => $promotions,
    );

  }

}

/**
 * Returns form elements for highlighted promotions settings.
 */
function culturefeed_uitpas_promotions_highlights_settings_get(&$form) {

  $form['max'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of promotions to display'),
    '#default_value' => variable_get('culturefeed_uitpas_promotions_highlight_max', 5),
  );
  $form['more_link_text'] = array(
    '#type' => 'textfield',
    '#title' => t('More link text'),
    '#default_value' => variable_get('culturefeed_uitpas_promotions_highlight_more_link_text', t('Show all promotions')),
  );

}

/**
 * Saves settings for highlighted promotions.
 */
function culturefeed_uitpas_promotions_highlights_settings_save($values) {

  variable_set('culturefeed_uitpas_promotions_highlight_max', $values['max']);
  variable_set('culturefeed_uitpas_promotions_highlight_more_link_text', $values['more_link_text']);

}
