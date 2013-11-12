<?php

/**
 * @file
 * Variable module hook implementations
 */

/**
 * Implements hook_variable_info().
 */
function culturefeed_uitpas_variable_info() {

  return array(
    'culturefeed_uitpas_promotions_highlight_more_link_text' => array(
      'title' => t('Culturefeed uitpas: promotions highlight: more link text'),
      'localize' => TRUE,
    ),
    'culturefeed_uitpas_registration_confirmation_actions_label' => array(
      'title' => t('Culturefeed uitpas: registration confirmation: actions label'),
      'localize' => TRUE,
    ),
    'culturefeed_uitpas_registration_confirmation_intro' => array(
      'title' => t('Culturefeed uitpas: registration confirmation: intro'),
      'localize' => TRUE,
    ),
    'culturefeed_uitpas_registration_confirmation_title' => array(
      'title' => t('Culturefeed uitpas: registration confirmation: title'),
      'localize' => TRUE,
    ),
    'culturefeed_uitpas_user_register_intro_text' => array(
      'title' => t('Culturefeed uitpas: user register: intro text'),
      'localize' => TRUE,
    ),
    'culturefeed_uitpas_user_register_number_label' => array(
      'title' => t('Culturefeed uitpas: user register: number label'),
      'localize' => TRUE,
    ),
    'culturefeed_uitpas_user_register_submit_value' => array(
      'title' => t('Culturefeed uitpas: user register: submit value'),
      'localize' => TRUE,
    ),
    'culturefeed_uitpas_user_register_apply_text' => array(
      'title' => t('Culturefeed uitpas: user register: apply text'),
      'localize' => TRUE,
    ),
  );

}
