<?php

/**
 * @file
 * Helper functions for UiTPAS profile details.
 */

/**
 * Returns profile details.
 */
function culturefeed_uitpas_profile_details_get() {

  $uitpas_user = culturefeed_uitpas_get_uitpas_user();

  if ($uitpas_user->user && $uitpas_user->passholder) {

    return array(
      '#theme' => 'culturefeed_uitpas_profile_details',
      '#uitpas_user' => $uitpas_user,
    );

  }

}

/**
 * Returns form elements for profile details settings.
 */
function culturefeed_uitpas_profile_details_settings_get(&$form) {

  $form['profile_details'] = array(
    '#type' => 'fieldset',
    '#title' => t('UiTPAS profile details settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['profile_details']['culturefeed_uitpas_profile_details_menu_advantages_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu: advantages title'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_details_menu_advantages_title', t('My advantages')),
  );
  $form['profile_details']['culturefeed_uitpas_profile_details_menu_advantages_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Menu: advantages description'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_details_menu_advantages_description', t('Discover which promotions and advantages you are entitled to.')),
  );
  $form['profile_details']['culturefeed_uitpas_profile_details_menu_actions_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu: actions title'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_details_menu_actions_title', t('User history')),
  );
  $form['profile_details']['culturefeed_uitpas_profile_details_menu_actions_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Menu: actions description'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_details_menu_actions_description', t('An overview of all your UiTPAS actions.')),
  );
  $form['profile_details']['culturefeed_uitpas_profile_details_menu_notifications_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu: notifications title'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_details_menu_notifications_title', t('My notifications')),
  );
  $form['profile_details']['culturefeed_uitpas_profile_details_menu_notifications_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Menu: notifications description'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_details_menu_notifications_description', t('Change your preferences for receiving personal notifications via email.')),
  );
  $form['profile_details']['culturefeed_uitpas_profile_details_intro'] = array(
    '#type' => 'textarea',
    '#title' => t('Intro'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_details_intro'),
  );
  $form['profile_details']['culturefeed_uitpas_profile_details_outro'] = array(
    '#type' => 'textarea',
    '#title' => t('Outro'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_details_outro'),
  );

}
