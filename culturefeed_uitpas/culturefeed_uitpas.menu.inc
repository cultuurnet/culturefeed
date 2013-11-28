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
    'advantages_promotions' => array(
      'title' => variable_get('culturefeed_uitpas_advantages_promotions_title', 'UiTPAS advantages'),
      'page callback' => 'culturefeed_uitpas_advantages_promotions_get',
      'file' => 'includes/advantages_promotions.inc',
      'access arguments' => array('access content'),
    ),
    'culturefeed/profile/uitpas/advantages' => array(
      'title callback' => 'culturefeed_uitpas_profile_advantages_title',
      'description' => variable_get('culturefeed_uitpas_profile_details_menu_advantages_description', t('Discover which promotions and advantages you are entitled to.')),
      'page callback' => 'culturefeed_uitpas_profile_advantages_get',
      'access callback' => 'culturefeed_is_culturefeed_user',
      'file' => 'includes/profile_advantages.inc',
    ),
    'promotion/%/%uitpas_promotion' => array(
      'title callback' => 'culturefeed_uitpas_promotion_details_title_get',
      'title arguments' => array(2),
      'description' => 'Details for an UiTPAS promotion.',
      'page callback' => 'culturefeed_uitpas_promotion_details_get',
      'page arguments' => array(2, 1),
      'access arguments' => array('access content'),
      'file' => 'includes/promotion_details.inc',
    ),
    'registration_confirmation' => array(
      'title callback' => 'culturefeed_uitpas_registration_confirmation_title',
      'page callback' => 'culturefeed_uitpas_registration_confirmation_get',
      'file' => 'includes/registration_confirmation.inc',
      'access callback' => TRUE,
    ),
  );

}
