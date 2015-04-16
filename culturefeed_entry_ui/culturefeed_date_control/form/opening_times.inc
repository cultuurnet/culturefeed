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

  $opening_times = $element['#value'];

  $element['all_day'] = array(
    '#options' => array('1' => t('Yes'), '0' => t('No')),
    '#title' => t('Open all day ?'),
    '#type' => 'radios',
    '#default_value' => $opening_times['days'] ? 0 : 1,
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
      '#default_value' => isset($opening_times['days'][$day]) && isset($opening_times['days'][$day][0]['open_from']) ? $opening_times['days'][$day][0]['open_from'] : '',
      '#description' => t('Format') . ': 12:00',
      '#size' => 12,
      '#type' => 'textfield',
    );

    $element['days'][$day][0]['open_till'] = array(
      '#default_value' => isset($opening_times['days'][$day]) && isset($opening_times['days'][$day][0]['open_till']) ? $opening_times['days'][$day][0]['open_till'] : '',
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

  if (!isset($element['#date_control'])) {
    $element['#attached']['css'][] = drupal_get_path('module', 'culturefeed_date_control') . '/css/culturefeed_date_control.css';
  }

  return $element;

}

/**
 * Value callback for the culturefeed date control opening times element.
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
function culturefeed_date_control_opening_times_value_callback(array $element, $input, array $form_state) {

  if ($input) {

    // Clean up.
    $days = array();
    foreach ($input['days'] as $day => $times) {
      if ($times[0]['open_from'] && $times[0]['open_till']) {
        $days[$day] = $input['days'][$day];
      }
    }
    $input['days'] = $days;
    return $input;

  }
  else {

    if (
      isset($element['#default_value']) && isset($element['#default_value']['days']) &&
      $element['#default_value']['days']
    ) {

      // @todo This should be in the entry_ui module.
      /* @var CultureFeed_Cdb_Data_Calendar_Weekscheme $week_scheme */
      $week_scheme = $element['#default_value']['days'];
      $days = array();

      foreach ($element['#allowed_days'] as $day) {

        $day_scheme = $week_scheme->getDay($day);
        if ($day_scheme->isOpen()) {

          /* @var CultureFeed_Cdb_Data_Calendar_OpeningTime[] $opening_times */
          $opening_times = $day_scheme->getOpeningTimes();
          // Strip the seconds.
          $days[$day] = array(
            array(
              'open_from' => substr($opening_times[0]->getOpenFrom(), 0, 5),
              'open_till' => substr($opening_times[0]->getOpenTill(), 0, 5),
            ),
          );

        }

      }

      return array('days' => $days);

    }
    else {
      return array('days' => '');
    }

  }

}
