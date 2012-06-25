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
  'defaults' => array(
    'limit' => 10,
  ),
);

function uitpas_ui_uitpas_user_profile_activities_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form['limit'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['limit'],
    '#title' => t('Page limit'),
  );
  return $form;
}

function uitpas_ui_uitpas_user_profile_activities_content_type_edit_form_submit(&$form, &$form_state) {
  $form_state['conf']['limit'] = $form_state['values']['limit'];
}

function uitpas_ui_uitpas_user_profile_activities_content_type_render($subtype, $conf, $args, $context) {
  if ($context->data) {
    $uitpas_user = $context->data->user;
    $cf = $context->data->cf;
    $limit = $conf['limit'];
    $page = pager_find_page(0);
    $total = 0;
    try {
      $query = new CultureFeed_Uitpas_Event_Query_SearchCheckinsOptions();
      $query->uid = $uitpas_user->id;
      $query->start = $page * $limit;
      $query->max = $limit;
      $result = $cf->uitpas()->searchCheckins($query);
      pager_default_initialize($result->total, $limit, 0);
      $block = new stdClass();
      $block->title = t('My activities');
      $block->content = theme('uitpas_ui_user_profile_activities', array(
        'activities' => $result->objects,
        'activities_total' => $result->total,
      ));
      return $block;
    }
    catch (Exception $e) {
      watchdog_exception('uitpas_ui_content_type_user_profile_promotions_cashed_in', $e);
    }
  }
}
