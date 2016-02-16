<?php

/**
 * @file
 * Helper functions for UiTPAS profile advantages.
 */

/**
 * Returns profile advantages.
 */
function culturefeed_uitpas_profile_promotions_get($filter = NULL) {

  $uitpas_user = culturefeed_uitpas_get_uitpas_user();

  $promotions = array();
  $promotions_max = variable_get('culturefeed_uitpas_profile_promotions_max', 20);
  $promotions_page = pager_find_page();
  $promotions_total = 0;

  if ($uitpas_user->user && $uitpas_user->passholder) {

    /* @var \CultureFeed_Uitpas_Passholder $passholder */
    $passholder = $uitpas_user->passholder;

    // Promotions.
    try {

      $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
      $query->cashingPeriodBegin = time();
      $query->unexpired = TRUE;
      $query->filterOnUserPoints = TRUE;
      $query->uid = $passholder->uitIdUser->id;

      if ($filter) {
        if (strpos($filter, '+') === FALSE) {
          $query->owningCardSystemId = $filter;
        }
        else {
          $query->applicableCardSystemId = array_slice(explode('+', $filter), 1);
        }
      }
      $card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');
      if ($card_system_id) {
        $query->orderByOwningCardSystemId = $card_system_id;
      }
      $query->sort = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::SORT_POINTS;

      $query->order = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::ORDER_ASC;

      $query->start = $promotions_page * $promotions_max;
      $query->max = $promotions_max;
      $result = $uitpas_user->cf->uitpas()->getPromotionPoints($query);
      $promotions = $result->objects;
      pager_default_initialize($result->total, $promotions_max);
      $promotions_total = $result->total;

    }
    catch (Exception $e) {

      watchdog_exception('culturefeed_uitpas_profile_promotions', $e);
      pager_default_initialize(0, $promotions_max);

    }

    // Title.
    drupal_set_title(t('My promotions'), PASS_THROUGH);

  }

  // Solution csdco on from https://www.drupal.org/node/1049354
  $_GET['q'] = 'culturefeed/profile/uitpas/promotions';

  return array(
    array(
      '#theme' => 'culturefeed_uitpas_profile_promotions',
      '#promotions' => $promotions,
      '#promotions_total' => $promotions_total,
      '#promotions_pager_min' => ($promotions_page * $promotions_max) + 1,
      '#promotions_pager_max' => ($promotions_page * $promotions_max) + $promotions_max,
    ),
    array(
      '#theme' => 'pager',
      '#parameters' => array('filter' => $filter),
    ),
  );

}

/**
 * Returns form elements for profile advantages settings.
 */
function culturefeed_uitpas_profile_promotions_settings_get(&$form) {

  $form['profile_promotions'] = array(
    '#type' => 'fieldset',
    '#title' => t('My UiTPAS promotions settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['profile_promotions']['culturefeed_uitpas_profile_promotions_max'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of promotions to display'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_promotions_max', 20),
  );

}

/**
 * Title callback for the profile advantages page.
 */
function culturefeed_uitpas_profile_promotions_title() {

  return t('My promotions');

}

/**
 * Returns form elements for filter & sort on profile promotions page.
 */
function culturefeed_uitpas_profile_promotions_filter_sort($form, $form_state) {

  $form['profile_promotions_link'] = array(
    '#attributes' => array(
      'id' => 'promotions_link'
    ),
    '#links' => array(
      'promotions' => array(
        'href' => 'culturefeed/profile/uitpas/promotions',
        'title' => t('My Promotions'),
      ),
      'advantages' => array(
        'href' => 'culturefeed/profile/uitpas/advantages',
        'title' => t('Welcome Advantages'),
      ),
    ),
    '#theme' => 'links',
  );

  // We can show promotions in other regions than owning card system.
  $other_regions = TRUE;

  // Get the card systems for the passholder.
  $options = culturefeed_uitpas_get_user_card_systems_as_options($other_regions);

  $id = drupal_html_id('profile-promotions-filter-sort');
  $form['profile_promotions']['#prefix'] = '<div class="filters filters-regions" id="' . $id . '">';
  $form['profile_promotions']["#suffix"] = '</div>';

  $filter = isset($form_state['values']['filter']) ? $form_state['values']['filter'] : (isset($_GET['filter']) ? $_GET['filter'] : NULL);

  if (count($options) > 1) {
    $form['profile_promotions']['filter'] = array(
      '#type' => 'radios',
      '#title' => t('Distributed by'),
      '#default_value' => $filter,
      '#empty_option' => t('All card systems'),
      '#options' => $options,
      '#ajax' => array(
        'event' => 'change',
        'wrapper' => $id,
        'callback' => 'culturefeed_uitpas_profile_promotions_ajax_callback',
      ),
    );
  }

  $results = culturefeed_uitpas_profile_promotions_get($filter);
  $form['profile_promotions']['result'] = array(
    '#markup' => drupal_render($results),
  );

  $form['profile_promotions']['#attached']['css'][] = drupal_get_path('module', 'culturefeed_uitpas') . '/css/culturefeed_uitpas.css';

  return $form;
}

function culturefeed_uitpas_profile_promotions_ajax_callback($form, $form_state) {
  return $form['profile_promotions'];
}
