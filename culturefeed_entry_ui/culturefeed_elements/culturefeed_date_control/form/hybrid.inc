<?php

/**
 * @file
 * Contains the culturefeed date control hybrid functions.
 */

/**
 * Pre renders the culturefeed date control hybrid.
 *
 * @param array $element
 *   The element.
 *
 * @return array $element
 *   The element.
 */
function culturefeed_date_control_hybrid_pre_render(array $element) {

  $hybrid = $element['#value'];

  if (isset($hybrid['end_date']) && isset($hybrid['end_time'])) {
    $element['add_end_date_or_time']['#access'] = FALSE;
  }
  else {
    $element['end_date']['#access'] = FALSE;
    $element['end_time']['#access'] = FALSE;
  }
  return $element;

}

/**
 * Sets the culturefeed date control hybrid form elements.
 *
 * @param array $element
 *   The element.
 * @param array $form_state
 *   The form state.
 * @param array $form
 *   The form.
 *
 * @return array
 *   The element.
 */
function culturefeed_date_control_hybrid_process(array $element, array &$form_state, array $form) {

  $element_id = $element['#id'];
  $array_parents = $element['#array_parents'];

  $element['#prefix'] = "<div id=\"" . $element_id . "-ajax-wrapper\">";
  $element['#suffix'] = "</div>";

  if (isset($element['#date_control'])) {
    $element_id = $element['#date_control']['#id'];
    $array_parents = $element['#date_control']['#array_parents'];
  }

  $hybrid = $element['#value'];

  $ajax_settings = array(
    'effect' => 'none',
    'path' => 'culturefeed_elements/ajax/' . implode('/', $array_parents),
    'progress' => array('type' => 'throbber'),
    'wrapper' => $element_id . '-ajax-wrapper',
  );

  $ajax_button = array(
    '#access' => TRUE,
    '#ajax' => $ajax_settings,
    '#attributes' => array('class' => array('add-more-link btn-link')),
    '#limit_validation_errors' => array($element['#parents']),
    '#submit' => array('culturefeed_date_control_date_control_submit'),
    '#type' => 'submit',
  );

  $date = isset($hybrid['start_date']) && $hybrid['start_date'] ? culturefeed_date_control_implode_date($element, $hybrid['start_date']) : '';
  $element['start_date'] = array(
    '#date_format' => 'd-m-Y',
    '#date_label_position' => 'none',
    '#default_value' => $date,
    '#icon' => TRUE,
    '#required' => TRUE,
    '#title' => t('Date'),
    '#type' => 'date_popup',
  );

  $time = isset($hybrid['start_time']) && $hybrid['start_time'] ? culturefeed_date_control_implode_time($hybrid['start_time'], $date) : '';
  $element['start_time'] = array(
    '#attributes' => array('class' => array('time-select-complete-minutes')),
    '#date_format' => 'H:i',
    '#date_increment' => 15,
    '#default_value' => $time,
    '#title' => '',
    '#type' => 'date_select',
  );

  $element['add_end_date_or_time'] = $ajax_button + array(
    '#value' => t('+ add end date or time'),
  );

  $element['end_date'] = array(
    '#access' => TRUE,
    '#ajax' => $ajax_settings,
    '#date_format' => 'd-m-Y',
    '#date_label_position' => 'none',
    '#default_value' => '',
    '#icon' => TRUE,
    '#title' => t('Date'),
    '#type' => 'date_popup',
  );

  $element['end_time'] = array(
    '#access' => TRUE,
    '#attributes' => array('class' => array('time-select-complete-minutes')),
    '#ajax' => $ajax_settings,
    '#date_format' => 'H:i',
    '#date_increment' => 15,
    '#default_value' => '',
    '#title' => '',
    '#type' => 'date_select',
  );

  $element['add_date'] = $ajax_button + array(
    '#value' => t('+ add date'),
  );

  // Value will always be 0 as a different element is used when actually
  // permanent.
  $element['permanent'] = array(
    '#ajax' => $ajax_settings,
    '#title' => t('Permanent'),
    '#type' => 'checkbox',
    '#value' => 0,
  );

  $element['#attached'] = array(
    'js' => array(
      drupal_get_path('module', 'culturefeed_date_control') . '/js/culturefeed_date_control_time_select.js',
    ),
  );

  return $element;

}

/**
 * Validate and alter the culturefeed date control hybrid form values.
 *
 * @param array $element
 *   The element.
 * @param array $form_state
 *   The form state.
 */
function culturefeed_date_control_hybrid_validate(array &$element, array &$form_state) {

  $triggering_element = (isset($form_state['triggering_element'])) ? $form_state['triggering_element'] : '';
  $triggering_parent = array_pop($triggering_element['#parents']);
  $hybrid = $element['#value'];

  if (culturefeed_elements_check_trigger_element($triggering_element, $element)) {

    switch ($triggering_parent) {

      case 'add_end_date_or_time':

        $hybrid['end_date'] = array('date' => '');
        $hybrid['end_time'] = array('hour' => '', 'minute' => '');

    }

    drupal_array_set_nested_value($form_state['input'], $element['#parents'], $hybrid);

  }

}

/**
 * Value callback for the culturefeed date control hybrid element.
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
function culturefeed_date_control_hybrid_value_callback(array $element, $input, array $form_state) {

  if ($input) {
    return $input;
  }
  else {

    $value = array('start_date' => '', 'start_time' => '');

    if (isset($element['#default_value']) && $element['#default_value']) {

      $default_value = $element['#default_value'];
      if (isset($default_value['start_date']) && $default_value['start_date']) {
        $value['start_date'] = culturefeed_date_control_explode_date($element, $default_value['start_date']);
        if (isset($element['#default_value']['start_time']) && $element['#default_value']['start_time']) {
          $value['start_time'] = culturefeed_date_control_explode_time($element, $default_value['start_time'], $default_value['start_date']);
        }
      }

    }

    return $value;

  }

}
