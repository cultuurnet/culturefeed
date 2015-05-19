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
function culturefeed_date_control_timestamps_pre_render(array $element) {

  $timestamps = $element['#value']['stamps'];

  // Remove button is not required if only one row.
  if (count($timestamps) < 2) {
    $element['stamps'][0]['remove_stamp']['#access'] = FALSE;
  }

  foreach ($timestamps as $key => $timestamp) {

    if (
      isset($timestamps[$key]['end_time'])
      && !is_null($timestamps[$key]['end_time']['hour'])
      && !is_null($timestamps[$key]['end_time']['minute'])
    ) {
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
 * Sets the culturefeed date control timestamps form elements.
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

  $element_id = $element['#id'];
  $array_parents = $element['#array_parents'];

  $element['#prefix'] = "<div id=\"" . $element_id . "-ajax-wrapper\">";
  $element['#suffix'] = "</div>";

  if (isset($element['#date_control'])) {
    $element_id = $element['#date_control']['#id'];
    $array_parents = $element['#date_control']['#array_parents'];
  }

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

  $timestamps = $element['#value'];

  foreach ($timestamps['stamps'] as $key => $timestamp) {

    $element['stamps'][$key]['remove_stamp'] = array(
      '#limit_validation_errors' => array(),
      '#value' => t('×'),
      '#name' => 'remove_stamp_' . $key,
    ) + $ajax_button;

    $date = isset($timestamp['date']) && $timestamp['date'] ? culturefeed_date_control_implode_date($element, $timestamp['date']) : '';
    $element['stamps'][$key]['date'] = array(
      '#date_format' => $element['#popup_date_format'],
      '#date_increment' => $element['#popup_date_increment'],
      '#date_label_position' => 'none',
      '#default_value' => $date,
      '#icon' => TRUE,
      '#required' => TRUE,
      '#title' => t('Date'),
      '#type' => 'date_popup',
    );

    $time = isset($timestamp['start_time']) && $timestamp['start_time'] ? culturefeed_date_control_implode_time($timestamp['start_time'], $date) : '';
    $element['stamps'][$key]['start_time'] = array(
      '#attributes' => array('class' => array('time-select-complete-minutes')),
      '#date_format' => $element['#select_time_format'],
      '#date_increment' => $element['#select_time_increment'],
      '#default_value' => $time,
      '#title' => '',
      '#type' => 'date_select',
    );

    $time = isset($timestamp['end_time']) && $timestamp['end_time'] ? culturefeed_date_control_implode_time($timestamp['end_time'], $date) : '';
    $element['stamps'][$key]['end_time'] = array(
      '#access' => TRUE,
      '#attributes' => array('class' => array('time-select-complete-minutes')),
      '#date_format' => $element['#select_time_format'],
      '#date_increment' => $element['#select_time_increment'],
      '#default_value' => $time,
      '#title' => '',
      '#type' => 'date_select',
    );

    $element['stamps'][$key]['remove_end'] = array(
      '#name' => 'remove_end_' . $key,
      '#value' => t('- remove end time'),
    ) + $ajax_button;

    $element['stamps'][$key]['add_end'] = array(
      '#name' => 'add_end_' . $key,
      '#value' => t('+ add end time'),
    ) + $ajax_button;

  }

  $element['timestamps_add_stamp'] = array(
    '#name' => 'add_stamp',
    '#value' => t('+ add date'),
  ) + $ajax_button;

  $element['#attached'] = array(
    'js' => array(
      drupal_get_path('module', 'culturefeed_date_control') . '/js/culturefeed_date_control_time_select.js',
    ),
  );

  if (!isset($element['#date_control'])) {
    $element['#attached']['css'] = array(
      drupal_get_path('module', 'culturefeed_date_control') . '/css/culturefeed_date_control.css',
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
function culturefeed_date_control_timestamps_validate(array $element, array &$form_state) {

  $triggering_element = (isset($form_state['triggering_element'])) ? $form_state['triggering_element'] : '';
  $triggering_parent = array_pop($triggering_element['#parents']);
  $timestamps = $element['#value'];

  if (culturefeed_elements_check_trigger_element($triggering_element, $element)) {

    switch ($triggering_parent) {

      case 'timestamps_add_stamp':
        $timestamps['stamps'][] = array('date' => array('date' => ''));
        break;

      case 'remove_stamp':
        $index = array_pop($triggering_element['#parents']);
        array_splice($timestamps['stamps'], $index, 1);
        break;

      case 'add_end':
        $index = array_pop($triggering_element['#parents']);
        $timestamps['stamps'][$index]['end_time'] = array('hour' => '', 'minute' => '');
        break;

      case 'remove_end':
        $index = array_pop($triggering_element['#parents']);
        unset($timestamps['stamps'][$index]['end_time']);
        break;

    }

    drupal_array_set_nested_value($form_state['input'], $element['#parents'], $timestamps);

  }

}

/**
 * Value callback for the culturefeed date control timestamps element.
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
function culturefeed_date_control_timestamps_value_callback(array $element, $input, array $form_state) {

  if ($input) {
    return $input;
  }
  else {

    $value = array(
      'stamps' => array(
        array('date' => '', 'start_time' => '', 'end_time' => ''),
      ),
    );

    if (isset($element['#default_value']) && $element['#default_value']) {

      $stamps = array();
      foreach ($element['#default_value'] as $default_value) {

        if (isset($default_value['date']) && $default_value['date']) {

          $stamp = array();
          $stamp['date'] = culturefeed_date_control_explode_date($element, $default_value['date']);
          if (isset($default_value['start_time']) && $default_value['start_time']) {
            $stamp['start_time'] = culturefeed_date_control_explode_time($element, $default_value['start_time'], $default_value['date']);
          }
          if (isset($default_value['end_time']) && $default_value['end_time']) {
            $stamp['end_time'] = culturefeed_date_control_explode_time($element, $default_value['end_time'], $default_value['date']);
          }
          $stamps[] = $stamp;

        }

      }

      if (count($stamps)) {
        $value['stamps'] = $stamps;
      }

    }

    return $value;

  }

}
