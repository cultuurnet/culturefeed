<?php

/**
 * @file
 * Plugin to provide access control/visibility uitpas location.
 */

$plugin = array(
  'title' => t("Uitpas: location"),
  'description' => t('Control access by checking location.'),
  'callback' => 'uitpas_ui_uitpas_location_access_check',
  'required context' => new ctools_context_required(t('Uitpas: location'), 'uitpas_location'),
);

function uitpas_ui_uitpas_location_access_check($conf, $context) {
  if (empty($context)) {
    return FALSE;
  }
  return $context->data->valid_argument;
}