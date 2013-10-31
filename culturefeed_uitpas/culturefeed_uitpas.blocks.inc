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

    case 'uitpas_promotions_highlight':
      $form['max'] = array(
        '#type' => 'textfield',
        '#title' => t('Number of promotions to display'),
        '#default_value' => variable_get('culturefeed_uitpas_promotions_highlight_max', 5),
      );
      $form['more_link_text'] = array(
        '#type' => 'textfield',
        '#title' => t('More link text'),
        '#default_value' => variable_get('culturefeed_uitpas_promotions_highlight_more_link_text', t('Show all promotions')),
      );
      break;

    case 'uitpas_recent_actions':
      $form['max'] = array(
        '#type' => 'textfield',
        '#title' => t('Number of actions to display'),
        '#default_value' => variable_get('culturefeed_uitpas_recent_actions_max', 5),
      );
      break;

  }

  return $form;

}

/**
 * Implements hook_block_save().
 */
function culturefeed_uitpas_block_save($delta = '', $edit = array()) {

  switch ($delta) {

    case 'uitpas_promotions_highlight':
      variable_set('culturefeed_uitpas_promotions_highlight_max', $edit['max']);
      variable_set('culturefeed_uitpas_promotions_highlight_more_link_text', $edit['more_link_text']);
      break;

    case 'uitpas_recent_actions':
      variable_set('culturefeed_uitpas_recent_actions_max', $edit['max']);
      break;

  }

}

/**
 * Implements hook_block_view().
 */
function culturefeed_uitpas_block_view($delta = '') {

  $block = array();

  switch ($delta) {

    case 'uitpas_promotions_highlight':
      $promotions = culturefeed_uitpas_get_promotions_highlight();
      if (count($promotions)) {

        $block['subject'] = t("Spotlight promotions");
        $block['content'] = array(
          '#theme' => 'culturefeed_uitpas_promotions_highlight',
          '#promotions' => $promotions,
        );

      }
      break;

    case 'uitpas_recent_actions':
      $actions = culturefeed_uitpas_get_recent_actions();
      if (count($actions)) {

        $block['subject'] = t('Recent actions');
        $block['content'] = array(
          '#theme' => 'culturefeed_uitpas_recent_actions',
          '#actions' => $actions,
        );

      }
      break;

    case 'uitpas_user_register':
      $uitpas_user = culturefeed_uitpas_get_uitpas_user();
      if (!user_is_logged_in() || !$uitpas_user->passholder) {

        $block['subject'] = t('Allready own an Uitpas ?');
        $form = drupal_get_form('culturefeed_uitpas_user_register_form', $uitpas_user);
        $block['content'] = $form;

      }
      break;

  }

  return $block;

}
