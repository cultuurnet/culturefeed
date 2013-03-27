<?php
/**
 * @file
 * Defines blocks which add integration to culturefeed.
 */

function culturefeed_agenda_block_form($form, & $form_state) {
  
  $form['search'] = array(
    '#type' => 'textfield',
    '#title' => 'Zoekopdracht',
    '#default_value' => '',
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Zoeken',
  );
  
  return $form;
}