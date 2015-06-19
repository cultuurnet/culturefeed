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
    $query->published = TRUE;
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
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_uitpas_promotions', $e);
  }

  $promotions_pager_min = ($promotions_page * $promotions_max) + 1;
  $promotions_pager_max = ($promotions_page * $promotions_max) + $promotions_max;
  if ($promotions_pager_max > $promotions_total) {
    $promotions_pager_max = $promotions_total;
  }

  return array(
    '#theme' => 'culturefeed_uitpas_promotions',
    '#promotions' => $promotions,
    '#promotions_total' => $promotions_total,
    '#promotions_pager_min' => $promotions_pager_min,
    '#promotions_pager_max' => $promotions_pager_max,
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
  $form['promotions']['culturefeed_uitpas_show_advantages_above_promotions']= array(
    '#type' => 'checkbox',
    '#title' => t('Show advantages above the promotions list.'),
    '#default_value' => variable_get('culturefeed_uitpas_show_advantages_above_promotions', TRUE),
  );
  $form['promotions']['culturefeed_uitpas_number_advantages_above_promotions'] = array(
    '#type' => 'select',
    '#title' => t('Number of advantages to show above the promotions list'),
    '#default_value' => variable_get('culturefeed_uitpas_number_advantages_above_promotions', 2),
    '#options' => range(0, 15),
    '#states' => array(
      'visible' => array(
        ':input[name="culturefeed_uitpas_show_advantages_above_promotions"]' =>array('checked' => TRUE)
      ),
    ),
  );

}

/**
 * Returns form elements for filter & sort on promotions page.
 */
function culturefeed_uitpas_promotions_filter_sort($form, $form_state) {

  $cf = DrupalCultureFeed::getConsumerInstance();
  $kind_of_card_systems = 'permanent';
  $card_systems = $cf->uitpas()->getCardSystems($kind_of_card_systems);

  $filter = isset($form_state['values']['filter']) ? $form_state['values']['filter'] : (isset($_GET['filter']) ? $_GET['filter'] : NULL);
  $promotions = culturefeed_uitpas_promotions_get($filter);

  $options = array();
  foreach ($card_systems as $system) {
    $options[$system->id] = $system->name;
  }

  $id = drupal_html_id('promotions-filter-sort');

  $form['promotions']['#prefix'] = "<div id=\"" . $id . "\">";
  $form['promotions']["#suffix"] = "</div>";

  $promotions_intro = variable_get('culturefeed_uitpas_promotions_info');
  if (trim($promotions_intro)) {
    $form['promotions']['intro']['#markup'] = '<div class="promotions-intro">' . $promotions_intro . '</div>';
  }

  $form['promotions']['filter'] = array(
    '#type' => 'radios',
    '#title' => t('Filter by'),
    '#default_value' => isset($filter) ? $filter : 0,
    '#empty_option' => t('All card systems'),
    '#options' => $options,
    '#theme' => 'culturefeed_uitpas_filter_labels',
    '#ajax' => array(
      'event' => 'change',
      'wrapper' => $id,
      'callback' => 'culturefeed_uitpas_promotions_ajax_callback',
    ),
  );
  $title = t('Found %count promotions', array('%count' => $promotions['#promotions_total']));
  $filter_reset_link = l(t('Show all'), '/promotions');
  if ($filter) {
    $form['promotions']['title']['#markup'] = '<h2>' . $title . ' <small>' . $options[$filter] . ' (' . $filter_reset_link . ')</small></h2>';
  }
  else {
    $form['promotions']['title']['#markup'] = '<h2>' . $title . '</h2>';
  }

  // Only show the advantages if the settings allow it.
  // Only show the advantages on the first page.
  if (variable_get('culturefeed_uitpas_show_advantages_above_promotions', TRUE) &&
      ((isset($_GET['page']) && $_GET['page'] == 0) || !isset($_GET['page']))) {
    module_load_include('inc', 'culturefeed_uitpas', 'includes/advantages');
    $advantages = culturefeed_uitpas_advantages_get();
    if (count($advantages['#advantages'])) {
      $advantages['#advantages'] = array_slice($advantages['#advantages'], 0, variable_get('culturefeed_uitpas_number_advantages_above_promotions', 2));
      $advantages['#advantages_total'] = variable_get('culturefeed_uitpas_number_advantages_above_promotions', 2);
      $form['promotions']['advantages']['#markup'] = theme('culturefeed_uitpas_advantages_table', $advantages);
      $form['promotions']['advantages']['#markup'] .= '<p class="link link-more">' . l(t('Show all advantages'), '/advantages') . '<i class="fa fa-angle-right"></i></p>';
    }
  }

  if (count($promotions['#promotions'])) {
    $form['promotions']['result']['#markup'] = theme('culturefeed_uitpas_promotions_table', $promotions);
  }
  else {
    $form['promotions']['result']['#markup'] = '<div class="no-results no-promotions">' . t('No results found') . '</div>';
  }

  // Pager.
  $promotions_max = variable_get('culturefeed_uitpas_promotions_promotions_max', 20);

  pager_default_initialize($promotions['#promotions_total'], $promotions_max);

  // Solution csdco on from https://www.drupal.org/node/1049354#comment-6253362
  $tmp_q = $_GET['q'];
  $_GET['q'] = 'promotions';
  $form['promotions']['pager']['#markup'] = '<div class="pager clearfix">' . theme('pager', array('parameters' => array('filter' => $filter))) . '</div>';
  $_GET['q'] = $tmp_q;

  return $form;

}

function culturefeed_uitpas_promotions_ajax_callback($form, $form_state) {
  return $form['promotions'];
}
