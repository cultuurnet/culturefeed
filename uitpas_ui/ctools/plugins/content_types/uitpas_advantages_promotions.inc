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
);

function uitpas_ui_uitpas_advantages_promotions_content_type_edit_form($form, &$form_state) {
  return $form;
}

function uitpas_ui_uitpas_advantages_promotions_content_type_render($subtype, $conf, $args, $context) {
  $location = $context->data->location;
  $cf = DrupalCultureFeed::getConsumerInstance();
  //advantages
  $result = $cf->oauth_client->consumerGetAsXml('/uitpas/promotion/welcomeAdvantages');
  //promotions
  $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
  $result = $cf->uitpas()->getPromotionPoints($query);
  
  //$query->city = 'Aalst';
  $query->sort = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::SORT_POINTS;
  //$result = $cf->uitpas()->getPromotionPoints($query);
  dpm($result);
  $block = new stdClass();
  $block->title = "";
  $block->content = drupal_get_form('culturefeed_uitpas_location_content_type_render_form', $locations, $location);
  return $block;
}