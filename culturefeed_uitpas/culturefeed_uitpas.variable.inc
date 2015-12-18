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
    'culturefeed_uitpas_profile_notifications_intro' => array(
      'title' => t('Culturefeed uitpas: profile notifications: intro'),
      'localize' => TRUE,
    ),
    'culturefeed_uitpas_promotions_highlight_more_link_text' => array(
      'title' => t('Culturefeed uitpas: promotions highlight: more link text'),
      'localize' => TRUE,
    ),
  );

}
