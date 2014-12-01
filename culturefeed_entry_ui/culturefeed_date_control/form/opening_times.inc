<?php

/**
 * @file
 * Contains the culturefeed date control opening times functions.
 */

/**
 * Sets the culturefeed date control opening times form elements.
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
function culturefeed_date_control_opening_times_process(array $element, array &$form_state, array $form) {

  $day_labels = array(
    'monday' => t('mon'),
    'tuesday' => t('tue'),
    'wednesday' => t('wed'),
    'thursday' => t('thu'),
    'friday' => t('fri'),
    'saturday' => t('sat'),
    'sunday' => t('sun'),
  );

  $element['all_day'] = array(
    '#options' => array('1' => t('Yes'), '0' => t('No')),
    '#title' => t('Open all day ?'),
    '#type' => 'radios',
    '#default_value' => 1,
  );

  $element['days'] = array(
    '#type' => 'container',
    '#theme' => 'culturefeed_date_control_opening_times_table',
    '#states' => array(
      'visible' => array(":input[name=\"" . $element['#name'] . "[all_day]\"]" => array('value' => '0')),
    ),
  );

  foreach ($element['#allowed_days'] as $day) {

    $element['days'][$day] = array(
      '#tree' => TRUE,
      '#type' => 'fieldset',
      '#title' => $day_labels[$day],
    );

    $element['days'][$day][0]['open_from'] = array(
      '#type' => 'textfield',
      '#size' => 12,
      '#default_value' => '',
      '#description' => t('Format') . ': 12:00',
    );

    $element['days'][$day][0]['open_till'] = array(
      '#type' => 'textfield',
      '#size' => 12,
      '#default_value' => '',
      '#description' => t('Format') . ': 12:00',
    );

  }

  $element['#attached'] = array(
    'js' => array(
      drupal_get_path('module', 'culturefeed_date_control') . '/js/culturefeed_date_control_opening_times.js',
    ),
  );

  return $element;

}
