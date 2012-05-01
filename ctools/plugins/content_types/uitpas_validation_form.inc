<?php

/**
 * @file
 * Plugin to provide a content type for culturefeed uitpas validation form.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Culturefeed: uitpas validation'),
  'description' => t('Culturefeed: uitpas validation.'),
  'category' => t('Culturefeed'),
  //'required context' => new ctools_context_required(t('Culturefeed: activities location'), 'culturefeed_location'),
);

function culturefeed_culturefeed_uitpas_form_content_type_edit_form($form, &$form_state) {
  return $form;
}

function culturefeed_culturefeed_uitpas_form_content_type_render($subtype, $conf, $args, $context) {
  $block = new stdClass();
  $block->title = t('Allready own an Uitpas ?');
  $block->content = drupal_get_form('culturefeed_culturefeed_uitpas_form_content_type_render_form');
  return $block;
}

function culturefeed_culturefeed_uitpas_form_content_type_render_form($form, $form_state) {
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

function culturefeed_culturefeed_uitpas_form_content_type_render_form_submit(&$form, &$form_state) {
  $number = $form_state['values']['uitpasnumber'];
  $dob = $form_state['values']['dob']['year'] . '-' . $form_state['values']['dob']['month'] . '-' . $form_state['values']['dob']['day'];
  dpm($dob);
  $oauth = new CultureFeed_DefaultOAuthClient(variable_get(), variable_get());
  $cf = new CultureFeed();
  
  //$result = file_get_contents('http://uitpas-acc.lodgon.com:8080/uitid/rest/uitpas/passholder/' . $number . '/activation?dob=' . $dob);
  //dpm($result);
  
  /*try {
    $xml = new CultureFeed_SimpleXMLElement($result);
  }
  catch (Exception $e) {
    throw new CultureFeed_ParseException($result);
  }*/
  //return $xml->xpath_str('/response/events/event/cdbid', TRUE);
  
}
