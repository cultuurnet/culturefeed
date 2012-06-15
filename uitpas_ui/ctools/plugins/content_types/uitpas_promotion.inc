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
  'required context' => new ctools_context_required(t('Uitpas promotion'), 'uitpas_promotion'),
);

function uitpas_ui_uitpas_promotion_content_type_edit_form($form, &$form_state) {
  return $form;
}

function uitpas_ui_uitpas_promotion_content_type_render($subtype, $conf, $args, $context) {
  $promotion = $context->data;
  if ($promotion) {
    $block = new stdClass();
    $block->title = t('Promotion') . ' ' . $promotion->title;
    $block->content = theme('uitpas_ui_promotion', array('promotion' => $promotion));
    return $block;
  }
}