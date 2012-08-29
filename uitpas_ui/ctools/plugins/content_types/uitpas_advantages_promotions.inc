<?php

/**
 * @file
 * Plugin to provide a content type for uitpas advantages and promotions.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Uitpas: advantages and promotions'),
  'description' => t('Uitpas: advantages and promotions.'),
  'category' => t('Uitpas'),
  'required context' => new ctools_context_required(t('Uitpas location'), 'uitpas_location'),
  'defaults' => array(
     'advantages_limit' => 10,
     'promotions_limit' => 10,
   ),
);

function uitpas_ui_uitpas_advantages_promotions_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form['advantages_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Advantages page limit'),
    '#default_value' => $conf['advantages_limit'],
  );
  $form['promotions_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Promotions page limit'),
    '#default_value' => $conf['promotions_limit'],
  );
  return $form;
}

function uitpas_ui_uitpas_advantages_promotions_content_type_edit_form_submit($form, &$form_state) {
  $form_state['conf']['advantages_limit'] = $form_state['values']['advantages_limit'];
  $form_state['conf']['promotions_limit'] = $form_state['values']['promotions_limit'];
}

function uitpas_ui_uitpas_advantages_promotions_content_type_render($subtype, $conf, $args, $context) {
  $location = $context->data->cf_query_argument;
  $advantages = array();
  $advantages_limit = $conf['advantages_limit'];
  $advantages_page = pager_find_page(0);
  $advantages_total = 0;
  $promotions = array();
  $promotions_limit = $conf['promotions_limit'];
  $promotions_page = pager_find_page(1);
  $promotions_total = 0;
  try {
    $cf = DrupalCultureFeed::getConsumerInstance();
    //advantages
    $query = new CultureFeed_Uitpas_Promotion_Query_WelcomeAdvantagesOptions();
    $query->start = $advantages_page * $advantages_limit;
    $query->max = $advantages_limit;
    if ($location) {
      $query->city = $location;
    }
    $query->cashingPeriodBegin = time();
    $result = $cf->uitpas()->searchWelcomeAdvantages($query);
    $advantages_page = pager_default_initialize($result->total, $advantages_limit, 0);
    $advantages = $result->objects;
    uitpas_ui_helpers_process_counters($advantages);
    $advantages_total = $result->total;
    //promotions
    $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
    $query->start = $promotions_page * $promotions_limit;
    $query->max = $promotions_limit;
    if ($location) {
      $query->city = $location;
    }
    $query->cashingPeriodBegin = time();
    $result = $cf->uitpas()->getPromotionPoints($query);
    $promotions_page = pager_default_initialize($result->total, $promotions_limit, 1);
    $promotions = $result->objects;
    uitpas_ui_helpers_process_counters($promotions);
    $promotions_total = $result->total;
  }
  catch (Exception $e) {
    watchdog_exception('uitpas_ui_content_type_advantages_promotions', $e);
  }
  $block = new stdClass();
  $block->title = t('Advantages and promotions');
  $block->content = theme('uitpas_ui_advantages_promotions', array(
    'advantages' => $advantages,
    'advantages_total' => $advantages_total,
    'promotions' => $promotions,
    'promotions_total' => $promotions_total,
  ));
  return $block;
}