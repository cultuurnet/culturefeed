<?php
/**
 * @file
 * Theming functions for entry api ui.
 */

/**
 * Returns html for an edit tags form rendered as table.
 */
function theme_culturefeed_entry_ui_edit_tag_objects_form_table(&$vars) {

  $form = $vars['form'];
  $attributes = (isset($form['#attributes'])) ? $form['#attributes'] : array();
  $attributes['class'][] = 'table-no-striping';

  $header = array();
  $rows = array();

  foreach (element_children($form) as $row) {
    $rows[] = array(
      'data' => array(
        array(
          'data' => drupal_render($form[$row]['value']),
          'class' => 'edit-tag-objects-form-cell',
        ),
        array(
          'data' => drupal_render($form[$row]['visible']),
          'class' => 'edit-tag-objects-form-cell',
        ),
        array(
          'data' => drupal_render($form[$row]['remove']),
          'class' => 'edit-tag-objects-form-cell',
        ),
      ),
      'no_striping' => TRUE,
    );
  }

  $output = theme('table', array('attributes' => $attributes, 'header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($form);
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
