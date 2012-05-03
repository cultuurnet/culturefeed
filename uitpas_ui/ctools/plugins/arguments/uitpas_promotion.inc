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
    return ctools_context_create_empty('string');
  }
  return ctools_context_create('string', $arg);
}