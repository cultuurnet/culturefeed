<?php

/**
 * @file
 * Helper functions for UiTPAS profile advantages.
 */

/**
 * Returns profile advantages.
 */
function culturefeed_uitpas_profile_promotions_get($filter = NULL, $sort = NULL) {

  $uitpas_user = culturefeed_uitpas_get_uitpas_user();

  $promotions = array();
  $promotions_max = variable_get('culturefeed_uitpas_profile_promotions_max', 20);
  $promotions_page = pager_find_page();
  $promotions_total = 0;

  if ($uitpas_user->user && $uitpas_user->passholder) {

    $card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');
    $uitpas_number = 0;
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
        $query->owningCardSystemId = $filter;
      }

      $query->sort = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::SORT_POINTS;

      if ($sort && $sort == 'cardsystem') {
        $query->orderByOwningCardSystemId = variable_get('culturefeed_uitpas_cardsystem_id');
      }

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

  return array(
    array(
      '#theme' => 'culturefeed_uitpas_profile_promotions',
      '#promotions' => $promotions,
      '#promotions_total' => $promotions_total,
    ),
    array(
      '#theme' => 'pager',
    ),
  );

}

/**
 * Returns form elements for profile advantages settings.
 */
function culturefeed_uitpas_profile_promotions_settings_get(&$form) {

  $form['profile_promotions'] = array(
    '#type' => 'fieldset',
    '#title' => t('UiTPAS profile advantages settings'),
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

  return variable_get('culturefeed_uitpas_profile_details_menu_advantages_title', t('My promotions'));

}

/**
 * Returns form elements for filter & sort on profile promotions page.
 */
function culturefeed_uitpas_profile_promotions_filter_sort($form, $form_state) {

  $form['profile_promotions_link'] = array(
    '#prefix' => '<div id="promotions_link"><ul>',
    '#suffix' => '</ul></div>',
  );

  $form['profile_promotions_link']['promotions'] = array(
    '#markup' => '<li><a href="/culturefeed/profile/uitpas/promotions">' . t('My Promotions') . '</a></li>',
  );

  $form['profile_promotions_link']['advantages'] = array(
    '#markup' => '<li><a href="/culturefeed/profile/uitpas/advantages">' . t('Welcome Advantages') . '</a></li>',
  );

  $uitpas_user = culturefeed_uitpas_get_uitpas_user();

  $card_systems = $uitpas_user->passholder->cardSystemSpecific;

  $options = array();
  foreach ($card_systems as $system) {
    $options[$system->cardSystem->id] = $system->cardSystem->name;
  }

  $id = drupal_html_id('profile-promotions-filter-sort');
  $form['profile_promotions']['#prefix'] = '<div id="' . $id . '">';
  $form['profile_promotions']["#suffix"] = '</div>';

  $form['profile_promotions']['filter'] = array(
    '#type' => 'select',
    '#title' => t('Filter'),
    '#default_value' => isset($filter) ? $filter : 0,
    '#empty_option' => t('All card systems'),
    '#options' => $options,
    '#ajax' => array(
      'event' => 'change',
      'wrapper' => $id,
      'callback' => 'culturefeed_uitpas_profile_promotions_ajax_callback',
    ),
  );

  $form['profile_promotions']['sort'] = array(
    '#type' => 'select',
    '#title' => t('Sort'),
    '#default_value' => isset($sort) ? $sort : 0,
    '#options' => array(
      'points' => t('Points'),
      'cardsystem' => t('Owning card system'),
    ),
    '#ajax' => array(
      'event' => 'change',
      'wrapper' => $id,
      'callback' => 'culturefeed_uitpas_profile_promotions_ajax_callback',
    ),
  );

  $filter = isset($form_state['values']['filter']) ? $form_state['values']['filter'] : NULL;
  $sort = isset($form_state['values']['sort']) ? $form_state['values']['sort'] : NULL;
  $results = culturefeed_uitpas_profile_promotions_get($filter, $sort);
  $form['profile_promotions']['result'] = array(
    '#markup' => drupal_render($results),
  );

  $form['profile_promotions']['#attached']['css'][] = drupal_get_path('module', 'culturefeed_uitpas') . '/css/culturefeed_uitpas.css';

  return $form;
}

function culturefeed_uitpas_profile_promotions_ajax_callback($form, $form_state) {
  $form_state['rebuild'] = TRUE;
  return $form['profile_promotions'];
}
