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

  $options = array(
    'geo' => 'In de buurt van mijn <b>huidige locatie</b> (<span class="geo-location"></span>)',
    'zip' => t('In de buurt van'),
  );
  $form['search'] = array(
    '#type' => 'radios',
    '#title' => '',
    '#default_value' => isset($_GET['search']) ? $_GET['search'] : 'zip',
    '#options' => $options,
  );

  $form['geo_zipcode'] = array(
    '#type' => 'hidden',
    '#default_value' => $zip ? $zip : '',
  );

  $form['zip'] = array(
    '#type' => 'textfield',
    '#autocomplete_path' => 'cnapi/autocomplete/location/onlyzip',
    '#default_value' => $zip ? $zip : '',
    '#size' => 4,
  );

  $form['markup'] = array(
    '#markup' => ' + ',
  );

  $form['radius'] = array(
    '#type' => 'textfield',
    '#title' => '',
    '#default_value' => isset($_GET['radius']) ? $_GET['radius'] : '',
    '#maxlength' => 3,
    '#suffix' => 'km in de omtrek',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Filter',
  );

  // load the extra javascript and css for the geolocation feature
  $form['#attached']['css'][] = drupal_get_path('module', 'culturefeed_ui') . '/css/geo.css';
  $form['#attached']['js'][] = 'http://maps.google.com/maps/api/js?sensor=false';
  $form['#attached']['js'][] = drupal_get_path('module', 'culturefeed_ui') . '/js/geo.js';

  return $form;
}

function culturefeed_ui_recommendations_filter_form_submit($form, &$form_state) {
  $zip = $form_state['values']['zip'];
  $zip = reset(explode(' ', $zip));

  $options = array('query' => array());

  if (!empty($form_state['values']['radius']) && is_numeric($form_state['values']['radius'])) {
    $options['query']['radius'] = $form_state['values']['radius'];
  }

  if ($form_state['values']['search'] == 'geo') {
    $options['query']['search'] = $form_state['values']['search'];
    if (!empty($form_state['values']['geo_zipcode']))  {
      $zip = $form_state['values']['geo_zipcode'];
    }
  }

  if (!empty($zip)) {
    drupal_goto('culturefeed/recommendations/' . $zip, $options);
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