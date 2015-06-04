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
    'advantages' => array(
      'title' => variable_get('culturefeed_uitpas_advantages_title', 'UiTPAS advantages'),
      'page callback' => 'culturefeed_uitpas_advantages_get',
      'file' => 'includes/advantages.inc',
      'access arguments' => array('access content'),
    ),
    'promotions' => array(
      'title' => variable_get('culturefeed_uitpas_promotions_title', 'UiTPAS promotions'),
      'page callback' => 'drupal_get_form',
      'page arguments' => array('culturefeed_uitpas_promotions_filter_sort'),
      'file' => 'includes/promotions.inc',
      'access arguments' => array('access content'),
    ),
    'culturefeed/profile/uitpas' => array(
      'title callback' => 'culturefeed_ui_title_callback_profile_name',
      'page callback' => 'culturefeed_uitpas_profile_details_get',
      'access callback' => 'culturefeed_uitpas_is_passholder',
      'file' => 'includes/profile_details.inc',
    ),
    'culturefeed/profile/uitpas/actions' => array(
      'title callback' => 'culturefeed_uitpas_profile_actions_title',
      'description' => variable_get('culturefeed_uitpas_profile_details_menu_actions_description', t('An overview of all your UiTPAS actions.')),
      'page callback' => 'culturefeed_uitpas_profile_actions_get',
      'access callback' => 'culturefeed_uitpas_is_passholder',
      'file' => 'includes/profile_actions.inc',
    ),
    'culturefeed/profile/uitpas/promotions' => array(
      'title callback' => 'culturefeed_uitpas_profile_promotions_title',
      'description' => variable_get('culturefeed_uitpas_profile_details_menu_promotions_description', t('Discover which promotions and advantages you are entitled to.')),
      'page callback' => 'drupal_get_form',
      'page arguments' => array('culturefeed_uitpas_profile_promotions_filter_sort'),
      'access callback' => 'culturefeed_uitpas_is_passholder',
      'file' => 'includes/profile_promotions.inc',
    ),
    'culturefeed/profile/uitpas/advantages' => array(
      'title callback' => 'culturefeed_uitpas_profile_advantages_title',
      'description' => variable_get('culturefeed_uitpas_profile_details_menu_advantages_description', t('Discover which promotions and advantages you are entitled to.')),
      'page callback' => 'culturefeed_uitpas_profile_advantages_get',
      'access callback' => 'culturefeed_uitpas_is_passholder',
      'file' => 'includes/profile_advantages.inc',
    ),
    'culturefeed/profile/uitpas/notifications' => array(
      'title callback' => 'culturefeed_uitpas_profile_notifications_title',
      'description' => variable_get('culturefeed_uitpas_profile_details_menu_notifications_description', t('Change your preferences for receiving personal notifications via email.')),
      'page callback' => 'culturefeed_uitpas_profile_notifications_get',
      'access callback' => 'culturefeed_uitpas_is_passholder',
      'file' => 'includes/profile_notifications.inc',
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
    'register_where' => array(
      'title callback' => 'culturefeed_uitpas_register_where_title',
      'page callback' => 'culturefeed_uitpas_register_where_get',
      'file' => 'includes/register_where.inc',
      'access callback' => TRUE,
    ),
    'registration_confirmation' => array(
      'title callback' => 'culturefeed_uitpas_registration_confirmation_title',
      'page callback' => 'culturefeed_uitpas_registration_confirmation_get',
      'file' => 'includes/registration_confirmation.inc',
      'access callback' => 'culturefeed_is_culturefeed_user',
      'type' => MENU_CALLBACK,
    ),
    'activate/%/%' => array(
      'title' => 'Activate',
      'page callback' => 'culturefeed_uitpas_activate',
      'page arguments' => array(1, 2),
      'access callback' => TRUE,
      'file' => 'includes/activate.inc',
      'type' => MENU_CALLBACK,
    ),
    'register_uitpas' => array(
      'title callback' => 'culturefeed_uitpas_register_uitpas_title',
      'page callback' => 'culturefeed_uitpas_user_register_get',
      'access callback' => TRUE,
      'file' => 'includes/user_register.inc',
      'type' => MENU_CALLBACK,
    ),
    'culturefeed/uitpas/reminder/toggle' => array(
      'title' => 'Toggle reminder on and off',
      'page callback' => 'culturefeed_uitpas_toggle_reminder',
      'access callback' => 'user_is_logged_in',
      'file' => 'includes/reminder.inc',
      'type' => MENU_CALLBACK,
    ),
  );

}
