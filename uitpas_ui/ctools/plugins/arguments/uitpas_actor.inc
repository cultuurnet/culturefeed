<?php

/**
 * @file
 * Plugin to provide an argument handler for uitpas actor.
 */

$plugin = array(
  'title' => t('Uitpas: actor'),
  'description' => t('Uitpas: actor.'),
  'keyword' => 'uitpas_actor',
  'context' => 'uitpas_ui_uitpas_actor_context',
);

function uitpas_ui_uitpas_actor_context($arg = NULL, $conf = NULL, $empty = FALSE) {
  if ($empty) {
    return ctools_context_create_empty('string');
  }
  return ctools_context_create('string', $arg);
}