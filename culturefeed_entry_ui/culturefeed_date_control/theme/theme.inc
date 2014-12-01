<?php

/**
 * @file
 * Contains the theming functions for the culturefeed date control.
 */

/**
 * Theme the timestamps table.
 *
 * @param array $variables
 *   The variables.
 *
 * @return string
 *   The output.
 */
function theme_culturefeed_date_control_timestamps_table(array &$variables) {

  $form = $variables['form'];

  $table = array('rows' => array(), 'attributes' => array('class' => 'date-control-timestamps-table'));

  foreach (element_children($form['stamps']) as $element) {

    if ($element !== 'timestamps_add_stamp') {

      $cells = array();
      if ($form['stamps'][$element]['remove_stamp']['#access']) {
        $cells[] = drupal_render($form['stamps'][$element]['remove_stamp']);
      }
      $cells[] = drupal_render($form['stamps'][$element]['date']);
      $cells[] = drupal_render($form['stamps'][$element]['start_time']);
      if ($form['stamps'][$element]['end_time']['#access']) {
        $cells[] = drupal_render($form['stamps'][$element]['end_time']);
      }
      if ($form['stamps'][$element]['add_end']['#access']) {
        $cells[] = drupal_render($form['stamps'][$element]['add_end']);
      }
      if ($form['stamps'][$element]['remove_end']['#access']) {
        $cells[] = drupal_render($form['stamps'][$element]['remove_end']);
      }
      elseif ($form['#cell_count'] == 5) {
        $cells[] = '';
      }
      $table['rows'][] = $cells;

    }

  }

  $output = theme('table', $table);
  $output .= drupal_render_children($form);
  return $output;

}
