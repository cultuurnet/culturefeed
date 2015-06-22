<?php

/**
 * @file
 * Profile implementations for Culturefeed uitpas.
 */

/**
 * Implements hook_culturefeed_ui_profile_menu().
 */
function culturefeed_uitpas_culturefeed_ui_profile_menu() {

  if (!culturefeed_uitpas_is_passholder()) {
    return array();
  }

  return array(
    'advantages' => array(
      'title' => variable_get('culturefeed_uitpas_profile_details_menu_advantages_title', t('My advantages')),
      'url' => 'culturefeed/profile/uitpas/promotions',
      'description' => variable_get('culturefeed_uitpas_profile_details_menu_advantages_description', t('Discover which promotions and advantages you are entitled to.')),
      'weight' => -12,
    ),
    'actions' => array(
      'title' => variable_get('culturefeed_uitpas_profile_details_menu_actions_title', t('User history')),
      'url' => 'culturefeed/profile/uitpas/actions',
      'description' => variable_get('culturefeed_uitpas_profile_details_menu_actions_description', t('An overview of all your UiTPAS actions.')),
      'weight' => -11,
    ),
  );

}

/**
 * Implements hook_culturefeed_ui_profile_menu_alter().
 */
function culturefeed_uitpas_culturefeed_ui_profile_menu_alter(&$menu) {
}
