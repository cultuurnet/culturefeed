<?php

/**
 * @file
 * Contains the culturefeed location control custom functions.
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
function culturefeed_location_control_custom_pre_render(array $element) {

  $custom = $element['#value'];
  $country = isset($custom['country']) && $custom['country'] ? $custom['country'] : variable_get('culturefeed_entry_ui_default_country');

  if ($country == 'BE') {
    $element['zip']['#access'] = FALSE;
  }
  else {
    $element['number']['#access'] = FALSE;
  }

  return $element;

}

/**
 * Sets the culturefeed location control custom form elements.
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
function culturefeed_location_control_custom_process(array $element, array &$form_state, array $form) {

  $element_id = $element['#id'];
  $array_parents = $element['#array_parents'];

  $element['#prefix'] = "<div id=\"" . $element_id . "-ajax-wrapper\">";
  $element['#suffix'] = "</div>";

  $custom = $element['#value'];

  $ajax_settings = array(
    'effect' => 'none',
    'path' => 'culturefeed_elements/ajax/' . implode('/', $array_parents),
    'progress' => array('type' => 'throbber'),
    'wrapper' => $element_id . '-ajax-wrapper',
  );

  $country = isset($custom['country']) && $custom['country'] ? $custom['country'] : variable_get('culturefeed_entry_ui_default_country');

  $element['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Location Title'),
    '#default_value' => isset($custom['title']) ? $custom['title'] : '',
    '#required' => FALSE,
    '#size' => 80,
  );

  $element['country'] = array(
    '#ajax' => $ajax_settings,
    '#default_value' => $country,
    '#required' => FALSE,
    '#title' => t('Country'),
    '#type' => 'select',
    '#options' => country_get_list(),
  );

  $element['zip'] = array(
    '#access' => TRUE,
    '#attributes' => array(
      'style' => array('margin-right:30px;'),
    ),
    '#default_value' => isset($custom['zip']) ? $custom['zip'] : '',
    '#required' => FALSE,
    '#size' => 10,
    '#title' => t('ZIP Code'),
    '#type' => 'textfield',
  );

  $element['city'] = array(
    '#autocomplete_path' => ($country == 'BE') ? 'autocomplete/culturefeed/city-suggestion' : '',
    '#default_value' => isset($custom['city']) ? $custom['city'] : '',
    '#required' => FALSE,
    '#size' => 60,
    '#title' => t('City'),
    '#type' => 'textfield',
  );

  $element['street'] = array(
    '#default_value' => isset($custom['street']) ? $custom['street'] : '',
    '#required' => FALSE,
    '#size' => 60,
    '#title' => ($country == 'BE') ? t('Street') : t('Street and House Number'),
    '#type' => 'textfield',
  );

  $element['number'] = array(
    '#access' => TRUE,
    '#default_value' => isset($custom['number']) ? $custom['number'] : '',
    '#required' => FALSE,
    '#size' => 10,
    '#title' => t('House Number'),
    '#type' => 'textfield',
  );

  if (isset($element['#location_control'])) {
    $element_id = $element['#location_control']['#id'];
    $array_parents = $element['#location_control']['#array_parents'];
  }

  $element['custom_remove'] = array(
    '#access' => TRUE,
    '#ajax' => array(
      'path' => 'culturefeed_elements/ajax/' . implode('/', $array_parents),
      'wrapper' => $element_id . '-ajax-wrapper',
    ) + $ajax_settings,
    '#attributes' => array('class' => array('add-more-link btn-link')),
    '#limit_validation_errors' => array($element['#parents']),
    '#name' => 'custom_remove',
    '#submit' => array('culturefeed_location_control_location_control_submit'),
    '#type' => 'submit',
    '#value' => t('Remove address'),
  );

  return $element;

}

/**
 * Validate and alter the culturefeed location control custom form values.
 *
 * @param array $element
 *   The element.
 * @param array $form_state
 *   The form state.
 */
function culturefeed_location_control_custom_validate(array &$element, array &$form_state) {

  $triggering_element = (isset($form_state['triggering_element'])) ? $form_state['triggering_element'] : '';
  $triggering_parent = array_pop($triggering_element['#parents']);
  $custom = $element['#value'];

  if (culturefeed_elements_check_trigger_element($triggering_element, $element)) {

    switch ($triggering_parent) {

      case 'custom_remove':

        $custom['city'] = '';
        $custom['country'] = '';
        $custom['number'] = '';
        $custom['street'] = '';
        $custom['title'] = '';
        $custom['zip'] = '';

    }

    drupal_array_set_nested_value($form_state['input'], $element['#parents'], $custom);

  }

}

/**
 * Value callback for the culturefeed location control custom element.
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
function culturefeed_location_control_custom_value_callback(array $element, $input, array $form_state) {

  if ($input) {
    return $input;
  }
  else {

    $value = array(
      'city' => '',
      'country' => '',
      'number' => '',
      'street' => '',
      'title' => '',
      'zip' => '',
    );

    if (isset($element['#default_value']) && $element['#default_value']) {

      $default_value = $element['#default_value'];
      $value['city'] = $default_value['city'];
      $value['country'] = $default_value['country'];
      $value['number'] = $default_value['number'];
      $value['street'] = $default_value['street'];
      $value['title'] = $default_value['title'];
      $value['zip'] = $default_value['zip'];

    }

    return $value;

  }

}
