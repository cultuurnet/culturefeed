<?php

/**
 * @file
 * Profile implementations for Culturefeed uitpas.
 */

/**
 * Implements hook_culturefeed_ui_profile_menu().
 */
function culturefeed_uitpas_culturefeed_ui_profile_menu() {

  return array(
    'advantages' => array(
      'title' => variable_get('culturefeed_uitpas_profile_details_menu_advantages_title', t('My advantages')),
      'url' => 'culturefeed/profile/advantages',
      'description' => variable_get('culturefeed_uitpas_profile_details_menu_advantages_description', t('Discover which promotions and advantages you are entitled to.')),
      'weight' => -12,
    ),
    'actions' => array(
      'title' => variable_get('culturefeed_uitpas_profile_details_menu_actions_title', t('User history')),
      'url' => 'culturefeed/profile/actions',
      'description' => variable_get('culturefeed_uitpas_profile_details_menu_actions_description', t('An overview of all your UiTPAS actions.')),
      'weight' => -11,
    ),
    'notifications' => array(
      'title' => variable_get('culturefeed_uitpas_profile_details_menu_notifications_title', t('My notifications')),
      'url' => 'culturefeed/profile/notifications',
      'description' => variable_get('culturefeed_uitpas_profile_details_menu_notifications_description', t('Change your preferences for receiving personal notifications via email.')),
      'weight' => -10,
    ),
  );

}

/**
 * Implements hook_culturefeed_ui_profile_menu_alter().
 */
function culturefeed_uitpas_culturefeed_ui_profile_menu_alter(&$menu) {
}

/**
 * Implements hook_user_view().
 */
function culturefeed_uitpas_user_view($account) {

  module_load_include('inc', 'culturefeed_uitpas', 'includes/profile_details');
  $account->content['uitpas'] = culturefeed_uitpas_profile_details_get();

}
