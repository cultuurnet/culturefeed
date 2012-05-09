<?php

/**
 * @file
 * Plugin to provide a content type for uitpas promotion.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Uitpas: promotion'),
  'description' => t('Uitpas: promotion.'),
  'category' => t('Uitpas'),
  'required context' => new ctools_context_required(t('Uitpas promotion'), 'string'),
);

function uitpas_ui_uitpas_promotion_content_type_edit_form($form, &$form_state) {
  return $form;
}

function uitpas_ui_uitpas_promotion_content_type_render($subtype, $conf, $args, $context) {
  $pid = $context->data;
  if ($pid) {
    try {
      $cf = DrupalCultureFeed::getConsumerInstance();
      $promotions = array($cf->uitpas()->getPointsPromotion($pid));
      uitpas_ui_link_actors($promotions);
      $block = new stdClass();
      $block->title = $promotions[0]->title;
      $block->content = theme('uitpas_ui_promotion', array('promotion' => $promotions[0]));
      return $block;
    }
    catch (Exception $e) {
      watchdog_exception('uitpas_ui_content_type_promotion', $e);
    }
  }
}