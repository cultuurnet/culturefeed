<?php

/**
 * @file
 * Theme implementations for Culturefeed uitpas.
 */

/**
 * Implements hook_theme().
 */
function culturefeed_uitpas_theme() {

  return array(
    'culturefeed_uitpas_profile_details' => array(
      'variables' => array(
        'uitpas_user' => NULL,
        'passholder' => NULL,
      ),
      'template' => 'culturefeed-uitpas-profile-details',
      'file' => 'theme.inc',
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
    ),
    'culturefeed_uitpas_promotions_highlight' => array(
      'variables' => array('promotions' => array()),
      'template' => 'culturefeed-uitpas-promotions-highlight',
      'file' => 'theme.inc',
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
    ),
    'culturefeed_uitpas_recent_actions' => array(
      'variables' => array('actions' => array()),
      'template' => 'culturefeed-uitpas-recent-actions',
      'file' => 'theme.inc',
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
    ),
  );

}
