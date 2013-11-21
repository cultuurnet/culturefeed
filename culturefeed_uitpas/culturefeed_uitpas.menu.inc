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
    'registration_confirmation' => array(
      'title callback' => 'culturefeed_uitpas_registration_confirmation_title',
      'page callback' => 'culturefeed_uitpas_registration_confirmation_get',
      'file' => 'includes/registration_confirmation.inc',
      'access callback' => TRUE,
    ),
  );

}
