<?php

/**
 * @file
 * Plugin to provide a context for uitpas advantage.
 */

$plugin = array(
  'title' => t('Uitpas: advantage'),
  'description' => t('Uitpas: advantage.'),
  'context' => 'uitpas_ui_context_uitpas_advantage',
  'context name' => 'uitpas_advantage',
  'convert list' => array(
    'id' => t('Uitpas advantage id'),
  ),
  'convert' => 'uitpas_ui_context_uitpas_advantage_convert',
  'keyword' => 'uitpas_advantage',
);

function uitpas_ui_context_uitpas_advantage($empty, $data = NULL, $conf = FALSE) {
  $context = new ctools_context('uitpas_advantage');
  $context->title = t('Uitpas advantage');
  $context->plugin = 'uitpas_advantage';
  $context->data = FALSE;
  if ($data) {
    try {
      $cf = DrupalCultureFeed::getConsumerInstance();
      $advantages = array($cf->uitpas()->getWelcomeAdvantage($data));
      uitpas_ui_helpers_process_counters($advantages);
      $context->data = $advantages[0];
    }
    catch (Exception $e) {
      watchdog_exception('uitpas_ui_context_uitpas_advantage', $e);
    }
  }
  return $context;
}

function uitpas_ui_context_uitpas_advantage_convert($context, $type) {
  switch ($type) {
    case 'id':
      return $context->data->id;
  }
}