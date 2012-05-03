<?php

/**
 * @file
 * Plugin to provide a content type for uitpas user profile activities.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Uitpas: user profile activities'),
  'description' => t('Uitpas: user profile activities.'),
  'category' => t('Uitpas'),
  'required context' => new ctools_context_required(t('Uitpas user'), 'uitpas_user'),
);

function uitpas_ui_uitpas_user_profile_activities_content_type_edit_form($form, &$form_state) {
  return $form;
}

function uitpas_ui_uitpas_user_profile_activities_content_type_render($subtype, $conf, $args, $context) {
  if ($context->data) {
    $uitpas_user = $context->data->user;
    $cf = $context->data->cf;
    try {
      $query = new CultureFeed_Uitpas_Passholder_Query_SearchCheckinsOptions();
      $query->uid = $uitpas_user->id;
      $result = $cf->uitpas()->searchCheckins($query);
      $block = new stdClass();
      $block->title = "";
      $block->content = theme('uitpas_ui_user_profile_activities', array('activities' => $result->objects));
      return $block;
    }
    catch (Exception $e) {
      watchdog_exception('uitpas_ui_content_type_user_profile_promotions_cashed_in', $e);
    }
  }
}