<?php

/**
 * @file
 * Contains the theming functions for the culturefeed date control.
 */

/**
 * Theme the timestamps table.
 */
function theme_culturefeed_date_control_timestamps_table(&$variables) {

  $form = $variables['form'];

  $table = array('rows' => array(), 'attributes' => array('class' => 'date-control-table'));

  foreach (element_children($form) as $element) {

    $cells = array();
    if (isset($form[$element]['timestamps_remove_child'])) {
      $cells[] = drupal_render($form[$element]['timestamps_remove_stamp']);
    }
    $cells[] = drupal_render($form[$element]['date']);
    $cells[] = drupal_render($form[$element]['start_time']);
    if (isset($form[$element]['end_time'])) {
      $cells[] = drupal_render($form[$element]['end_time']);
    }
    if (isset($form[$element]['add_end_' . $element])) {
      $cells[] = drupal_render($form[$element]['add_end_' . $element]);
    }
    if (isset($form[$element]['remove_end_' . $element])) {
      $cells[] = drupal_render($form[$element]['remove_end_' . $element]);
    }
    else {
      $cells[] = '';
    }
    $table['rows'][] = $cells;

  }

  $output = theme('table', $table);
  $output .= drupal_render_children($form);
  return $output;

}
