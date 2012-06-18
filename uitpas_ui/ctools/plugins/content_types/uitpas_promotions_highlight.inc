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
  $location = $context->data->cf_query_argument;
  try {
    $cf = DrupalCultureFeed::getConsumerInstance();
    $highlights = variable_get('uitpas_ui_promotions_highlight', array());
    $promotions = array();
    foreach ($highlights as $highlight) {
      try {
        $promotions[] = $cf->uitpas()->getPointsPromotion($highlight);
      }
      catch (Exception $e) {
        watchdog_exception('uitpas_ui_content_type_promotions_highlight', $e);
      }
    }
    $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
    $query->sort = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::SORT_CASHING_PERIOD_END;
    $query->limit = 10 - count($highlights);
    $query->cashingPeriodBegin = time();
    if ($location) {
      $query->city = $location;
    }
    $result = $cf->uitpas()->getPromotionPoints($query);
    foreach ($result->objects as $object) {
      if (!in_array($object->id, $highlights)) {
        $promotions[] = $object;
      }
    }
    $block = new stdClass();
    $block->title = t("Spotlight promotions");
    if ($location) {
      $city = $context->data->locations[$context->data->location]['name'];
      $block->title = t("Spotlight promotions in !city", array('!city' => $city));
    }
    $block->content = theme('uitpas_ui_promotions_highlight', array('promotions' => $promotions));
    return $block;
  }
  catch (Exception $e) {
    watchdog_exception('uitpas_ui_content_type_promotions_highlight', $e);
  }
}