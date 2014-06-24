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
