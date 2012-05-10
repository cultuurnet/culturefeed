<?php

/**
 * @file
 * Plugin to provide a context for uitpas activity.
 */

$plugin = array(
  'title' => t('Uitpas: activity'),
  'description' => t('Uitpas: activity.'),
  'context' => 'uitpas_ui_context_uitpas_activity',
  'context name' => 'uitpas_activity',
  'convert list' => array(
    'id' => t('Uitpas activity id'),
  ),
  'convert' => 'uitpas_ui_context_uitpas_activity_convert',
  'keyword' => 'uitpas_activity',
);

function uitpas_ui_context_uitpas_activity($empty, $data = NULL, $conf = FALSE) {
  $context = new ctools_context('uitpas_activity');
  $context->title = t('Uitpas activity');
  $context->plugin = 'uitpas_activity';
  $context->data = FALSE;
  if ($data) {
    $activity = cnapi_get_event($data);
    if ($activity) {
      $context->data = $activity;
    }
  }
  return $context;
}

function uitpas_ui_context_uitpas_activity_convert($context, $type) {
  switch ($type) {
    case 'id':
      return $context->data['cdbid'];
  }
}