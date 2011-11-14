<?php

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