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
    'advantage/%/%uitpas_advantage' => array(
      'title callback' => 'culturefeed_uitpas_advantage_details_title_get',
      'title arguments' => array(2),
      'description' => 'Details for an UiTPAS advantage.',
      'page callback' => 'culturefeed_uitpas_advantage_details_get',
      'page arguments' => array(2, 1),
      'access arguments' => array('access content'),
      'file' => 'includes/advantage_details.inc',
    ),
    'registration_confirmation' => array(
      'title' => variable_get('culturefeed_uitpas_registration_confirmation_title', 'You are now registered on UiTPAS'),
      'page callback' => 'culturefeed_uitpas_registration_confirmation_get',
      'file' => 'includes/registration_confirmation.inc',
      'access callback' => TRUE,
    ),
  );

}
