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
);

function uitpas_ui_uitpas_user_access_check($conf, $context) {
  if (!empty($context) && $context->data && $context->data->passholder) {
    return TRUE;
  }
  drupal_access_denied();
  return FALSE;
}