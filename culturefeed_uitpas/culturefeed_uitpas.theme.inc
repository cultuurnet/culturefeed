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
    'culturefeed_uitpas_advantage' => array(
      'variables' => array('advantage' => NULL),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
      'template' => 'culturefeed-uitpas-advantage',
    ),
    'culturefeed_uitpas_advantages_promotions' => array(
      'variables' => array(
        'advantages' => array(),
        'advantages_total' => 0,
        'advantages_pager_element' => 0,
        'promotions' => array(),
        'promotions_total' => 0,
        'promotions_pager_element' => 0,
      ),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
      'template' => 'culturefeed-uitpas-advantages-promotions',
    ),
    'culturefeed_uitpas_profile_actions' => array(
      'variables' => array(
        'actions' => array(),
        'actions_total' => 0,
        'actions_pager_element' => 0,
      ),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
      'template' => 'culturefeed-uitpas-profile-actions',
    ),
    'culturefeed_uitpas_profile_advantages' => array(
      'variables' => array(
        'advantages' => array(),
        'advantages_total' => 0,
        'advantages_pager_element' => 0,
        'promotions' => array(),
        'promotions_total' => 0,
        'promotions_pager_element' => 0,
        'upcoming_promotions' => array(),
        'upcoming_promotions_total' => 0,
        'upcoming_promotions_pager_element' => 0,
      ),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
      'template' => 'culturefeed-uitpas-profile-advantages',
    ),
    'culturefeed_uitpas_profile_details' => array(
      'variables' => array(
        'uitpas_user' => NULL,
        'passholder' => NULL,
      ),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
      'template' => 'culturefeed-uitpas-profile-details',
    ),
    'culturefeed_uitpas_profile_summary' => array(
      'variables' => array(
        'uitpas_user' => NULL,
        'passholder' => NULL,
        'advantages' => array(),
        'promotions' => array(),
        'upcoming_promotions' => array(),
      ),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
      'template' => 'culturefeed-uitpas-profile-summary',
    ),
    'culturefeed_uitpas_promotion' => array(
      'variables' => array('promotion' => NULL),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
      'template' => 'culturefeed-uitpas-promotion',
    ),
    'culturefeed_uitpas_promotions_highlight' => array(
      'variables' => array('promotions' => array()),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
      'template' => 'culturefeed-uitpas-promotions-highlight',
    ),
    'culturefeed_uitpas_recent_actions' => array(
      'variables' => array('actions' => array()),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
      'template' => 'culturefeed-uitpas-recent-actions',
    ),
    'culturefeed_uitpas_promotion_image' => array(
      'variables' => array('path' => NULL),
      'path' => drupal_get_path('module', 'culturefeed_uitpas') . '/theme',
      'file' => 'theme.inc',
    ),
  );

}
