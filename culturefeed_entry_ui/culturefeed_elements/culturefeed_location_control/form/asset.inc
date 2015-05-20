<?php

/**
 * @file
 * Contains the culturefeed location control asset functions.
 */

/**
 * Sets the culturefeed location control asset form elements.
 *
 * @param array $element
 *   The element.
 * @param array $form_state
 *   The form state.
 * @param array $form
 *   The form.
 *
 * @return array
 *   The processed element.
 */
function culturefeed_location_control_asset_process(array $element, array &$form_state, array $form) {

  $element_id = $element['#id'];
  $array_parents = $element['#array_parents'];

  $element['#prefix'] = "<div id=\"" . $element_id . "-ajax-wrapper\">";
  $element['#suffix'] = "</div>";

  if (isset($element['#location_control'])) {
    $element_id = $element['#location_control']['#id'];
    $array_parents = $element['#location_control']['#array_parents'];
  }

  $asset = $element['#value'];

  $ajax_settings = array(
    'effect' => 'none',
    'path' => 'culturefeed_elements/ajax/' . implode('/', $array_parents),
    'progress' => array('type' => 'throbber'),
    'wrapper' => $element_id . '-ajax-wrapper',
  );

  $ajax_button = array(
    '#access' => TRUE,
    '#ajax' => $ajax_settings,
    '#attributes' => array(
      'class' => array('add-more-link btn-link'),
    ),
    '#limit_validation_errors' => array($element['#parents']),
    '#submit' => array('culturefeed_location_control_location_control_submit'),
    '#type' => 'submit',
  );

  $element['label'] = array(
    '#autocomplete_path' => 'autocomplete/culturefeed/actor-suggestions/all',
    '#default_value' => isset($asset['label']) ? $asset['label'] : '',
    '#required' => FALSE,
    '#size' => 80,
    '#title' => t('Location'),
    '#type' => 'textfield',
  );
  if ($asset['id']) {
    $element['label']['#attributes']['readonly'] = 'readonly';
  }

  // @see culturefeed_entry_ui.js.
  $element['id'] = array(
    '#attributes' => array(
      'id' => 'location_actor_id',
    ),
    '#default_value' => isset($asset['id']) ? $asset['id'] : '',
    '#type' => 'hidden',
  );

  $element['asset_remove'] = $ajax_button + array(
    '#id' => 'location_asset_remove',
    '#name' => 'asset_remove',
    '#value' => t('Remove address'),
  );
  if (!$asset['id']) {
    $element['asset_remove']['#attributes']['style'] = 'display: none;';
  }

  $element['new'] = $ajax_button + array(
    '#id' => 'location_custom_add',
    '#name' => 'new',
    '#value' => t('Location not found, add location'),
  );
  $element['new']['#attributes']['style'] = 'display: none;';

  return $element;

}

/**
 * Validate and alter the culturefeed location control asset form values.
 *
 * @param array $element
 *   The element.
 * @param array $form_state
 *   The form state.
 */
function culturefeed_location_control_asset_validate(array &$element, array &$form_state) {

  $triggering_element = (isset($form_state['triggering_element'])) ? $form_state['triggering_element'] : '';
  $triggering_parent = array_pop($triggering_element['#parents']);
  $asset = $element['#value'];

  if (culturefeed_elements_check_trigger_element($triggering_element, $element)) {

    switch ($triggering_parent) {

      case 'asset_remove':

        $asset['id'] = '';
        $asset['label'] = '';

    }

    drupal_array_set_nested_value($form_state['input'], $element['#parents'], $asset);

  }

}

/**
 * Value callback for the culturefeed location control asset element.
 *
 * @param array $element
 *   The element.
 * @param mixed $input
 *   The input.
 * @param array $form_state
 *   The form state.
 *
 * @return mixed
 *   The value.
 */
function culturefeed_location_control_asset_value_callback(array $element, $input, array $form_state) {

  if ($input) {
    return $input;
  }
  else {

    $value = array(
      'id' => '',
      'label' => '',
    );

    if (isset($element['#default_value']) && $element['#default_value']) {

      $default_value = $element['#default_value'];
      $value['id'] = $default_value['id'];
      $value['label'] = $default_value['label'];

    }

    return $value;

  }

}
