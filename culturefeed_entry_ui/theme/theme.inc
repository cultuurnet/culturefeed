<?php
/**
 * @file
 * Theming functions for entry api ui.
 */

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
