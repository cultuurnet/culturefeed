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
  $context = ctools_context_create('uitpas_location', strtolower($arg));
  if ($arg && $arg != 'all' && $context->data->location != strtolower($arg)) {
    //we can't check on validity since the argument is optional
    drupal_not_found();
    return FALSE;
  }
  return $context;
}