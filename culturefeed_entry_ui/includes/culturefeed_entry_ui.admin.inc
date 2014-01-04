<?php
/**
 * @file
 * Settings file for culturefeed entry ui.
 */

/**
 * Callback for the CultureFeed Entry UI settings form.
 */
function culturefeed_entry_ui_settings_form($form, &$form_state) {

  $language_options = array();
  foreach (language_list() as $language) {
    if ($language->enabled !== '1') {
      continue;
    }
    $language_options[$language->language] = $language->name;
  }

  $form['culturefeed_entry_ui_preferred_language'] = array(
    '#title' => t('Preferred language'),
    '#type' => 'select',
    '#empty_option' =>  t('Use the preferred language of the UiTID user, with the Drupal UI language as fallback'),
    '#options' => $language_options,
    '#default_value' => variable_get('culturefeed_entry_ui_preferred_language', ''),
  );

  $form['culturefeed_entry_ui_custom_tags'] = array(
    '#type' => 'textarea',
    '#title' => t('Custom tags'),
    '#default_value' => variable_get('culturefeed_entry_ui_custom_tags'),
    '#description' => t('Custom tags specific for this site. Use a new line for each tag.'),
  );

  return system_settings_form($form);

}
