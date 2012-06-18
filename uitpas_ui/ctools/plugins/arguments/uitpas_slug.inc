<?php

/**
 * @file
 * Plugin to provide an argument handler for uitpas slug.
 */

$plugin = array(
  'title' => t('Uitpas: slug'),
  'description' => t('Uitpas: slug.'),
  'keyword' => 'uitpas_slug',
  'context' => 'uitpas_ui_uitpas_slug_context',
);

function uitpas_ui_uitpas_slug_context($arg = NULL, $conf = NULL, $empty = FALSE) {
  if ($empty) {
    return ctools_context_create_empty('string');
  }
  return ctools_context_create('string', $arg);
}