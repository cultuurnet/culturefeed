<?php

/**
 * @file
 * Plugin to provide a context for uitpas actor.
 */

$plugin = array(
  'title' => t('Uitpas: actor'),
  'description' => t('Uitpas: actor.'),
  'context' => 'uitpas_ui_context_uitpas_actor',
  'context name' => 'uitpas_actor',
  'convert list' => array(
    'id' => t('Uitpas actor id'),
  ),
  'convert' => 'uitpas_ui_context_uitpas_actor_convert',
  'keyword' => 'uitpas_actor',
);

function uitpas_ui_context_uitpas_actor($empty, $data = NULL, $conf = FALSE) {
  $context = new ctools_context('uitpas_actor');
  $context->title = t('Uitpas actor');
  $context->plugin = 'uitpas_actor';
  $context->data = FALSE;
  if ($data) {
    $actor = cnapi_get_actor($data);
    if ($actor) {
      $context->data = $actor;
    }
  }
  return $context;
}

function uitpas_ui_context_uitpas_actor_convert($context, $type) {
  switch ($type) {
    case 'id':
      return $context->data['cdbid'];
  }
}