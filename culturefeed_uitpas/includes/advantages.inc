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

  // If advantages aren't shown above promotions, fall back to links to toggle between both
  if (variable_get('culturefeed_uitpas_show_advantages_above_promotions') == FALSE) {
    $form['advantages_link'] = array(
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

  $advantages = culturefeed_uitpas_advantages_get($filter);

  $id = drupal_html_id('advantages-filter-sort');

  $form['advantages']['#prefix'] = "<div class=\"filters filters-regions\" id=\"" . $id . "\">";
  $form['advantages']["#suffix"] = "</div>";

  $advantages_intro = t('This page shows all advantages for which UiTPAS holders can save points.');

  if (culturefeed_uitpas_is_passholder()) {
   $advantages_intro .= ' '. t('Discover <a href="@my-promotions">your personal promotions</a>  and points balance.', array('@my-promotions' => url('culturefeed/profile/uitpas/promotions')));
  }
  else {
    $advantages_intro .= ' '. t('<a href="@register-where">Find out where</a> to get an UiTPAS', array('@register-where' => url('register_where'))); 
    if (!culturefeed_is_culturefeed_user()) {
      $advantages_intro .= ' '. t('or <a href="@login">login</a> and register your UiTPAS to discover your personal promotions and points balance.', array('@login' => url('culturefeed/oauth/connect', array('query' => array('destination' => 'register_uitpas'))))); 
    }
    else {
      $advantages_intro .= '.';
    }
  }

  if (trim($advantages_intro)) {
    $form['advantages']['intro']['#markup'] = '<div class="intro intro-advantages">' . $advantages_intro . '</div>';
  }

  if ($multiple_card_systems) {
  
    $options = array();
    foreach ($card_systems as $system) {
      $options[$system->id] = $system->name;
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
  
    if (isset($filter)) {
      $filter_reset_link = '<div class="filter-reset-link">' . l(t('Show all advantages'), 'advantages', array('attributes' => array('class' => array('reset-filter')))) . '</div>';
    }
    else {
      $filter_reset_link = '';
    }
  
    $form['advantages']['filter']['#prefix'] = '<div class="form-group">';
    $form['advantages']['filter']['#suffix'] = $filter_reset_link . '</div>';
  }

  $total = $advantages['#advantages_total'];
  $title = t('Found %count advantages', array('%count' => $total));  
  $filter_reset_link = l('&times;', 'advantages', array('attributes' => array('class' => array('reset-filter-inline')), 'html' => TRUE));  
  $all_promotions_link = l(t('Show all promotions'), 'promotions', array('attributes' => array('class' => array('all-promotions-link'))));

  if ($filter && $multiple_card_systems) {
    $form['advantages']['title']['#markup'] = '<h3 class="results-title">' . $title . ' <small>' . $options[$filter] . ' ' . $filter_reset_link . '</small></h3>';
  }
  else {
    $form['advantages']['title']['#markup'] = '<h3 class="results-title">' . $title . ' <small>' . $all_promotions_link . '</small></h3>';
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

/**
 * Title callback for the advantages page.
 */
function culturefeed_uitpas_advantages_title() {

  return t('UiTPAS advantages');

}
