<?php

/**
 * @file
 * Plugin to provide an argument handler for uitpas activity.
 */

$plugin = array(
  'title' => t('Uitpas: activity'),
  'description' => t('Uitpas: activity.'),
  'keyword' => 'uitpas_activity',
  'context' => 'uitpas_ui_uitpas_activity_context',
);

function uitpas_ui_uitpas_activity_context($arg = NULL, $conf = NULL, $empty = FALSE) {
  if ($empty) {
    return ctools_context_create_empty('string');
  }
  return ctools_context_create('string', $arg);
}