<?php

/**
 * @file
 * Plugin to provide an argument handler for uitpas advantage.
 */

$plugin = array(
  'title' => t('Uitpas: advantage'),
  'description' => t('Uitpas: advantage.'),
  'keyword' => 'uitpas_advantage',
  'context' => 'uitpas_ui_uitpas_advantage_context',
);

function uitpas_ui_uitpas_advantage_context($arg = NULL, $conf = NULL, $empty = FALSE) {
  if ($empty) {
    return ctools_context_create_empty('uitpas_advantage');
  }
  $context = ctools_context_create('uitpas_advantage', $arg);
  if (!$context->data) {
    return FALSE;
  }
  return $context;
}