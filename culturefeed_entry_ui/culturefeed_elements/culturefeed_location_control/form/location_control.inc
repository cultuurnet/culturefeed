<?php

/**
 * @file
 * Contains the functions for the culturefeed location control form element.
 */

/**
 * Pre renders the culturefeed location control custom.
 *
 * @param array $element
 *   The element.
 *
 * @return array $element
 *   The element.
 */
function culturefeed_location_control_location_control_pre_render(array $element) {

  $location_control = $element['#value'];

  if ($location_control['location_control']['type'] == 'asset') {
    $element['location_control']['custom']['#access'] = FALSE;
  }
  elseif ($location_control['location_control']['type'] == 'custom') {
    $element['location_control']['asset']['#access'] = FALSE;
  }

  return $element;

}

/**
 * Returns the culturefeed location control element.
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
function culturefeed_location_control_location_control_process(array $element, array &$form_state, array $form) {

  $location_control = $element['#value']['location_control'];

  $element['location_control'] = array(
    '#attributes' => array(
      'class' => array('clearfix'),
      'id' => $element['#id'] . '-ajax-wrapper',
    ),
    '#type' => 'container',
  );

  // Type.
  $element['location_control']['type'] = array(
    '#type' => 'hidden',
    '#value' => $location_control['type'],
  );

  // Asset.
  $element['location_control']['asset'] = array(
    '#access' => TRUE,
    '#default_value' => isset($location_control['asset']) ? $location_control['asset'] : '',
    '#location_control' => $element,
    '#type' => 'culturefeed_location_control_asset',
  );

  // Custom.
  $element['location_control']['custom'] = array(
    '#access' => TRUE,
    '#default_value' => isset($location_control['custom']) ? $location_control['custom'] : '',
    '#location_control' => $element,
    '#type' => 'culturefeed_location_control_custom',
  );

  if (isset($element['#description'])) {
    $element['location_control']['description'] = array(
      '#attributes' => array(
        'class' => array('description'),
      ),
      '#type' => 'container',
      'markup' => array(
        '#markup' => $element['#description'],
      ),
    );
  }

  return $element;

}

/**
 * Submit handler for the ajax buttons.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 */
function culturefeed_location_control_location_control_submit(array $form, array &$form_state) {

  $form_state['rebuild'] = TRUE;

}

/**
 * Validate and alter the culturefeed location control form values.
 *
 * This is where the element switching is set, based on values.
 *
 * @param array $element
 *   The element.
 * @param array $form_state
 *   The form state.
 */
function culturefeed_location_control_location_control_validate(array $element, array &$form_state) {

  $triggering_element = (isset($form_state['triggering_element'])) ? $form_state['triggering_element'] : '';
  $triggering_parent = array_pop($triggering_element['#parents']);

  // Use form_state instead of element as individual sub elements have changes
  // that won't be reflected in element.
  $values = drupal_array_get_nested_value($form_state['input'], $element['#parents']);
  $location_control = $values['location_control'];

  if (culturefeed_elements_check_trigger_element($triggering_element, $element)) {

    $type = $location_control['type'];

    // Custom.
    if ($triggering_parent == 'new') {

      $type = 'custom';
      if (isset($location_control['asset']['label']) && $location_control['asset']['label']) {
        $location_control['custom']['title'] = $location_control['asset']['label'];
      }

    }

    // Remove.
    if ($triggering_parent == 'asset_remove' || $triggering_parent == 'custom_remove') {

      $type = 'asset';

    }

    $location_control['type'] = $type;
    $element['#parents'][] = 'location_control';

    drupal_array_set_nested_value($form_state['input'], $element['#parents'], $location_control);
  }

}

/**
 * Value callback for the location control element.
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
function culturefeed_location_control_location_control_value_callback(array &$element, $input, array $form_state) {

  if ($input) {
    return $input;
  }
  else {

    $value = array(
      'location_control' => array(
        'type' => 'asset',
        'asset' => array(),
        'custom' => array(),
      ),
    );

    if (isset($element['#default_value']) && $element['#default_value']) {

      $location_control = $value['location_control'];
      $default_value = $element['#default_value'];

      // Asset.
      if ($default_value['asset']['id']) {
        $location_control['asset'] = $default_value['asset'];
        $location_control['type'] = 'asset';
      }

      // Custom.
      if ($default_value['custom']['title']) {
        $location_control['custom'] = $default_value['custom'];
        $location_control['type'] = 'custom';
      }

      $value['location_control'] = $location_control;

    }

    return $value;

  }

}
