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
    $query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
    $query->sort = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::SORT_CASHING_PERIOD_END;
    $query->limit = 10;
    if ($location) {
      $query->city = $location;
    }
    $result = $cf->uitpas()->getPromotionPoints($query);
    $block = new stdClass();
    $block->title = t("Spotlight promotions");
    $block->content = theme('uitpas_ui_promotions_highlight', array('promotions' => $result->objects));
    return $block;
  }
  catch (Exception $e) {
    watchdog_exception('uitpas_ui_content_type_promotions_highlight', $e);
  }
}