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
    'culturefeed_uitpas_profile_actions' => array(
      'variables' => array(
        'activities' => array(),
        'activites_total' => 0,
        'activites_pager_element' => 0,
      ),
      'template' => 'culturefeed-uitpas-profile-actions',
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
