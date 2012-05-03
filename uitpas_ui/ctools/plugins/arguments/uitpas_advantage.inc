<?php

/**
 * @file
 * Plugin to provide an argument handler for uitpas promotion.
 */

$plugin = array(
  'title' => t('Uitpas: advantage'),
  'description' => t('Uitpas: advantage.'),
  'keyword' => 'uitpas_advantage',
  'context' => 'uitpas_ui_uitpas_advantage_context',
);

function uitpas_ui_uitpas_advantage_context($arg = NULL, $conf = NULL, $empty = FALSE) {
  if ($empty) {
    return ctools_context_create_empty('string');
  }
  return ctools_context_create('string', $arg);
}