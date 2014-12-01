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

  $element['#prefix'] = "<div id=\"" . $element['#id'] . "-ajax-wrapper\">";
  $element['#suffix'] = "</div>";

  $ajax_button = array(
    '#access' => TRUE,
    '#ajax' => array(
      'effect' => 'none',
      'path' => 'culturefeed_elements/ajax/' . implode('/', $element['#array_parents']),
      'progress' => array('type' => 'throbber'),
      'wrapper' => $element['#id'] . '-ajax-wrapper',
    ),
    '#attributes' => array('class' => array('add-more-link btn-link')),
    // '#limit_validation_errors' => array($element['#parents']),
    '#submit' => array('culturefeed_date_control_date_control_submit'),
    '#type' => 'submit',
  );

  $element['start_date'] = array(
    '#date_format' => 'd-m-Y',
    '#date_label_position' => 'none',
    '#default_value' => '',
    '#icon' => TRUE,
    '#title' => t('Start date'),
    '#type' => 'date_popup',
  );

  $element['add_end'] = $ajax_button + array(
    '#value' => t('+ add end date'),
    '#name' => 'add_end',
  );

  $element['end_date'] = array(
    '#access' => TRUE,
    '#date_format' => 'd-m-Y',
    '#date_label_position' => 'none',
    '#default_value' => '',
    '#icon' => TRUE,
    '#title' => t('End date'),
    '#type' => 'date_popup',
  );

  $element['remove_end'] = $ajax_button + array(
    '#value' => t('- remove end date'),
    '#name' => 'remove_end',
  );

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

  if ($triggering_element && culturefeed_date_control_check_trigger_element($triggering_element, $element)) {

    $dates = $element['#value'];
    $triggering_parent = array_pop($triggering_element['#parents']);

    switch ($triggering_parent) {

      case 'add_end':
        $dates['end_date'] = '';
        break;

      case 'remove_end':
        unset($dates['end_date']);
        break;

    }

    drupal_array_set_nested_value($form_state['values'], $triggering_element['#parents'], $dates);
    drupal_array_set_nested_value($form_state['input'], $triggering_element['#parents'], $dates);

  }

}
