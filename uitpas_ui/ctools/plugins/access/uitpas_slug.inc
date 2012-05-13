<?php

/**
 * @file
 * Plugin to provide access control/visibility uitpas slug.
 */

$plugin = array(
  'title' => t("Uitpas: slug"),
  'description' => t('Control access by checking slug.'),
  'callback' => 'uitpas_ui_uitpas_slug_access_check',
  'required context' => array(
    new ctools_context_required(t('Uitpas: object'), array(
      'uitpas_activity', 
      'uitpas_actor', 
      'uitpas_advantage',
      'uitpas_promotion',
    )),
    new ctools_context_required(t('Uitpas: slug'), 'string'),
   ),
  'summary' => 'uitpas_ui_uitpas_slug_access_summary',
);

function uitpas_ui_uitpas_slug_access_check($conf, $context) {
  if (empty($context)) {
    return FALSE;
  }
  else {
    $object = $context[0]->data;
    $slug = $context[1]->data;
    $title = uitpas_ui_helpers_object_info_by_context_type($context[0]->type, $object, 'title');
    if ($slug == cnapi_ui_slug($title)) {
      return TRUE;
    }
  }
  return FALSE;
}

function uitpas_ui_uitpas_slug_access_summary($conf, $context) {
  return t('Valid uitpas slug');
}