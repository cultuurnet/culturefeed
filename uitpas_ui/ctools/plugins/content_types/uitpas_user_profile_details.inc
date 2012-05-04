<?php

/**
 * @file
 * Plugin to provide a content type for uitpas user profile details.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Uitpas: user profile details'),
  'description' => t('Uitpas: user profile details.'),
  'category' => t('Uitpas'),
  'required context' => new ctools_context_required(t('Uitpas user'), 'uitpas_user'),
);

function uitpas_ui_uitpas_user_profile_details_content_type_edit_form($form, &$form_state) {
  return $form;
}

function uitpas_ui_uitpas_user_profile_details_content_type_render($subtype, $conf, $args, $context) {
  if ($context->data) {
    $uitpas_user = $context->data->user;
    $passholder = $context->data->passholder;
    $block = new stdClass();
    $block->title = "";
    $block->content = theme('uitpas_ui_user_profile_details', array(
      'uitpas_user' => $uitpas_user,
      'passholder' => $passholder,
    ));
    return $block;
  }
}