<?php

/**
 * @file
 * Contains the culturefeed date control period functions.
 */

/**
 * Pre renders the culturefeed date control timestamps.
 *
 * @param array $element
 *   The element.
 *
 * @return array $element
 *   The element.
 */
function culturefeed_date_control_period_pre_render(array $element) {

  $dates = $element['#value'];

  if (isset($dates['end_date'])) {
    $element['add_end']['#access'] = FALSE;
  }
  else {
    $element['end_date']['#access'] = FALSE;
    $element['remove_end']['#access'] = FALSE;
  }

  return $element;

}

/**
 * Sets the culturefeed date control period form elements.
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
function culturefeed_date_control_period_process(array $element, array &$form_state, array $form) {

  $element_id = $element['#id'];
  $array_parents = $element['#array_parents'];

  $element['#prefix'] = "<div id=\"" . $element_id . "-ajax-wrapper\">";
  $element['#suffix'] = "</div>";

  if (isset($element['#date_control'])) {
    $element_id = $element['#date_control']['#id'];
    $array_parents = $element['#date_control']['#array_parents'];
  }

  $period = $element['#value'];

  $ajax_button = array(
    '#access' => TRUE,
    '#ajax' => array(
      'effect' => 'none',
      'path' => 'culturefeed_elements/ajax/' . implode('/', $array_parents),
      'progress' => array('type' => 'throbber'),
      'wrapper' => $element_id . '-ajax-wrapper',
    ),
    '#attributes' => array('class' => array('add-more-link btn-link')),
    '#limit_validation_errors' => array($element['#parents']),
    '#submit' => array('culturefeed_date_control_date_control_submit'),
    '#type' => 'submit',
  );

  $date = isset($period['start_date']) && $period['start_date'] ? culturefeed_date_control_implode_date($element, $period['start_date']) : '';
  $element['start_date'] = array(
    '#date_format' => 'd-m-Y',
    '#date_label_position' => 'none',
    '#default_value' => $date,
    '#icon' => TRUE,
    '#required' => TRUE,
    '#title' => t('Start date'),
    '#type' => 'date_popup',
  );

  $element['add_end'] = array(
    '#name' => 'add_end',
    '#value' => t('+ add end date'),
  ) + $ajax_button;

  $date = isset($period['end_date']) && $period['end_date'] ? culturefeed_date_control_implode_date($element, $period['end_date']) : '';
  $element['end_date'] = array(
    '#access' => TRUE,
    '#date_format' => 'd-m-Y',
    '#date_label_position' => 'none',
    '#default_value' => $date,
    '#icon' => TRUE,
    '#required' => TRUE,
    '#title' => t('End date'),
    '#type' => 'date_popup',
  );

  $element['remove_end'] = array(
    '#limit_validation_errors' => array(),
    '#name' => 'remove_end',
    '#value' => t('- remove end date'),
  ) + $ajax_button;

  if (!isset($element['#date_control'])) {
    $element['#attached'] = array(
      'css' => array(
        drupal_get_path('module', 'culturefeed_date_control') . '/css/culturefeed_date_control.css',
      ),
    );
  }

  return $element;

}

/**
 * Validate and alter the culturefeed date control timestamps form values.
 *
 * @param array $element
 *   The element.
 * @param array $form_state
 *   The form state.
 */
function culturefeed_date_control_period_validate(array $element, array &$form_state) {

  $triggering_element = (isset($form_state['triggering_element'])) ? $form_state['triggering_element'] : '';
  $triggering_parent = array_pop($triggering_element['#parents']);
  $dates = $element['#value'];

  if (culturefeed_elements_check_trigger_element($triggering_element, $element)) {

    switch ($triggering_parent) {

      case 'add_end':
        $dates['end_date'] = array('date' => '');
        break;

      case 'remove_end':
        unset($dates['end_date']);
        break;

    }

    drupal_array_set_nested_value($form_state['input'], $element['#parents'], $dates, TRUE);

  }

}

/**
 * Value callback for the culturefeed date control period element.
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
function culturefeed_date_control_period_value_callback(array $element, $input, array $form_state) {

  if ($input) {
    return $input;
  }
  else {

    $value = array('start_date' => '', 'end_date' => '');

    if (isset($element['#default_value']) && $element['#default_value']) {

      $default_value = $element['#default_value'];
      if (
        isset($default_value['start_date']) && $default_value['start_date'] &&
        isset($default_value['end_date']) && $default_value['end_date']
      ) {
        $value['start_date'] = culturefeed_date_control_explode_date($element, $default_value['start_date']);
        $value['end_date'] = culturefeed_date_control_explode_date($element, $default_value['end_date']);
      }

    }

    return $value;

  }

}
