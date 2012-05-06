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
  'defaults' => array(
    'path' => '',
    'label' => '',
  ),
);

function uitpas_ui_uitpas_location_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#default_value' => $conf['path'],
  );
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => $conf['label'],
  );
  return $form;
}

function uitpas_ui_uitpas_location_content_type_edit_form_submit($form, &$form_state) {
  $form_state['conf']['path'] = $form_state['values']['path'];
  $form_state['conf']['label'] = $form_state['values']['label'];
}

function uitpas_ui_uitpas_location_content_type_render($subtype, $conf, $args, $context) {
  $location = $context->data->location;
  $locations = $context->data->locations;
  $block = new stdClass();
  $block->title = "";
  $block->content = drupal_get_form('uitpas_ui_uitpas_location_content_type_render_form', $conf, $locations, $location);
  return $block;
}

function uitpas_ui_uitpas_location_content_type_render_form($form, $form_state, $conf, $locations, $location) {
  $form['path'] = array(
    '#type' => 'value',
    '#value' => $conf['path'],
  );
  $options = array('all' => '');
  foreach ($locations as $key => $info) {
    $options[$key] = $info['name'];
  }
  $form['location'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => $conf['label'],
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

function uitpas_ui_uitpas_location_content_type_render_form_submit(&$form, &$form_state) {
  $location = $form_state['values']['location'];
  $path = $form_state['values']['path'];
  $form_state['redirect'] = $path . '/' . $location;
}
