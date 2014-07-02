<?php
/**
 * @file
 * Theming functions for entry api ui.
 */

/**
 * Theme the opening times table.
 */
function theme_culturefeed_entry_ui_opening_times_table($variables) {

  $container = $variables['element'];

  $total_opening_times = 1;
  $rows = array();
  foreach (element_children($container) as $key) {

    $day_element = $container[$key];
    $row = array($day_element['#title']);
    $opening_times = element_children($day_element);
    $total_opening_times = count($opening_times);

    foreach ($opening_times as $opening_time) {
      $row[] = render($day_element[$opening_time]['open_from']);
      $row[] = render($day_element[$opening_time]['open_till']);
    }

    $rows[] = $row;

  }

  $table_variables = array(
    'header' => array(
      t('open'),
    ),
    'rows' => $rows,
  );

  while ($total_opening_times > 0) {
    $table_variables['header'][] = t('from');
    $table_variables['header'][] = t('to');
    $total_opening_times--;
  }

  return theme('table', $table_variables);

}

/**
 * Theme the datetimes table.
 */
function theme_culturefeed_entry_ui_when_datetimes_table(&$variables) {

  $form = $variables['form'];
  $table = array('rows' => array(), 'attributes' => array('class' => 'when-datetimes-table'));

  // First loop to check how many cells we need.
  $cell_dates_remove_child = FALSE;
  $cell_begin_date = FALSE;
  $cell_begin_hour = FALSE;
  $cell_datetimes_add_end = FALSE;
  $cell_end_date = FALSE;
  $cell_end_hour = FALSE;
  $cell_datetimes_remove_end = FALSE;
  foreach (element_children($form) as $element) {
    if (isset($form[$element]['begin']['datetimes_remove_child'])) {
      $cell_dates_remove_child = TRUE;
    }
    if (isset($form[$element]['begin']['date'])) {
      $cell_begin_date = TRUE;
    }
    if (isset($form[$element]['begin']['hour'])) {
      $cell_begin_hour = TRUE;
    }
    if (isset($form[$element]['begin']['datetimes_add_end'])) {
      $cell_datetimes_add_end = TRUE;
    }
    if (isset($form[$element]['end']['date'])) {
      $cell_end_date = TRUE;
    }
    if (isset($form[$element]['end']['hour'])) {
      $cell_end_hour = TRUE;
    }
    if (isset($form[$element]['end']['datetimes_remove_end'])) {
      $cell_datetimes_remove_end = TRUE;
    }
  }

  // Second loop to set the cells.
  foreach (element_children($form) as $element) {
    if ($element !== 'datetimes_add_child') {

      $begin = isset($form[$element]['begin']) ? $form[$element]['begin'] : array();
      $end = isset($form[$element]['end']) ? $form[$element]['end'] : array();

      $cells = array();
      if ($cell_dates_remove_child) {
        $cells[] = isset($begin['datetimes_remove_child']) ? drupal_render($begin['datetimes_remove_child']) : '';
      }
      if ($cell_begin_date) {
        $cells[] = isset($begin['date']) ? drupal_render($begin['date']) : '';
      }
      if ($cell_begin_hour) {
        $cells[] = isset($begin['hour']) ? drupal_render($begin['hour']) : '';
      }
      if ($cell_datetimes_add_end) {
        $cells[] = isset($begin['datetimes_add_end']) ? drupal_render($begin['datetimes_add_end']) : '';
      }
      if ($cell_end_date) {
        $add = $cells;
        $add[] = array();
        $cells = array();
        $cells[] = isset($end['date']) ? drupal_render($end['date']) : '';
      }
      if ($cell_end_hour) {
        $cells[] = isset($end['hour']) ? drupal_render($end['hour']) : '';
      }
      if ($cell_datetimes_remove_end) {
        $cells[] = isset($end['datetimes_remove_end']) ? drupal_render($end['datetimes_remove_end']) : '';
      }

      if (isset($add)) {
        $table['rows'][] = $add;
      }
      $table['rows'][] = $cells;

    }
  }

  $output = theme('table', $table);
  $output .= drupal_render($form['datetimes_add_child']);
  return $output;

}

/**
 * Preprocess function for the culturefeed_entry_ui_event_form.
 */
function template_preprocess_culturefeed_entry_ui_event_form(&$variables) {
  // Add the theme form elements as suffixes to the what options.
  $variables['form']['basic']['what']['0.100.0.0.0']['0.50.4.0.0']['#suffix'] = render($variables['form']['basic']['what']['themes']['concerts']);
  $variables['form']['basic']['what']['0.100.0.0.0']['0.55.0.0.0']['#suffix'] = render($variables['form']['basic']['what']['themes']['theatre']);
  $variables['form']['basic']['what']['0.100.0.0.0']['0.54.0.0.0']['#suffix'] = render($variables['form']['basic']['what']['themes']['dance']);
  $variables['form']['basic']['what']['0.100.0.0.0']['0.50.6.0.0']['#suffix'] = render($variables['form']['basic']['what']['themes']['film']);
  $variables['form']['basic']['what']['0.100.0.0.0']['0.19.0.0.0']['#suffix'] = render($variables['form']['basic']['what']['themes']['sport']);
  // Don't show any markup anymore for the themes fieldset.
  $variables['form']['basic']['what']['themes']['#printed'] = TRUE;
}
