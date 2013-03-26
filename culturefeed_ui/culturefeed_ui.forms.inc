<?php

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