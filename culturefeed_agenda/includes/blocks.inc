<?php
/**
 * @file
 * Defines blocks which add integration to culturefeed.
 */

/**
 * Form function for a search box in all searchable types.
 * 
 * @param array $form
 * @param array $form_state
 * @return array
 */
function culturefeed_agenda_search_block_form($form, & $form_state) {

  $searchable_types = culturefeed_get_searchable_types();

  $options = array();
  foreach ($searchable_types as $key => $type) {
    $options[$type['path']] = $type['title'];
  }
  //$form['#action'] = url($type['path'], array('absolute' => TRUE));
  
  $form['type'] = array(
    '#type' => 'select',
    '#title' => 'Zoeken in',
    '#default_value' => key($options),
    '#options' => $options,
  );
  
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

/**
 * Submit function for a search box in all searchable types.
 *
 * @param array $form
 * @param array $form_state
 */
function culturefeed_agenda_search_block_form_submit($form, & $form_state) {
  drupal_goto($form_state['values']['type'], array('query' => array(
      'search' => $form_state['values']['search'], 
      'sort' => 'relevancy')));
}
