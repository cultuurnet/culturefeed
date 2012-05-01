<?php

/**
 * @file
 * Plugin to provide an argument handler for uitpas location.
 */

$plugin = array(
  'title' => t('Uitpas: location'),
  'description' => t('Uitpas: location.'),
  'keyword' => 'uitpas_location',
  'context' => 'culturefeed_uitpas_location_context',
);

function culturefeed_uitpas_location_context($arg = NULL, $conf = NULL, $empty = FALSE) {
  if ($empty) {
    return ctools_context_create_empty('uitpas_location');
  }
  //we can't check on validity since the argument is optional
  return ctools_context_create('uitpas_location', strtolower($arg));
}