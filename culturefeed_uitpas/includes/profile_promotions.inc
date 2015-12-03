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

    $card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');

    if ($card_system_id && isset($uitpas_user->passholder->cardSystemSpecific[$card_system_id]->currentCard->uitpasNumber)) {
      $uitpas_number = $uitpas_user->passholder->cardSystemSpecific[$card_system_id]->currentCard->uitpasNumber;
    }

    // Promotions.
    try {

      $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
      $query->uitpasNumber = $uitpas_number;
      $query->cashingPeriodBegin = time();
      $query->unexpired = TRUE;

      if ($filter) {
        if (strpos($filter, '+') === FALSE) {
          $query->owningCardSystemId = $filter;
        }
        else {
          $query->applicableCardSystemId = explode('+', $filter);
        }
      }
      $query->orderByOwningCardSystemId = variable_get('culturefeed_uitpas_cardsystem_id');
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
    $singular = 'My promotions (1 point saved)';
    $plural = 'My promotions (@count points saved)';
    drupal_set_title(format_plural($uitpas_user->passholder->points, $singular, $plural), PASS_THROUGH);

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

  // Get the card systems for the passholder.
  $options = array();
  $uitpas_user = culturefeed_uitpas_get_uitpas_user();
  /* @var \CultureFeed_Uitpas_Passholder $passholder */
  $passholder = $uitpas_user->passholder;
  foreach ($passholder->cardSystemSpecific as $card_system_specific) {
    $card_system = $card_system_specific->cardSystem;
    $options[$card_system->id] = $card_system->name;
  }

  // Get the remaining card systems.
  $card_systems = culturefeed_uitpas_get_card_systems();
  array_walk($card_systems, function (&$card_system) {
    /* @var CultureFeed_Uitpas_Cardsystem $card_system */
    $card_system = $card_system->id;
  });
  $card_systems = array_diff($card_systems, array_keys($options));
  $options[implode('+', $card_systems)] = t('other');

  $id = drupal_html_id('profile-promotions-filter-sort');
  $form['profile_promotions']['#prefix'] = '<div id="' . $id . '">';
  $form['profile_promotions']["#suffix"] = '</div>';

  $filter = isset($form_state['values']['filter']) ? $form_state['values']['filter'] : (isset($_GET['filter']) ? $_GET['filter'] : NULL);

  $form['profile_promotions']['filter'] = array(
    '#type' => 'radios',
    '#title' => t('Filter'),
    '#default_value' => $filter,
    '#empty_option' => t('All card systems'),
    '#options' => $options,
    '#ajax' => array(
      'event' => 'change',
      'wrapper' => $id,
      'callback' => 'culturefeed_uitpas_profile_promotions_ajax_callback',
    ),
  );

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
