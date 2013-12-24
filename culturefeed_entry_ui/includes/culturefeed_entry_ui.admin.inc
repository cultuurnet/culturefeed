<?php
/**
 * Settings file for culturefeed entry ui.
 */

function culturefeed_entry_ui_settings_form($form, $form_state = array()) {

  $form['culturefeed_entry_ui_custom_tags'] = array(
    '#type' => 'textarea',
    '#title' => t('Custom tags'),
    '#default_value' => variable_get('culturefeed_entry_ui_custom_tags'),
    '#description' => t('Custom tags specific for this site. Use a new line for each tag.'),
  );

  return system_settings_form($form);

}