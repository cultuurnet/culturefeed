<?php

/**
 * @file
 * Plugin to provide an relationship handler for uitpas slug redirect.
 */

$plugin = array(
  'title' => t('Uitpas: slug redirect'),
  'description' => t('Uitpas: slug redirect.'),
  'keyword' => 'uitpas_slug_redirect',
  'required context' => new ctools_context_required(t('Uitpas object'), array(
    'uitpas_activity', 
    'uitpas_actor', 
    'uitpas_advantage', 
    'uitpas_promotion',
  )),
  'context' => 'uitpas_ui_uitpas_slug_redirect_context',
);

function uitpas_ui_uitpas_slug_redirect_context($context, $conf) {
  if (empty($context->data)) {
    return ctools_context_create_empty('string');
  }
  if (isset($context->data)) {
    $object = $context->data;
    $link = uitpas_ui_helpers_object_info_by_context_type($context->type, $object, 'link');
    return ctools_context_create('string', $link);
  }
}