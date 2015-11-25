<?php

/**
 * @file
 * Helper functions for UiTPAS advantages.
 */

/**
 * Returns advantages.
 */
function culturefeed_uitpas_advantages_get($filter = NULL) {

  $advantages = array();
  $advantages_max = variable_get('culturefeed_uitpas_promotions_advantages_max', 20);
  $advantages_pager_element = 0;
  $advantages_page = pager_find_page($advantages_pager_element);
  $advantages_total = 0;

  try {

    $cf = DrupalCultureFeed::getConsumerInstance();

    // Advantages.
    $query = new CultureFeed_Uitpas_Promotion_Query_WelcomeAdvantagesOptions();
    $query->start = $advantages_page * $advantages_max;
    $query->max = $advantages_max;
    $query->unexpired = TRUE;
    $query->cashingPeriodBegin = time();
    $card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');
    if ($card_system_id) {
      $query->applicableCardSystemId = $card_system_id;
    }
    if ($filter) {
      $query->owningCardSystemId = $filter;
    }
    $result = $cf->uitpas()->searchWelcomeAdvantages($query);
    pager_default_initialize($result->total, $advantages_max, $advantages_pager_element);
    $advantages = $result->objects;
    $advantages_total = $result->total;

  }
  catch (Exception $e) {

    watchdog_exception('culturefeed_uitpas_advantages', $e);
    pager_default_initialize(0, $advantages_max, $advantages_pager_element);

  }

  return array(
    '#theme' => 'culturefeed_uitpas_advantages',
    '#advantages' => $advantages,
    '#advantages_total' => $advantages_total,
    '#advantages_pager_element' => $advantages_pager_element,
  );

}

/**
 * Returns form elements for advantages promotions settings.
 */
function culturefeed_uitpas_advantages_settings_get(&$form) {

  $form['advantages'] = array(
    '#type' => 'fieldset',
    '#title' => t('UiTPAS advantages settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['advantages']['culturefeed_uitpas_advantages_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Page title'),
    '#default_value' => variable_get('culturefeed_uitpas_advantages_title', t('UiTPAS advantages')),
  );
  $form['advantages']['culturefeed_uitpas_advantages_info'] = array(
    '#type' => 'textarea',
    '#title' => t('Info text'),
    '#default_value' => variable_get('culturefeed_uitpas_advantages_info', t('This page shows all advantages for which UiTPAS holders can save points. Find out <a href="@search-url">where to save points</a>.', array('@search-url' => url('agenda/search?search=UITPAS*')))),
  );
  $form['advantages']['culturefeed_uitpas_advantages_advantages_max'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of advantages to display'),
    '#default_value' => variable_get('culturefeed_uitpas_advantages_advantages_max', 20),
  );

}

/**
 * Returns form elements for filter & sort on promotions page.
 */
function culturefeed_uitpas_advantages_filter_sort($form, $form_state) {

  $cf = DrupalCultureFeed::getConsumerInstance();
  $kind_of_card_systems = 'permanent';
  $card_systems = $cf->uitpas()->getCardSystems($kind_of_card_systems);

  $filter = isset($form_state['values']['filter']) ? $form_state['values']['filter'] : (isset($_GET['filter']) ? $_GET['filter'] : NULL);
  $advantages = culturefeed_uitpas_advantages_get($filter);

  $options = array();
  foreach ($card_systems as $system) {
    $options[$system->id] = $system->name;
  }

  $id = drupal_html_id('advantages-filter-sort');

  $form['advantages']['#prefix'] = "<div id=\"" . $id . "\">";
  $form['advantages']["#suffix"] = "</div>";

  $advantages_intro = variable_get('culturefeed_uitpas_advantages_info');
  if (trim($advantages_intro)) {
    $form['advantages']['intro']['#markup'] = '<div class="advantages-intro">' . $advantages_intro . '</div>';
  }

  $form['advantages']['filter'] = array(
    '#type' => 'radios',
    '#title' => t('Filter by'),
    '#default_value' => isset($filter) ? $filter : 0,
    '#empty_option' => t('All card systems'),
    '#options' => $options,
    '#theme' => 'culturefeed_uitpas_filter_labels',
    '#ajax' => array(
      'event' => 'change',
      'wrapper' => $id,
      'callback' => 'culturefeed_uitpas_advantages_ajax_callback',
    ),
  );

  $total = $advantages['#advantages_total'];
  $title = t('Found %count advantages', array('%count' => $total));
  $filter_reset_link = l(t('Show all'), 'promotions');

  if ($filter) {
    $form['advantages']['title']['#markup'] = '<h2>' . $title . ' <small>' . $options[$filter] . ' (' . $filter_reset_link . ')</small></h2>';
  }
  else {
    $form['advantages']['title']['#markup'] = '<h2>' . $title . ' <small>(' . $filter_reset_link . ')</small></h2>';
  }

  if (count($advantages['#advantages'])) {
    $form['advantages']['result']['#markup'] = theme('culturefeed_uitpas_advantages_table', $advantages);
  }
  else {
    $form['advantages']['result']['#markup'] = '<div class="no-results no-advantages">' . t('No results found') . '</div>';
  }

  // Pager.
  $advantages_max = variable_get('culturefeed_uitpas_advantages_advantages_max', 20);

  pager_default_initialize($advantages['#advantages_total'], $advantages_max);

  // Solution csdco on from https://www.drupal.org/node/1049354#comment-6253362
  $tmp_q = $_GET['q'];
  $_GET['q'] = 'advantages';
  $form['promotions']['pager']['#markup'] = '<div class="pager clearfix">' . theme('pager', array('parameters' => array('filter' => $filter))) . '</div>';
  $_GET['q'] = $tmp_q;

  return $form;

}

function culturefeed_uitpas_advantages_ajax_callback($form, $form_state) {
  return $form['advantages'];
}
