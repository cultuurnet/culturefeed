<?php

/**
 * @file
 * Plugin to provide a content type for uitpas advantage.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Uitpas: advantage'),
  'description' => t('Uitpas: advantage.'),
  'category' => t('Uitpas'),
  'required context' => new ctools_context_required(t('Uitpas advantage'), 'uitpas_advantage'),
);

function uitpas_ui_uitpas_advantage_content_type_edit_form($form, &$form_state) {
  return $form;
}

function uitpas_ui_uitpas_advantage_content_type_render($subtype, $conf, $args, $context) {
  $advantage = $context->data;
  if ($advantage) {
    $block = new stdClass();
    $block->title = t('Welcome advantage') . ' ' . $advantage->title;
    $block->content = theme('uitpas_ui_advantage', array('advantage' => $advantage));
    return $block;
  }
}