<?php

/**
 * @file
 * Plugin to provide a content type for uitpas recent actions.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Uitpas: recent actions'),
  'description' => t('Uitpas: recent actions.'),
  'category' => t('Uitpas'),
  'required context' => array(
    new ctools_context_required(t('Uitpas loation'), 'uitpas_location'),
    new ctools_context_required(t('Uitpas user'), 'uitpas_user'),
  ),
);

function uitpas_ui_uitpas_recent_actions_content_type_edit_form($form, &$form_state) {
  return $form;
}

function uitpas_ui_uitpas_recent_actions_content_type_render($subtype, $conf, $args, $context) {
  $location = $context[0]->data;
  $actions = array();
  try {
    $cf = DrupalCultureFeed::getConsumerInstance();
    $query = new CultureFeed_Uitpas_Passholder_Query_SearchCheckinsOptions();
    if ($location) {
      $query->city = $location;
    }
    //$result = $cf->uitpas()->searchCheckins($query);
    if ($result->total) {
      $actions = $result['objects'];
    }
  }
  catch (Exception $e) {
    watchdog_exception('uitpas_ui_content_type_recent_actions', $e);
  }
  
  $block = new stdClass();
  $block->title = "";
  $block->content = theme('uitpas_ui_recent_actions', array('actions' => $actions));
  return $block;
}