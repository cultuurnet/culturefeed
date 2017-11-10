<?php

/**
 * @file
 * Helper functions for UiTPAS profile actions.
 */

/**
 * Returns profile actions.
 */
function culturefeed_uitpas_profile_memberships_get() {
    $memberships = array();
    $uitpas_user = culturefeed_uitpas_get_uitpas_user();

  if ($uitpas_user->user && $uitpas_user->passholder) {
    $memberships = culturefeed_uitpas_get_passholder_memberships($uitpas_user);
  }

  if (count($memberships) > 0) {
    return theme('culturefeed_uitpas_memberships', array('memberships' => $memberships));
  }
  else {
    return array(
      '#markup' => t('There were no memberships found for this user')
    );
  }
}