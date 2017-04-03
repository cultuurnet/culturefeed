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

  return array(
    '#markup' => t('There were no memberships found for this user')
  );
}