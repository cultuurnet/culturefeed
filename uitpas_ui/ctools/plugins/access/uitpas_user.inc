<?php

/**
 * @file
 * Plugin to provide access control/visibility uitpas user.
 */

$plugin = array(
  'title' => t("Uitpas: user"),
  'description' => t('Control access by checking passholder.'),
  'callback' => 'uitpas_ui_uitpas_user_access_check',
  'required context' => new ctools_context_required(t('Uitpas: user'), array('uitpas_user')),
  'summary' => 'uitpas_ui_uitpas_user_access_summary',
);

function uitpas_ui_uitpas_user_access_check($conf, $context) {
  if (!empty($context) && $context->data && $context->data->passholder) {
    return TRUE;
  }
  return FALSE;
}

function uitpas_ui_uitpas_user_access_summary($conf, $context) {
  return t('Valid uitpas passholder');
}