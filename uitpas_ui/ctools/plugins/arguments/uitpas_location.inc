<?php

/**
 * @file
 * Plugin to provide an argument handler for uitpas location.
 */

$plugin = array(
  'title' => t('Uitpas: location'),
  'description' => t('Uitpas: location.'),
  'keyword' => 'uitpas_location',
  'context' => 'uitpas_ui_uitpas_location_context',
);

function uitpas_ui_uitpas_location_context($arg = NULL, $conf = NULL, $empty = FALSE) {
  if ($empty) {
    return ctools_context_create_empty('uitpas_location');
  }
  //we can't validate an optional argument so we put a http_response code variant in front
  $context = ctools_context_create('uitpas_location', strtolower($arg));
  return $context;
}