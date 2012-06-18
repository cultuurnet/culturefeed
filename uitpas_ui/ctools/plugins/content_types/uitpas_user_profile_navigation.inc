<?php

/**
 * @file
 * Plugin to provide a content type for uitpas user profile navigation.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Uitpas: user profile navigation'),
  'description' => t('Uitpas: user profile navigation.'),
  'category' => t('Uitpas'),
  'required context' => new ctools_context_required(t('Uitpas user'), 'uitpas_user'),
);

function uitpas_ui_uitpas_user_profile_navigation_content_type_edit_form($form, &$form_state) {
  return $form;
}

function uitpas_ui_uitpas_user_profile_navigation_content_type_render($subtype, $conf, $args, $context) {
  if ($context->data && $context->data->passholder) {
    $navigation = array('details', 'advantages', 'activities', 'uitid', 'account_edit', 'notifications');
    $block = new stdClass();
    $block->title = t('My UITPAS');
    $block->content = theme('uitpas_ui_user_profile_navigation', array('navigation' => $navigation));
    return $block;
  }
}