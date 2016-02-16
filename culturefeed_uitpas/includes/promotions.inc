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
    $card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');
    if ($card_system_id) {
      $query->orderByOwningCardSystemId = $card_system_id;
    }
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
    '#title' => t('UiTPAS promotions settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
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

  // If advantages aren't shown above promotions, fall back to links to toggle between both
  if (variable_get('culturefeed_uitpas_show_advantages_above_promotions') == FALSE) {
    $form['promotions_link'] = array(
      '#attributes' => array(
        'id' => 'promotions_link'
      ),
      '#links' => array(
        'promotions' => array(
          'href' => 'promotions',
          'title' => t('Promotions'),
        ),
        'advantages' => array(
          'href' => 'advantages',
          'title' => t('Welcome Advantages'),
        ),
      ),
      '#theme' => 'links',
    );
  }

  $card_systems = culturefeed_uitpas_get_card_systems();
  $multiple_card_systems = culturefeed_uitpas_multiple_cardsystems();

  if ($multiple_card_systems) {
    $filter = isset($form_state['values']['filter']) ? $form_state['values']['filter'] : (isset($_GET['filter']) ? $_GET['filter'] : NULL);
  }
  // If only one cardsystem is enabled, use default filter option
  else {
    $filter = $card_systems[0]->id;
  }
  $promotions = culturefeed_uitpas_promotions_get($filter);

  $advantages = array();
  if (variable_get('culturefeed_uitpas_show_advantages_above_promotions', TRUE)) {
    module_load_include('inc', 'culturefeed_uitpas', 'includes/advantages');
    $advantages = culturefeed_uitpas_advantages_get($filter);
  }

  $id = drupal_html_id('promotions-filter-sort');

  $form['promotions']['#prefix'] = "<div class=\"filters filters-regions\" id=\"" . $id . "\">";
  $form['promotions']["#suffix"] = "</div>";

  $promotions_intro = t('This page shows all promotions for which UiTPAS holders can save points.');
  
  if (culturefeed_uitpas_is_passholder()) {
   $promotions_intro .= ' '. t('Discover <a href="@my-promotions">your personal promotions</a>  and points balance.', array('@my-promotions' => url('culturefeed/profile/uitpas/promotions')));
  }
  else {
    $promotions_intro .= ' '. t('<a href="@register-where">Find out where</a> to get an UiTPAS', array('@register-where' => url('register_where'))); 
    if (!culturefeed_is_culturefeed_user()) {
      $promotions_intro .= ' '. t('or <a href="@login">login</a> and register your UiTPAS to discover your personal promotions and points balance.', array('@login' => url('culturefeed/oauth/connect', array('query' => array('destination' => 'register_uitpas'))))); 
    }
    else {
      $promotions_intro .= '.';
    }
  }

  if (trim($promotions_intro)) {
    $form['promotions']['intro']['#markup'] = '<div class="intro intro-promotions">' . $promotions_intro . '</div>';
  }

  if ($multiple_card_systems) {
  
    $options = array();
    foreach ($card_systems as $system) {
      $options[$system->id] = $system->name;
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

    if (isset($filter)) {
      $filter_reset_link = '<div class="filter-reset-link">' . l(t('Show all promotions'), 'promotions', array('attributes' => array('class' => array('reset-filter')))) . '</div>';
    }
    else {
      $filter_reset_link = '';
    }

    $form['promotions']['filter']['#prefix'] = '<div class="form-group">';
    $form['promotions']['filter']['#suffix'] = $filter_reset_link . '</div>';

  }

  $total = $promotions['#promotions_total'];
  if (isset($advantages['#advantages_total'])) {
    $total += $advantages['#advantages_total'];
  }
  $title = t('Found %count promotions', array('%count' => $total));
  $filter_reset_link = l('&times;', 'promotions', array('attributes' => array('class' => array('reset-filter-inline')), 'html' => TRUE));
  if ($filter && $multiple_card_systems) {
    $form['promotions']['title']['#markup'] = '<h3 class="results-title">' . $title . ' <small>' . $options[$filter] . ' ' . $filter_reset_link . '</small></h3>';
  }
  else {
    $form['promotions']['title']['#markup'] = '<h3 class="results-title">' . $title . '</h3>';
  }

  // Only show the advantages if the settings allow it.
  // Only show the advantages on the first page.
  if (
    variable_get('culturefeed_uitpas_show_advantages_above_promotions', TRUE) &&
    ((isset($_GET['page']) && $_GET['page'] == 0) || !isset($_GET['page'])) &&
    count($advantages['#advantages'])
  ) {
    $advantages['#advantages'] = array_slice($advantages['#advantages'], 0, variable_get('culturefeed_uitpas_number_advantages_above_promotions', 2));
    $advantages['#advantages_total'] = variable_get('culturefeed_uitpas_number_advantages_above_promotions', 2);
    $form['promotions']['advantages']['#markup'] = theme('culturefeed_uitpas_advantages_table', $advantages);
    $options = array('attributes' => array('class' => 'link-primary'));
    if ($filter) {
      $options['query'] = array('filter' => $filter);
    }
    $form['promotions']['advantages']['#markup'] .= '<p class="link-more">' . l(t('Show all advantages'), 'advantages',  $options) . '</p>';
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

/**
 * Title callback for the promotions page.
 */
function culturefeed_uitpas_promotions_title() {

  return t('UiTPAS promotions');

}
