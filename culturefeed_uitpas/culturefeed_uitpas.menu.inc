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
    'culturefeed/profile/uitpas/actions' => array(
      'title' => variable_get('culturefeed_uitpas_profile_details_menu_actions_title', t('User history')),
      'description' => variable_get('culturefeed_uitpas_profile_details_menu_actions_description', t('An overview of all your UiTPAS actions.')),
      'page callback' => 'culturefeed_uitpas_profile_actions_get',
      'access callback' => 'culturefeed_is_culturefeed_user',
      'file' => 'includes/profile_actions.inc',
    ),
    'registration_confirmation' => array(
      'title' => variable_get('culturefeed_uitpas_registration_confirmation_title', 'You are now registered on UiTPAS'),
      'page callback' => 'culturefeed_uitpas_registration_confirmation_get',
      'file' => 'includes/registration_confirmation.inc',
      'access callback' => TRUE,
    ),
  );

}
