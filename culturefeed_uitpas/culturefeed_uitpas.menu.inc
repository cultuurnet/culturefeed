<?php

/**
 * @file
 * Menu functions for Culturefeed uitpas.
 */

/**
 * Implements hook_menu().
 */
function culturefeed_uitpas_menu() {

  return array(
    'admin/config/culturefeed/uitpas' => array(
      'title' => 'UiTPAS',
      'description' => 'Change UiTPAS setting like the card system, ...',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('culturefeed_uitpas_settings_form'),
      'access arguments' => array('administer site configuration'),
      'file' => 'culturefeed_uitpas.admin.inc',
    ),
    'culturefeed/profile/uitpas/advantages' => array(
      'title' => variable_get('culturefeed_uitpas_profile_details_menu_advantages_title', t('My advantages')),
      'description' => variable_get('culturefeed_uitpas_profile_details_menu_advantages_description', t('Discover which promotions and advantages you are entitled to.')),
      'page callback' => 'culturefeed_uitpas_profile_advantages_get',
      'access callback' => 'culturefeed_is_culturefeed_user',
      'file' => 'includes/profile_advantages.inc',
    ),
    'registration_confirmation' => array(
      'title' => variable_get('culturefeed_uitpas_registration_confirmation_title', 'You are now registered on UiTPAS'),
      'page callback' => 'culturefeed_uitpas_registration_confirmation_get',
      'file' => 'includes/registration_confirmation.inc',
      'access callback' => TRUE,
    ),
  );

}
