<?php

/**
 * @file
 * Plugin to provide a content type for uitpas status links.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Uitpas: status links'),
  'description' => t('Uitpas: status links.'),
  'category' => t('Uitpas'),
  'required context' => new ctools_context_required(t('Uitpas user'), 'uitpas_user'),
);

function uitpas_ui_uitpas_status_links_content_type_edit_form($form, &$form_state) {
  return $form;
}

function uitpas_ui_uitpas_status_links_content_type_render($subtype, $conf, $args, $context) {
  if ($context->data && $context->data->passholder) {
    $links = array('links' => array(), 'heading' => array(), 'attributes' => array());
    $link = array('attributes' => array());
    if (isset($context->data->user->depiction)) {
      $links['links']['uitpas_picture'] = $link + array(
        'title' => theme_image(array('path' => $context->data->user->depiction, 'attributes' => array())),
        'html' => TRUE,
      );
    }
    $links['links']['uitpas_name'] = $link + array(
     'title' => $context->data->user->nick,
    );
    $links['links']['uitpas_details'] = $link + array(
     'title' => t('My UITPAS'),
     'href' => 'myprofile/details', 
    );
    $links['links']['uitpas_logout'] = $link + array(
     'title' => t('Logout'),
     'href' => 'user/logout', 
    );
    $block = new stdClass();
    $block->title = '';
    $block->content = theme_links($links);
    return $block;
  }
}