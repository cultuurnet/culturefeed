<?php

/**
 * @file
 * Plugin to provide a content type for uitpas where to register.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Uitpas: register where'),
  'description' => t('Uitpas: register where.'),
  'category' => t('Uitpas'),
  'required context' => new ctools_context_required(t('Uitpas location'), 'uitpas_location'),
  'defaults' => array(
    'limit' => 10,
  ),
);

function uitpas_ui_uitpas_register_where_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form['limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Page limit'),
    '#default_value' => $conf['limit'],
  );
  return $form;
}

function uitpas_ui_uitpas_register_where_content_type_edit_form_submit(&$form, &$form_state) {
  $form_state['conf']['limit'] = $form_state['values']['limit'];
}

function uitpas_ui_uitpas_register_where_content_type_render($subtype, $conf, $args, $context) {
  $location = $context->data->cf_query_argument;
  $pos = array();
  $actors = array();
  $limit = $conf['limit'];
  $page = pager_find_page(0);
  $total = 0;
  try {
    $cf = DrupalCultureFeed::getConsumerInstance();
    $query = new CultureFeed_Uitpas_Counter_Query_SearchPointsOfSaleOptions;
    $query->start = $page * $limit;
    $query->max = $limit;
    if ($location) {
      $query->city = $location;
    }
    $result = $cf->uitpas()->searchPointOfSales($query);
    if ($result->total) {
      $page = pager_default_initialize($result->total, $limit, 0);
      $pos = $result->objects;
      $total = $result->total;
      foreach ($result->objects as $object) {
        $actor = cnapi_get_actor($object->id);
        if ($actor) {
          $actors[$object->id] = $actor;
        }
      }
    }
  }
  catch (Exception $e) {
    watchdog_exception('uitpas_ui_content_type_register_where', $e);
  }
  $block = new stdClass();
  $block->title = t('Register your UITPAS');
  $block->content = theme('uitpas_ui_register_where', array(
    'pos' => $pos, 
    'actors' => $actors, 
    'total' => $total,
  ));
  return $block;
}