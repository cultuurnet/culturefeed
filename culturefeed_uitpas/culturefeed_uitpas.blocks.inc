<?php

/**
 * @file
 * Block implementations for Culturefeed uitpas.
 */

/**
 * Implements hook_block_info().
 */
function culturefeed_uitpas_block_info() {

  return array(
    'uitpas_event_details_advantages' => array(
      'info' => t('Culturefeed uitpas: event details advantages'),
      'cache' => DRUPAL_NO_CACHE,
    ),
    'uitpas_profile_summary' => array(
      'info' => t('Culturefeed uitpas: profile (summary)'),
      'cache' => DRUPAL_NO_CACHE,
    ),
    'uitpas_promotions_highlight' => array(
      'info' => t('Culturefeed uitpas: promotions highlight'),
      'cache' => DRUPAL_NO_CACHE,
    ),
    'uitpas_recent_actions' => array(
      'info' => t('Culturefeed uitpas: recent actions'),
      'cache' => DRUPAL_NO_CACHE,
    ),
    'uitpas_user_register' => array(
      'info' => t('Culturefeed uitpas: user register'),
      'cache' => DRUPAL_NO_CACHE,
    ),
  );

}

/**
 * Implements hook_block_configure().
 */
function culturefeed_uitpas_block_configure($delta = '') {

  $form = array();

  switch ($delta) {

    case 'uitpas_profile_summary':
      module_load_include('inc', 'culturefeed_uitpas', 'includes/profile_summary');
      culturefeed_uitpas_profile_summary_settings_get($form);
      break;

    case 'uitpas_promotions_highlight':
      module_load_include('inc', 'culturefeed_uitpas', 'includes/promotions_highlight');
      culturefeed_uitpas_promotions_highlights_settings_get($form);
      break;

    case 'uitpas_recent_actions':
      module_load_include('inc', 'culturefeed_uitpas', 'includes/recent_actions');
      culturefeed_uitpas_recent_actions_settings_get($form);
      break;

  }

  return $form;

}

/**
 * Implements hook_block_save().
 */
function culturefeed_uitpas_block_save($delta = '', $edit = array()) {

  switch ($delta) {

    case 'uitpas_profile_summary':
      module_load_include('inc', 'culturefeed_uitpas', 'includes/profile_summary');
      culturefeed_uitpas_profile_summary_settings_save($edit);
      break;

    case 'uitpas_promotions_highlight':
      module_load_include('inc', 'culturefeed_uitpas', 'includes/promotions_highlight');
      culturefeed_uitpas_promotions_highlights_settings_save($edit);
      break;

    case 'uitpas_recent_actions':
      module_load_include('inc', 'culturefeed_uitpas', 'includes/recent_actions');
      culturefeed_uitpas_recent_actions_settings_save($edit);
      break;

  }

}

/**
 * Implements hook_block_view().
 */
function culturefeed_uitpas_block_view($delta = '') {

  $block = array();

  switch ($delta) {

    case 'uitpas_event_details_advantages':
      module_load_include('inc', 'culturefeed_uitpas', 'includes/event_details_advantages');
      $content = culturefeed_uitpas_event_details_advantages_get();

      if ($content) {

        $block['subject'] = t("UiTPAS Advantages");
        $block['content'] = $content;
      }
      break;

    case 'uitpas_profile_summary':
      module_load_include('inc', 'culturefeed_uitpas', 'includes/profile_summary');
      $content = culturefeed_uitpas_profile_summary_get();
      if ($content) {

        $block['subject'] = t("My UiTPAS");
        $block['content'] = $content;

      }
      break;

    case 'uitpas_promotions_highlight':
      module_load_include('inc', 'culturefeed_uitpas', 'includes/promotions_highlight');
      $content = culturefeed_uitpas_promotions_highlight_get();
      if ($content) {

        $block['subject'] = t("Spotlight promotions");
        $block['content'] = $content;

      }
      break;

    case 'uitpas_recent_actions':
      module_load_include('inc', 'culturefeed_uitpas', 'includes/recent_actions');
      $content = culturefeed_uitpas_recent_actions_get();
      if ($content) {

        $block['subject'] = t('Recent actions');
        $block['content'] = $content;

      }
      break;

    case 'uitpas_user_register':
      module_load_include('inc', 'culturefeed_uitpas', 'includes/user_register');     
      $content = culturefeed_uitpas_user_register_cta_get();
      if ($content) {

        $block['subject'] = t('Already own an UiTPAS?');
        $block['content'] = $content;

      }
      break;

  }

  return $block;

}
