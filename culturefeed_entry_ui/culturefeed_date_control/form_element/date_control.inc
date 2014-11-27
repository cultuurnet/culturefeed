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
 *
 * @return array
 *   The processed element.
 */
function culturefeed_date_control_process_element(array $element, array &$form_state) {

  // Values.
  $element['#tree'] = FALSE;
  $values = isset($form_state['values']['date_control']) ? $form_state['values']['date_control'] : $element['#default_value'];
  $triggering_parent = isset($form_state['triggering_element']['#parents']) ? array_pop($form_state['triggering_element']['#parents']) : '';

  $type = culturefeed_date_control_get_type($values);

  $element['container'] = array(
    '#type' => 'container',
    '#prefix' => "<div id=\"date-control-ajax-wrapper\">",
    '#suffix' => "</div>",
  );

  $element['container']['date_control'] = array(
    '#type' => 'fieldset',
    '#title' => $element['#title'],
    '#tree' => TRUE,
  );

  // Timestamps.
  if ($type == 'timestamps') {
    culturefeed_date_control_get_timestamps(
      $element['container']['date_control'],
      $form_state,
      $triggering_parent,
      isset($values['timestamps']) ? $values['timestamps'] : array()
    );
  }

  $element['#attached'] = array(
    'css' => array(
      drupal_get_path('module', 'culturefeed_date_control') . '/css/culturefeed_date_control.css',
    ),
  );

  return $element;
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

/**
 * Sets the date control timestamps form elements.
 *
 * @param array $element
 *   The element.
 * @param array $form_state
 *   The form state.
 * @param string $triggering_parent
 *   The triggering parent.
 * @param array $timestamps
 *   The timestamps.
 */
function culturefeed_date_control_get_timestamps(array &$element, array &$form_state, $triggering_parent, array $timestamps) {

  if (empty($form_state['timestamp_count'])) {
    $form_state['timestamp_count'] = 1;
  }

  $element['timestamps'] = array(
    '#type' => 'container',
    '#theme' => 'culturefeed_date_control_timestamps_table',
  );

  for ($i = 0; $i < $form_state['timestamp_count']; $i++) {

    if ($form_state['timestamp_count'] > 1) {
      $element['timestamps'][$i]['timestamps_remove_stamp'] = array(
        '#type' => 'submit',
        '#value' => t('×'),
        '#name' => 'remove_stamp_' . $i,
        '#submit' => array('culturefeed_date_control_ajax_submit'),
        '#attributes' => array('class' => array('add-more-link btn-link'), 'title' => 'remove date'),
        '#ajax' => array(
          'callback' => 'culturefeed_date_control_ajax_callback',
          'wrapper' => 'date-control-ajax-wrapper',
          'effect' => 'none',
          'progress' => array('type' => 'throbber'),
        ),
      );
    }

    $element['timestamps'][$i]['date'] = array(
      '#type' => 'date_popup',
      '#default_value' => isset($timestamps[$i]['date']) ? $timestamps[$i]['date'] : '',
      '#title' => t('Date'),
      '#date_label_position' => 'none',
      '#date_format' => 'd-m-Y',
      '#icon' => TRUE,
    );

    $element['timestamps'][$i]['start_time'] = array(
      '#type' => 'date_select',
      '#default_value' => isset($timestamps[$i]['start_time']) ? $timestamps[$i]['date'] . ' ' . $timestamps[$i]['start_time'] : '',
      '#title' => '',
      '#date_format' => 'H:i',
      '#date_increment' => 30,
    );

    $element['timestamps'][$i]['end_time'] = array(
      '#type' => 'date_select',
      '#default_value' => isset($timestamps[$i]['end_time']) ? $timestamps[$i]['date'] . ' ' . $timestamps[$i]['end_time'] : '',
      '#title' => '',
      '#date_format' => 'H:i',
      '#date_increment' => 30,
      '#access' => isset($timestamps[$i]['end_time']),
    );

    $element['timestamps'][$i]['remove_end_' . $i] = array(
      '#type' => 'submit',
      '#value' => t('- remove end time'),
      '#submit' => array('culturefeed_date_control_ajax_submit'),
      '#attributes' => array('class' => array('add-more-link btn-link')),
      '#ajax' => array(
        'callback' => 'culturefeed_date_control_ajax_callback',
        'wrapper' => 'date-control-ajax-wrapper',
        'effect' => 'none',
        'progress' => array('type' => 'throbber'),
      ),
      '#access' => isset($timestamps[$i]['end_time']),
    );

    $element['timestamps'][$i]['add_end_' . $i] = array(
      '#type' => 'submit',
      '#value' => t('+ add end time'),
      '#submit' => array('culturefeed_date_control_ajax_submit'),
      '#attributes' => array('class' => array('add-more-link btn-link')),
      '#ajax' => array(
        'callback' => 'culturefeed_date_control_ajax_callback',
        'wrapper' => 'date-control-ajax-wrapper',
        'effect' => 'none',
        'progress' => array('type' => 'throbber'),
      ),
      '#access' => !isset($timestamps[$i]['end_time']),
    );

  }

  $element['timestamps_add_stamp'] = array(
    '#type' => 'submit',
    '#value' => t('+ add date'),
    '#submit' => array('culturefeed_date_control_ajax_submit'),
    '#attributes' => array('class' => array('add-more-link btn-link')),
    '#ajax' => array(
      'callback' => 'culturefeed_date_control_ajax_callback',
      'wrapper' => 'date-control-ajax-wrapper',
      'effect' => 'none',
      'progress' => array('type' => 'throbber'),
    ),
  );

}

/**
 * Submit handler for the ajax buttons.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 */
function culturefeed_date_control_ajax_submit(array $form, array &$form_state) {

  $triggering_parent = array_pop($form_state['triggering_element']['#parents']);
  $timestamps = $form_state['values']['date_control']['timestamps'];

  switch ($triggering_parent) {
    case 'add_stamp':
      $form_state['timestamp_count']++;
      break;

    case 'remove_stamp':
      array_splice($timestamps, array_pop($form_state['triggering_element']['#parents']), 1);
      $form_state['timestamp_count']--;
      break;

    case strpos($triggering_parent, 'add_end_') === 0:
      $timestamps[array_pop($form_state['triggering_element']['#parents'])]['end_time'] = '';
      break;

    case strpos($triggering_parent, 'remove_end_') === 0:
      unset($timestamps[array_pop($form_state['triggering_element']['#parents'])]['end_time']);
      break;
  }

  drupal_array_set_nested_value($form_state['input'], array('date_control', 'timestamps'), $timestamps, TRUE);
  drupal_array_set_nested_value($form_state['values'], array('date_control', 'timestamps'), $timestamps, TRUE);

  $form_state['rebuild'] = TRUE;

}
