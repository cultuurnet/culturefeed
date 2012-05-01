<?php

/**
 * @file
 * Plugin to provide a content type for uitpas location selection form.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Uitpas: select location'),
  'description' => t('Uitpas: select location.'),
  'category' => t('Uitpas'),
  'required context' => new ctools_context_required(t('Uitpas location'), 'uitpas_location'),
);

function culturefeed_uitpas_location_content_type_edit_form($form, &$form_state) {
  return $form;
}

function culturefeed_uitpas_location_content_type_render($subtype, $conf, $args, $context) {
  $location = $context->data->location;
  $locations = $context->data->locations;
  $block = new stdClass();
  $block->title = "";
  $block->content = drupal_get_form('culturefeed_uitpas_location_content_type_render_form', $locations, $location);
  return $block;
}

function culturefeed_uitpas_location_content_type_render_form($form, $form_state, $locations, $location) {
  $options = array();
  foreach ($locations as $key => $info) {
    $options[$key] = $info['name'];
  }
  $form['location'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => t('Where to save points in'),
    '#default_value' => $location,
  );
  $form['actions'] = array(
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('search'),
    ),
  );
  return $form;
}

function culturefeed_uitpas_location_content_type_render_form_submit(&$form, &$form_state) {
  if ($form_state['values']['location']) {
    $form_state['redirect'] = 'home/' . $form_state['values']['location'];
  }
}
