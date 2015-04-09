<?php

/**
 * @file
 * Helper functions for UiTPAS promotions.
 */

/**
 * Returns promotions.
 */
function culturefeed_uitpas_promotions_get($filter = NULL) {

  $promotions = array();
  $promotions_max = variable_get('culturefeed_uitpas_promotions_promotions_max', 20);
  $promotions_page = pager_find_page();
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
    $card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');
    if ($card_system_id) {
      $query->applicableCardSystemId = $card_system_id;
    }
    if ($filter) {
      $query->owningCardSystemId = $filter;
    }
    $query->orderByOwningCardSystemId = variable_get('culturefeed_uitpas_cardsystem_id');
    $query->sort = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::SORT_POINTS;

    $query->order = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::ORDER_ASC;
    $result = $cf->uitpas()->getPromotionPoints($query);

    $promotions = $result->objects;
    $promotions_total = $result->total;

    foreach ($promotions as $key => $promotion) {
      if ($promotion->publicationPeriodBegin != NULL && $promotion->publicationPeriodBegin > time()) {
        unset($promotions[$key]);
        $promotions_total--;
      }
      if ($promotion->publicationPeriodEnd != NULL && $promotion->publicationPeriodEnd < time()) {
        unset($promotions[$key]);
        $promotions_total--;
      }
    }

  }
  catch (Exception $e) {

    watchdog_exception('culturefeed_uitpas_promotions', $e);

  }

  return array(
    '#theme' => 'culturefeed_uitpas_promotions',
    '#promotions' => $promotions,
    '#promotions_total' => $promotions_total,
    '#promotions_pager_min' => ($promotions_page * $promotions_max) + 1,
    '#promotions_pager_max' => ($promotions_page * $promotions_max) + $promotions_max,
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

  $form['promotions_link'] = array(
    '#prefix' => '<div id="promotions_link"><ul>',
    '#suffix' => '</ul></div>',
  );

  $form['promotions_link']['promotions'] = array(
    '#markup' => '<li><a href="/promotions">' . t('Promotions') . '</a></li>',
  );

  $form['promotions_link']['advantages'] = array(
    '#markup' => '<li><a href="/advantages">' . t('Welcome Advantages') . '</a></li>',
  );

  $cf = DrupalCultureFeed::getConsumerInstance();

  $card_systems = $cf->uitpas()->getCardSystems();

  $filter = isset($form_state['values']['filter']) ? $form_state['values']['filter'] : (isset($_GET['filter']) ? $_GET['filter'] : NULL);

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

  $results = culturefeed_uitpas_promotions_get($filter);
  $form['promotions']['result'] = array(
    '#markup' => drupal_render($results),
  );

  // Pager.
  $promotions_max = variable_get('culturefeed_uitpas_promotions_promotions_max', 20);

  pager_default_initialize($results['#promotions_total'], $promotions_max);

  // Solution csdco on from https://www.drupal.org/node/1049354
  $tmp_q = $_GET['q'];
  $_GET['q'] = 'promotions';
  $form['promotions']['pager']['#markup'] = theme('pager', array('parameters' => array('filter' => $filter)));
  $_GET['q'] = $tmp_q;

  $form['promotions']['#attached']['css'][] = drupal_get_path('module', 'culturefeed_uitpas') . '/css/culturefeed_uitpas.css';

  return $form;

}

function culturefeed_uitpas_promotions_ajax_callback($form, $form_state) {
  return $form['promotions'];
}