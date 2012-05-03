<?php

/**
 * @file
 * Plugin to provide a content type for uitpas promotions highlight.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Uitpas: promotions highlight'),
  'description' => t('Uitpas: promotions highlight.'),
  'category' => t('Uitpas'),
  'required context' => new ctools_context_required(t('Uitpas location'), 'uitpas_location'),
);

function uitpas_ui_uitpas_promotions_highlight_content_type_edit_form($form, &$form_state) {
  return $form;
}

function uitpas_ui_uitpas_promotions_highlight_content_type_render($subtype, $conf, $args, $context) {
  $location = $context->data->location;
  $cf = DrupalCultureFeed::getConsumerInstance();
  $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
  //$query->sort = SORT_CASHING_PERIOD_END;
  $query->limit = 10;
  if ($location) {
    $query->city = $location;
  }
  //$result = $cf->uitpas()->getPromotionPoints($query);
  $promotions = array();
  $block = new stdClass();
  $block->title = "";
  $block->content = theme('uitpas_promotions_highlight', array('promotions' => $promotions));
  return $block;
}