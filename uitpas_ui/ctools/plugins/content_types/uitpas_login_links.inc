<?php

/**
 * @file
 * Plugin to provide a content type for uitpas login links.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Uitpas: login links'),
  'description' => t('Uitpas: login links.'),
  'category' => t('Uitpas'),
  'required context' => new ctools_context_required(t('Uitpas user'), 'uitpas_user'),
);

function uitpas_ui_uitpas_login_links_content_type_edit_form($form, &$form_state) {
  return $form;
}

function uitpas_ui_uitpas_login_links_content_type_render($subtype, $conf, $args, $context) {
  
  $links = array('links' => array(), 'heading' => array(), 'attributes' => array());
  $link = array('attributes' => array('class' => array('culturefeedconnect')), 'query' => drupal_get_destination());
  if (!isset($context->data->user)) {
    $links['links']['uitpas_login'] = $link + array(
      'title' => t('Login'),
      'href' => 'culturefeed/oauth/connect', 
    );
  }
  if (!$context->data || $context->data && !$context->data->passholder) {
    $links['links']['uitpas_register'] = $link + array(
      'title' => t('Register'),
      'href' => 'culturefeed/oauth/connect/register',
    );
  }
  if (count($links['links'])) {
    $block = new stdClass();
    $block->title = '';
    $block->content = theme_links($links);
    return $block;
  }
}