<?php

/**
 * @file
 * Plugin to provide an argument handler for uitpas promotion.
 */

$plugin = array(
  'title' => t('Uitpas: promotion'),
  'description' => t('Uitpas: promotion.'),
  'keyword' => 'uitpas_promotion',
  'context' => 'uitpas_ui_uitpas_promotion_context',
);

function uitpas_ui_uitpas_promotion_context($arg = NULL, $conf = NULL, $empty = FALSE) {
  if ($empty) {
    return ctools_context_create_empty('uitpas_promotion');
  }
  $context = ctools_context_create('uitpas_promotion', $arg);
  if (!$context->data) {
    return FALSE;
  }
  return $context;
}