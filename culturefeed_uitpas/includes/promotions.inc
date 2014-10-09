<?php

/**
 * @file
 * Helper functions for UiTPAS promotions.
 */

/**
 * Returns promotions.
 */
function culturefeed_uitpas_promotions_get($filter = NULL, $sort = NULL) {

  $promotions = array();
  $promotions_max = variable_get('culturefeed_uitpas_promotions_promotions_max', 20);
  $promotions_pager_element = 1;
  $promotions_page = pager_find_page($promotions_pager_element);
  $promotions_total = 0;

  try {

    $cf = DrupalCultureFeed::getConsumerInstance();

    // Promotions.
    $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
    $query->start = $promotions_page * $promotions_max;
    $query->max = $promotions_max;
    $query->unexpired = TRUE;
    $query->cashingPeriodBegin = time();
    $query->cashInState = array(
      CultureFeed_Uitpas_Passholder_PointsPromotion::CASHIN_POSSIBLE,
      // Also show items that are not any longer in stock.
      CultureFeed_Uitpas_Passholder_PointsPromotion::CASHIN_NOT_POSSIBLE_VOLUME_CONSTRAINT,
    );
    $query->published = TRUE;
    $card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');
    if ($card_system_id) {
      $query->applicableCardSystemId = $card_system_id;
    }
    if ($filter) {
      $query->owningCardSystemId = $filter;
    }
    $query->sort = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::SORT_POINTS;
    if ($sort && $sort == 'cardsystem') {
      $query->sort = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::SORT_TITLE;
    }
    $query->order = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::ORDER_ASC;
    $result = $cf->uitpas()->getPromotionPoints($query);
    pager_default_initialize($result->total, $promotions_max, $promotions_pager_element);
    $promotions = $result->objects;
    $promotions_total = $result->total;

    foreach ($promotions as $key => $promotion) {
      if ($promotion->publicationPeriodBegin != NULL && $promotion->publicationPeriodBegin > time()) {
        unset($promotions[$key]);
      }
    }

  }
  catch (Exception $e) {

    watchdog_exception('culturefeed_uitpas_promotions', $e);
    pager_default_initialize(0, $promotions_max, $promotions_pager_element);

  }

  return array(
    '#theme' => 'culturefeed_uitpas_promotions',
    '#promotions' => $promotions,
    '#promotions_total' => $promotions_total,
    '#promotions_pager_element' => $promotions_pager_element,
    //'#promotions_filter' => $promotions_filter,
  );

}

/**
 * Returns form elements for advantages promotions settings.
 */
function culturefeed_uitpas_promotions_settings_get(&$form) {

  $form['promotions'] = array(
    '#type' => 'fieldset',
    '#title' => t('UiTPAS advantages promotions settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['promotions']['culturefeed_uitpas_promotions_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Page title'),
    '#default_value' => variable_get('culturefeed_uitpas_promotions_title', 'UiTPAS promotions'),
  );
  $form['promotions']['culturefeed_uitpas_promotions_info'] = array(
    '#type' => 'textarea',
    '#title' => t('Info text'),
    '#default_value' => variable_get('culturefeed_uitpas_promotions_info'),
  );
  $form['promotions']['culturefeed_uitpas_promotions_promotions_max'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of promotions to display'),
    '#default_value' => variable_get('culturefeed_uitpas_promotions_promotions_max', 20),
  );

}

/**
 * Returns form elements for filter & sort on promotions page.
 */
function culturefeed_uitpas_promotions_filter_sort($form, $form_state) {
  $cf = DrupalCultureFeed::getConsumerInstance();

  $card_systems = $cf->uitpas()->getCardSystems();

  $options = array();
  foreach ($card_systems as $system) {
    $options[$system->id] = $system->name;
  }

  $id = drupal_html_id('promotions-filter-sort');

  $form['promotions']['#prefix'] = "<div id=\"" . $id . "\">";
  $form['promotions']["#suffix"] = "</div>";

  $form['promotions']['filter'] = array(
    '#type' => 'select',
    '#title' => t('Filter'),
    '#default_value' => isset($filter) ? $filter : 0,
    '#empty_option' => t('All card systems'),
    '#options' => $options,
    '#ajax' => array(
      'event' => 'change',
      'wrapper' => $id,
      'callback' => 'culturefeed_uitpas_promotions_ajax_callback',
    ),
  );

  $form['promotions']['sort'] = array(
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
      'callback' => 'culturefeed_uitpas_promotions_ajax_callback',
    ),
  );

  $filter = isset($form_state['values']['filter']) ? $form_state['values']['filter'] : NULL;
  $sort = isset($form_state['values']['sort']) ? $form_state['values']['sort'] : NULL;
  $results = culturefeed_uitpas_promotions_get($filter, $sort);
  $form['promotions']['result'] = array(
    '#markup' => drupal_render($results),
  );

  return $form;
}

function culturefeed_uitpas_promotions_ajax_callback($form, $form_state) {
  return $form['promotions'];
}