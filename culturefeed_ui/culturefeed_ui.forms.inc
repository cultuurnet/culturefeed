<?php

function culturefeed_ui_activities_filter_form($form, &$form_state, $type = NULL) {
  $form = array();
  
  $types = array(-1 => 'Alle acties') + culturefeed_ui_activity_types();

  $form['type'] = array(
    '#type' => 'select',
    '#title' => 'Toon enkel',
    '#default_value' => $type ? $type : -1,
    '#options' => $types,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Filter',
  );

  return $form;
}

function culturefeed_ui_activities_filter_form_submit($form, &$form_state) {
  $type = $form_state['values']['type'];

  if ($type > -1) {
    drupal_goto('culturefeed/activities/' . $type);
  }
  else {
    drupal_goto('culturefeed/activities');
  }
}

function culturefeed_ui_recommendations_filter_form($form, &$form_state, $zip = NULL) {
  $form = array();

  $form['zip'] = array(
    '#type' => 'textfield',
    '#title' => 'Toon enkel aanbevelingen voor postcode',
    '#autocomplete_path' => 'cnapi/autocomplete/location/onlyzip',
    '#default_value' => $zip ? $zip : '',
    '#size' => 4,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Filter',
  );

  return $form;
}

function culturefeed_ui_recommendations_filter_form_submit($form, &$form_state) {
  $zip = $form_state['values']['zip'];
  
  if (!empty($zip)) {
    drupal_goto('culturefeed/recommendations/' . $zip);
  }
  else {
    drupal_goto('culturefeed/recommendations');
  }
}

function culturefeed_ui_users_search_form() {
  $form = array();

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => 'Naam',
    '#size' => 20,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Zoeken',
  );

  return $form;
}

function culturefeed_ui_users_search_form_submit($form, &$form_state) {
  $name = trim($form_state['values']['name']);

  if (!empty($name)) {
    $form_state['redirect'] = 'culturefeed/users/search/' . $name;
  }
  else {
    $form_state['redirect'] = 'culturefeed/users/search';
  }
}