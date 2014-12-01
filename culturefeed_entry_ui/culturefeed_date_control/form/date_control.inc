<?php

/**
 * @file
 * Contains the functions for the culturefeed date control form element.
 */

/**
 * Returns the culturefeed date control element.
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
function culturefeed_date_control_date_control_process(array $element, array &$form_state, array $form) {

  $element['#prefix'] = "<div id=\"" . $element['#id'] . "-ajax-wrapper\">";
  $element['#suffix'] = "</div>";

  $element['date_control'] = array(
    '#type' => 'fieldset',
    '#title' => (isset($element['#title'])) ? $element['#title'] : t('Dates'),
  );

  // Timestamps.
  $element['date_control']['timestamps'] = array(
    '#type' => 'culturefeed_date_control_timestamps',
  );

  $element['#attached'] = array(
    'css' => array(
      drupal_get_path('module', 'culturefeed_date_control') . '/css/culturefeed_date_control.css',
    ),
  );

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
function culturefeed_date_control_date_control_submit(array $form, array &$form_state) {

  $form_state['rebuild'] = TRUE;

}

/**
 * Value callback for the date control element.
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
function culturefeed_date_control_date_control_value_callback(array &$element, $input, array $form_state) {

  if ($input) {
    return $input;
  }

}

/**
 * Returns the date control type based on the values.
 *
 * @param array $values
 *   The element values.
 *
 * @return string
 *   The type.
 */
function culturefeed_date_control_get_type(array $values) {

  if (isset($values['period']['start']) && $values['period']['end']) {
    return 'period';
  }
  if (isset($values['permanent'])) {
    return 'permanent';
  }
  else {
    // Default setting.
    return 'timestamps';
  }

}
