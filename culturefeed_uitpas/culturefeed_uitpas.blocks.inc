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
      $promotions = array();
      try {
        $cf = DrupalCultureFeed::getConsumerInstance();
        $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
        $query->sort = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::SORT_CASHING_PERIOD_END;
        $query->max = variable_get('culturefeed_uitpas_promotions_highlight_max', 5);
        $query->cashingPeriodBegin = time();
        $query->unexpired = TRUE;
        $result = $cf->uitpas()->getPromotionPoints($query);
        if ($result->total) {
          $promotions = $result->objects;
        }
      }
      catch (Exception $e) {
        watchdog_exception('culturefeed_uitpas_block_promotions_highlight', $e);
      }

      if (count($promotions)) {

        $block['subject'] = t("Spotlight promotions");
        $block['content'] = array(
          '#theme' => 'culturefeed_uitpas_promotions_highlight',
          '#promotions' => $promotions,
        );

      }
      break;

    case 'uitpas_recent_actions':
      $actions = array();
      try {

        $cf = DrupalCultureFeed::getConsumerInstance();
        $query = new CultureFeed_SearchActivitiesQuery();
        $query->type = CultureFeed_Activity::TYPE_UITPAS;
        $query->private = FALSE;
        $query->max = variable_get('culturefeed_uitpas_recent_actions_max', 5);
        $result = $cf->searchActivities($query);
        if ($result->total) {
          $actions = $result->objects;
        }

      }
      catch (Exception $e) {
        watchdog_exception('culturefeed_uitpas_block_recent_actions', $e);
      }

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

        $block['subject'] = t('Already own an Uitpas ?');
        $form = drupal_get_form('culturefeed_uitpas_user_register_form', $uitpas_user);
        $block['content'] = $form;

      }
      break;

  }

  return $block;

}
