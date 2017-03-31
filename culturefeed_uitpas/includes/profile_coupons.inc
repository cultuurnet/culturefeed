<?php

/**
 * @file
 * Helper functions for UiTPAS profile actions.
 */

/**
 * Returns profile actions.
 */
function culturefeed_uitpas_profile_coupons_get()
{

  $coupons = array();
  $uitpas_user = culturefeed_uitpas_get_uitpas_user();

  if ($uitpas_user->user && $uitpas_user->passholder) {
    $coupons = culturefeed_uitpas_get_passholder_coupons($uitpas_user);
  }

  if ($coupons->total > 0) {
    return array(
      '#theme' => 'culturefeed_uitpas_profile_coupons',
      '#coupons' => $coupons->objects,
    );
  } else {
    return array(
      '#markup' => t('There were no coupons found for this user')
    );
  }
}
