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
    $memberships = culturefeed_uitpas_get_passholder_memeberships($uitpas_user);
  }

  if (count($memberships) > 0) {
    $header = array(t('Name'), t('Valid until'));
    $rows = array();
    foreach ($memberships as $membership) {
      $row = array($membership->name, date('d/m/Y', $membership->endDate));
      $rows[] = $row;
    }
    return theme('table', array('header' => $header, 'rows' => $rows));
  }
  else {
    return array(
      '#markup' => t('There were no memberships found for this user')
    );
  }
}