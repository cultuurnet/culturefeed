<?php

/**
 * @file
 * Plugin to provide a content type for uitpas validation form.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Uitpas: validation form'),
  'description' => t('Uitpas: validation form.'),
  'category' => t('Uitpas'),
);

function uitpas_ui_uitpas_validation_form_content_type_edit_form($form, &$form_state) {
  return $form;
}

function uitpas_ui_uitpas_validation_form_content_type_render($subtype, $conf, $args, $context) {
  $block = new stdClass();
  $block->title = t('Allready own an Uitpas ?');
  $block->content = drupal_get_form('uitpas_ui_uitpas_validation_form_content_type_render_form');
  return $block;
}

function uitpas_ui_uitpas_validation_form_content_type_render_form($form, $form_state) {
  if (isset($form_state['values']['link'])) {
    //as defined in js/culturefeed.js we also reload the page as our page still has the rebuilt form
    $link = $form_state['values']['link'];
    $options = 'location=1,status=1,scrollbars=1,resizable=1,width=810,height=635';
    $script = "window.open('" . $link . "', 'UiTiD', '" . $options . "');";
    drupal_add_js($script, 'inline');
  }
  $form['link'] = array(
    '#type' => 'value',
  );
  $form['uitpasnumber'] = array(
    '#type' => 'textfield',
    '#title' => t('UITPAS number'),
  );
  $form['dob'] = array(
    '#type' => 'date',
    '#title' => t('Date of birth'),
  );
  $form['actions'] = array(
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Continue'),
    ),
  );
  $form['#suffix'] = t('Allready registered ?  Apply here.');
  return $form;
}

function uitpas_ui_uitpas_validation_form_content_type_render_form_validate(&$form, &$form_state) {
  $number = $form_state['values']['uitpasnumber'];
  $dob = $form_state['values']['dob']['year'] . '-' . $form_state['values']['dob']['month'] . '-' . $form_state['values']['dob']['day'];
  $dob = new DateTime($dob);
  
  $cf = DrupalCultureFeed::getConsumerInstance();
  try {
    $link = $cf->uitpas()->getPassholderActivationLinkChainedWithAuthorization($number, $dob, base_path());
    $form_state['values']['link'] = $link;
  }
  catch (Exception $e) {
    if ($e->error_code == 'INVALID_DOB') {
      $error = variable_get('culturefeed_uitpas_invalid_dob_error_meesage', t('Invalid date of birth'));
      form_set_error('dob', $error);
    }
    if ($e->error_code == 'PARSE_INVALID_UITPASNUMBER') {
      $error = variable_get('culturefeed_uitpas_parse_invalid_uitpasnumber_error_meesage', t('Invalid uitpas number'));
      form_set_error('number', $error);
    }
    if ($e->error_code == 'USER_ALREADY_ACTIVATED') {
      $error = variable_get('culturefeed_uitpas_user_already_activated_error_meesage', t('User already activated'));
      form_set_error('number', $error);
    }
  }
}

function uitpas_ui_uitpas_validation_form_content_type_render_form_submit(&$form, &$form_state) {
  $link = $form_state['values']['link'];
  //we rebuild the form so our form definition can popup a window if required
  $form_state['rebuild'] = TRUE;
}