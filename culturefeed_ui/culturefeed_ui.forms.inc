<?php

function culturefeed_ui_recommendations_filter_form() {
  $form = array();
  
  $zip = NULL;
  
  if (arg(0) == 'culturefeed' && arg(1) == 'recommendations' && arg(2)) {
    $zip = arg(2);
  }

  $form['zip'] = array(
    '#type' => 'textfield',
    '#title' => 'Postcode',
    '#autocomplete_path' => 'cnapi/autocomplete/location/onlyzip',
    '#default_value' => $zip ? $zip : '',
    '#size' => 4,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Filter',
  );

  return $form;
}

function culturefeed_ui_recommendations_filter_form_submit($form, &$form_state) {
  $zip = $form_state['values']['zip'];
  
  drupal_goto('culturefeed/recommendations/' . $zip);
}

function culturefeed_ui_search_users_form() {
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

function culturefeed_ui_search_users_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $form_state['redirect'] = 'culturefeed/users/search' . (!empty($values['name']) ? '/' . trim($values['name']) : '');
}

function culturefeed_ui_is_culturefeed_user($uid = NULL) {
  return DrupalCultureFeed::isCultureFeedUser($uid);
}