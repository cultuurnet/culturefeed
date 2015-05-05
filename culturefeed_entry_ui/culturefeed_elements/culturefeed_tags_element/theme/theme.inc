<?php

/**
 * @file
 * Theming functions for culturefeed tags element.
 */

/**
 * Returns html for an edit tags form rendered as table.
 */
function theme_culturefeed_tags_element_tags_element_form_table(&$vars) {

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
          'class' => 'tags-element-form-cell',
        ),
        array(
          'data' => drupal_render($form[$row]['hidden']),
          'class' => 'tags-element-form-cell',
        ),
        array(
          'data' => drupal_render($form[$row]['remove']),
          'class' => 'tags-element-form-cell',
        ),
      ),
      'no_striping' => TRUE,
    );
  }

  $output = theme('table', array(
    'attributes' => $attributes,
    'header' => $header,
    'rows' => $rows,
  ));
  $output .= drupal_render_children($form);
  return $output;

}
