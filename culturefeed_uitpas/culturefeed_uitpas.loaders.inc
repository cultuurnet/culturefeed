<?php

/**
 * @file
 * Loader functions for Culturefeed uitpas.
 */

/**
 * Load an uitpas promotion.
 */
function uitpas_promotion_load($id) {

  try {

    $cf = DrupalCultureFeed::getConsumerInstance();
    $promotions = array($cf->uitpas()->getPointsPromotion($id));
    return $promotions[0];

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_uitpas_uitpas_promotion', $e);
  }

  return FALSE;

}
