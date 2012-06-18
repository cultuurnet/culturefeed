<?php

/**
 * @file
 * Plugin to provide a context for uitpas promotion.
 */

$plugin = array(
  'title' => t('Uitpas: promotion'),
  'description' => t('Uitpas: promotion.'),
  'context' => 'uitpas_ui_context_uitpas_promotion',
  'context name' => 'uitpas_promotion',
  'convert list' => array(
    'id' => t('Uitpas promotion id'),
    'points' => t('Uitpas promotion points'),
  ),
  'convert' => 'uitpas_ui_context_uitpas_promotion_convert',
  'keyword' => 'uitpas_promotion',
);

function uitpas_ui_context_uitpas_promotion($empty, $data = NULL, $conf = FALSE) {
  $context = new ctools_context('uitpas_promotion');
  $context->title = t('Uitpas promotion');
  $context->plugin = 'uitpas_promotion';
  $context->data = FALSE;
  if ($data) {
    try {
      $cf = DrupalCultureFeed::getConsumerInstance();
      $promotions = array($cf->uitpas()->getPointsPromotion($data));
      uitpas_ui_helpers_process_counters($promotions);
      $context->data = $promotions[0];
    }
    catch (Exception $e) {
      watchdog_exception('uitpas_ui_context_uitpas_promotion', $e);
    }
  }
  return $context;
}

function uitpas_ui_context_uitpas_promotion_convert($context, $type) {
  switch ($type) {
    case 'id':
      return $context->data->id;
    case 'points':
      return $context->data->points;
  }
}