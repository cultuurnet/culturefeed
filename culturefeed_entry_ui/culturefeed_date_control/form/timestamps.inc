<?php

/**
 * @file
 * Contains the culturefeed date control timestamps functions.
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
function culturefeed_date_control_timestamps_pre_render(array &$element) {

  $timestamps = $element['#value']['stamps'];

  // Remove button is not required if only one row.
  if (count($timestamps) < 2) {
    $element['stamps'][0]['remove_stamp']['#access'] = FALSE;
  }

  foreach ($timestamps as $key => $timestamp) {

    if (isset($timestamps[$key]['end_time'])) {
      $element['stamps'][$key]['add_end']['#access'] = FALSE;
      $element['#cell_count'] = 5;
    }
    else {
      $element['stamps'][$key]['end_time']['#access'] = FALSE;
      $element['stamps'][$key]['remove_end']['#access'] = FALSE;
    }
  }

  return $element;

}

/**
 * Sets the date control timestamps form elements.
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
function culturefeed_date_control_timestamps_process(array $element, array &$form_state, array $form) {

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

  // Get the timestamps.
  $timestamps = $element['#value']['stamps'];

  foreach ($timestamps as $key => $timestamp) {

    if ($key !== 'timestamps_add_stamp') {

      $element['stamps'][$key]['remove_stamp'] = $ajax_button + array(
        '#value' => t('×'),
        '#name' => 'remove_stamp_' . $key,
      );

      $element['stamps'][$key]['date'] = array(
        '#type' => 'date_popup',
        '#default_value' => '',
        '#title' => t('Date'),
        '#date_label_position' => 'none',
        '#date_format' => 'd-m-Y',
        '#icon' => TRUE,
      );

      $element['stamps'][$key]['start_time'] = array(
        '#type' => 'date_select',
        '#default_value' => '',
        '#title' => '',
        '#date_format' => 'H:i',
        '#date_increment' => 30,
      );

      $element['stamps'][$key]['end_time'] = array(
        '#type' => 'date_select',
        '#default_value' => '',
        '#title' => '',
        '#date_format' => 'H:i',
        '#date_increment' => 30,
        '#access' => TRUE,
      );

      $element['stamps'][$key]['remove_end'] = $ajax_button + array(
        '#name' => 'remove_end_' . $key,
        '#value' => t('- remove end time'),
      );

      $element['stamps'][$key]['add_end'] = $ajax_button + array(
        '#name' => 'add_end_' . $key,
        '#value' => t('+ add end time'),
      );

    }

  }

  $element['timestamps_add_stamp'] = $ajax_button + array(
    '#name' => 'add_stamp',
    '#value' => t('+ add date'),
  );

  $element['#attached'] = array(
    'css' => array(
      drupal_get_path('module', 'culturefeed_date_control') . '/css/culturefeed_date_control_timestamps.css',
    ),
  );

  return $element;

}

/**
 * Validate and alter the timestamp form values.
 *
 * @param array $element
 *   The element.
 * @param array $form_state
 *   The form state.
 */
function culturefeed_date_control_timestamps_validate(array $element, array &$form_state) {

  $triggering_element = (isset($form_state['triggering_element'])) ? $form_state['triggering_element'] : '';

  if ($triggering_element) {

    $timestamps = $element['#value']['stamps'];
    $triggering_parent = array_pop($triggering_element['#parents']);

    switch ($triggering_parent) {

      case 'timestamps_add_stamp':
        $timestamps[] = array('date' => '');
        $triggering_element['#parents'][] = 'stamps';
        break;

      case 'remove_stamp':
        array_splice($timestamps, array_pop($triggering_element['#parents']), 1);
        break;

      case 'add_end':
        $timestamps[array_pop($triggering_element['#parents'])]['end_time'] = '';
        break;

      case 'remove_end':
        unset($timestamps[array_pop($triggering_element['#parents'])]['end_time']);
        break;

    }

    drupal_array_set_nested_value($form_state['values'], $triggering_element['#parents'], $timestamps);
    drupal_array_set_nested_value($form_state['input'], $triggering_element['#parents'], $timestamps);

  }

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
function culturefeed_date_control_timestamps_value_callback(array &$element, $input, array $form_state) {

  if ($input) {
    return $input;
  }
  else {
    // Default value or an empty stamp.
    return array('stamps' => array(array('date' => '')));
  }

}
