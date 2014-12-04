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

  /* @var CultureFeed_Cdb_Data_Calendar_Weekscheme $week_scheme */
  // @todo This should be in the entry_ui module.
  $week_scheme = !count($form_state['input']) && isset($element['#default_value']['days']) ? $element['#default_value']['days'] : '';

  $element['all_day'] = array(
    '#options' => array('1' => t('Yes'), '0' => t('No')),
    '#title' => t('Open all day ?'),
    '#type' => 'radios',
    '#default_value' => $week_scheme ? 0 : 1,
  );

  $element['days'] = array(
    '#type' => 'container',
    '#theme' => 'culturefeed_date_control_opening_times_table',
    '#states' => array(
      'visible' => array(":input[name=\"" . $element['#name'] . "[all_day]\"]" => array('value' => '0')),
    ),
  );

  foreach ($element['#allowed_days'] as $day) {

    $default_value = '';
    if ($week_scheme) {

      $day_scheme = $week_scheme->getDay($day);
      if ($day_scheme->isOpen()) {
        $opening_times = $day_scheme->getOpeningTimes();
        // Strip the seconds.
        $default_value = array(
          'open_from' => substr($opening_times[0]->getOpenFrom(), 0, 5),
          'open_till' => substr($opening_times[0]->getOpenTill(), 0, 5),
        );
      }

    }

    $element['days'][$day] = array(
      '#tree' => TRUE,
      '#type' => 'fieldset',
      '#title' => $day_labels[$day],
    );

    $element['days'][$day][0]['open_from'] = array(
      '#default_value' => $default_value && isset($default_value['open_from']) ? $default_value['open_from'] : '',
      '#description' => t('Format') . ': 12:00',
      '#size' => 12,
      '#type' => 'textfield',
    );

    $element['days'][$day][0]['open_till'] = array(
      '#default_value' => $default_value && isset($default_value['open_till']) ? $default_value['open_till'] : '',
      '#description' => t('Format') . ': 12:00',
      '#size' => 12,
      '#type' => 'textfield',
    );

  }

  $element['#attached'] = array(
    'css' => array(
      drupal_get_path('module', 'culturefeed_date_control') . '/css/culturefeed_date_control_opening_times.css',
    ),
    'js' => array(
      drupal_get_path('module', 'culturefeed_date_control') . '/js/culturefeed_date_control_opening_times.js',
    ),
  );

  return $element;

}
