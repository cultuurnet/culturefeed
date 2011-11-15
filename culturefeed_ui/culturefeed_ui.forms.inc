<?php

function culturefeed_ui_activities_filter_form() {
  $form = array();

  $type = NULL;

  if (arg(0) == 'culturefeed' && arg(1) == 'activities' && arg(2)) {
    $type = arg(2);
  }
  
  $types = array(
    -1                                  => 'Alle acties',
    CultureFeed_Activity::TYPE_VIEW     => 'Bekeken',
    CultureFeed_Activity::TYPE_DETAIL   => 'Bekeken',
    CultureFeed_Activity::TYPE_LIKE     => 'Vind ik leuk',
    CultureFeed_Activity::TYPE_MAIL     => 'Gemaild',
    CultureFeed_Activity::TYPE_PRINT    => 'Geprint',
    CultureFeed_Activity::TYPE_FACEBOOK => 'Gedeeld op Facebook',
    CultureFeed_Activity::TYPE_TWITTER  => 'Gedeeld op Twitter',
    CultureFeed_Activity::TYPE_IK_GA    => 'Ga hier naartoe',
  );

  $form['type'] = array(
    '#type' => 'select',
    '#title' => 'Type',
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