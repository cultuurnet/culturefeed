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

  if (isset($element['#description'])) {
    $element['date_control']['#description'] = $element['#description'];
  }

  $date_control = $element['#value']['date_control'];

  // Type.
  $element['date_control']['type'] = array(
    '#type' => 'hidden',
    '#value' => $date_control['type'],
  );

  // Hybrid.
  $access = $date_control['type'] == 'hybrid';
  $element['date_control']['hybrid'] = array(
    '#access' => $access,
    '#date_control' => $element,
    '#default_value' => $access && isset($date_control['hybrid']) ? $date_control['hybrid'] : '',
    '#popup_date_format' => 'd-m-Y',
    '#popup_date_increment' => 1,
    '#select_time_format' => 'H:i',
    '#select_time_increment' => 15,
    '#type' => 'culturefeed_date_control_hybrid',
  );

  // Timestamps.
  $access = $date_control['type'] == 'timestamps';
  $element['date_control']['timestamps'] = array(
    '#access' => $access,
    '#date_control' => $element,
    '#default_value' => $access && isset($date_control['timestamps']) ? $date_control['timestamps'] : '',
    '#popup_date_format' => 'd-m-Y',
    '#popup_date_increment' => 1,
    '#select_time_format' => 'H:i',
    '#select_time_increment' => 15,
    '#type' => 'culturefeed_date_control_timestamps',
  );

  // Period.
  $access = $date_control['type'] == 'period';
  $element['date_control']['period'] = array(
    '#access' => $access,
    '#date_control' => $element,
    '#default_value' => $access && isset($date_control['period']) ? $date_control['period'] : '',
    '#popup_date_format' => 'd-m-Y',
    '#popup_date_increment' => 1,
    '#type' => 'culturefeed_date_control_period',
  );

  // Permanent.
  $access = $date_control['type'] == 'permanent';
  $element['date_control']['permanent'] = array(
    '#access' => $access,
    '#ajax' => array(
      'effect' => 'none',
      'path' => 'culturefeed_elements/ajax/' . implode('/', $element['#array_parents']),
      'progress' => array('type' => 'throbber'),
      'wrapper' => $element['#id'] . '-ajax-wrapper',
    ),
    '#value' => $access && isset($date_control['permanent']) ? $date_control['permanent'] : 0,
    '#title' => t('Permanent'),
    '#type' => 'checkbox',
  );

  // Opening times.
  $access = $date_control['type'] == 'period' || $date_control['type'] == 'permanent';
  $element['date_control']['opening_times'] = array(
    '#access' => $access,
    '#allowed_days' => $element['#allowed_days'],
    '#date_control' => $element,
    '#default_value' => $access && isset($date_control['opening_times']) ? $date_control['opening_times'] : '',
    '#type' => 'culturefeed_date_control_opening_times',
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
 * Validate and alter the culturefeed date control form values.
 *
 * This is where the element switching is set, based on values.
 *
 * @param array $element
 *   The element.
 * @param array $form_state
 *   The form state.
 */
function culturefeed_date_control_date_control_validate(array $element, array &$form_state) {

  $triggering_element = (isset($form_state['triggering_element'])) ? $form_state['triggering_element'] : '';
  $triggering_parent = array_pop($triggering_element['#parents']);
  // Use form_state instead of element as individual sub elements have changes
  // that won't be reflected in element.
  $values = drupal_array_get_nested_value($form_state['input'], $element['#parents']);
  $date_control = $values['date_control'];

  if (culturefeed_elements_check_trigger_element($triggering_element, $element)) {

    $type = $date_control['type'];

    // Hybrid.
    if ($date_control['type'] == 'hybrid') {

      // Period.
      if ($triggering_parent == 'end_date') {

        $type = 'period';
        $date_control['period'] = array('start_date' => '');
        if (isset($date_control['hybrid']['start_date'])) {
          $date_control['period']['start_date'] = $date_control['hybrid']['start_date'];
        }
        if (isset($date_control['hybrid']['end_date'])) {
          $date_control['period']['end_date'] = $date_control['hybrid']['end_date'];
        }

      }

      // Permanent.
      if ($triggering_parent == 'permanent') {

        $type = 'permanent';
        $date_control['permanent'] = 1;

      }

      // Timestamps.
      if (
        $triggering_parent == 'add_date' || $triggering_parent == 'hour' || $triggering_parent == 'minute' ||
        ($triggering_parent == 'end_date' && isset($date_control['period']['start_date']) && isset($date_control['period']['end_date']) &&
          $date_control['period']['start_date'] == $date_control['period']['end_date'])
      ) {

        $type = 'timestamps';
        $date_control['timestamps']['stamps'] = array(
          array('date' => ''),
        );
        if (isset($date_control['hybrid']['start_date'])) {
          $date_control['timestamps']['stamps'][0]['date'] = $date_control['hybrid']['start_date'];
        }
        if (isset($date_control['hybrid']['start_time'])) {
          $date_control['timestamps']['stamps'][0]['start_time'] = $date_control['hybrid']['start_time'];
        }
        if (isset($date_control['hybrid']['end_time'])) {
          $date_control['timestamps']['stamps'][0]['end_time'] = $date_control['hybrid']['end_time'];
        }
        if ($triggering_parent == 'add_date') {
          $date_control['timestamps']['stamps'][] = array('date' => '');
        }

      }

    }

    // Period.
    if ($date_control['type'] == 'period') {

      if ($triggering_parent == 'remove_end') {

        $type = 'hybrid';
        // Pass over any set values.
        $date_control['hybrid'] = array(
          'start_date' => isset($date_control['period']['start_date']) ? $date_control['period']['start_date'] : '',
        );

      }

    }

    // Permanent.
    if ($date_control['type'] == 'permanent') {

      if ($triggering_parent == 'permanent') {

        $type = 'hybrid';

      }

    }

    // Timestamps.
    if ($date_control['type'] == 'timestamps') {

      if (
        (($triggering_parent == 'remove_stamp' && !isset($date_control['timestamps']['stamps'][0]['end_time'])) ||
          $triggering_parent == 'remove_end')  && count($date_control['timestamps']['stamps']) == 1
      ) {

        $type = 'hybrid';
        $date_control['hybrid'] = array('start_date' => '');
        if (isset($date_control['timestamps']['stamps'][0]['date'])) {
          $date_control['hybrid']['start_date'] = $date_control['timestamps']['stamps'][0]['date'];
        }
        if (isset($date_control['timestamps']['stamps'][0]['start_time'])) {
          $date_control['hybrid']['start_time'] = $date_control['timestamps']['stamps'][0]['start_time'];
        }

      }

    }

    $date_control['type'] = $type;
    $element['#parents'][] = 'date_control';

    drupal_array_set_nested_value($form_state['input'], $element['#parents'], $date_control);

  }
  else {

    // The one exception.
    if ($date_control['type'] == 'hybrid') {

      $values = drupal_array_get_nested_value($form_state['values'], $element['#parents']);
      $date_control = $values['date_control'];

      $date_control['timestamps']['stamps'][0] = array(
        'date' => $date_control['hybrid']['start_date'],
      );
      if (isset($date_control['hybrid']['start_time'])) {
        $date_control['timestamps']['stamps'][0]['start_time'] = $date_control['hybrid']['start_time'];
      }
      $date_control['type'] = 'timestamps';

      $element['#parents'][] = 'date_control';
      drupal_array_set_nested_value($form_state['values'], $element['#parents'], $date_control);

    }

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
function culturefeed_date_control_date_control_value_callback(array &$element, $input, array $form_state) {

  if ($input) {
    return $input;
  }
  else {

    $value = array(
      'date_control' => array(
        'type' => 'hybrid',
        'hybrid' => array(),
        'opening_times' => array(),
        'period' => array(),
        'timestamps' => array(),
      ),
    );

    if (isset($element['#default_value']) && $element['#default_value']) {

      $date_control = $value['date_control'];
      $default_value = $element['#default_value'];

      // Timestamps / hybrid.
      if (count($default_value['timestamps']) == 1 && !isset($default_value['timestamps'][0]['end_time'])) {

        $date_control['type'] = 'hybrid';
        $date_control['hybrid'] = array(
          'start_date' => $default_value['timestamps'][0]['date'],
        );
        if (isset($default_value['timestamps'][0]['start_time'])) {
          $date_control['hybrid']['start_time'] = $default_value['timestamps'][0]['start_time'];
        }

      }

      // Timestamps.
      elseif (count($default_value['timestamps'])) {

        $date_control['type'] = 'timestamps';
        $date_control['timestamps'] = $default_value['timestamps'];

      }

      // Period.
      elseif ($default_value['period']['start_date'] && $default_value['period']['end_date']) {

        $date_control['type'] = 'period';
        $date_control['period'] = $default_value['period'];

      }

      // Permanent.
      elseif ($default_value['permanent']) {

        $date_control['type'] = 'permanent';
        $date_control['permanent'] = $default_value['permanent'];

      }

      else {
        $date_control['type'] = 'hybrid';
      }

      // Opening times don't set type, they go with period or permanent.
      if ($default_value['opening_times']) {

        $date_control['opening_times'] = $default_value['opening_times'];

      }

      $value['date_control'] = $date_control;

    }

    return $value;

  }

}
