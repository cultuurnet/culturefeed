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

  $cdb_version = variable_get('culturefeed_cdb_version', CULTUREFEED_CDB_DEFAULT_VERSION);

  $form['culturefeed_entry_ui_preferred_language'] = array(
    '#title' => t('Preferred language'),
    '#type' => 'select',
    '#empty_option' =>  t('Use the preferred language of the UiTID user, with the Drupal UI language as fallback'),
    '#options' => $language_options,
    '#default_value' => variable_get('culturefeed_entry_ui_preferred_language', ''),
  );

  if (version_compare($cdb_version, '3.3', '>=')) {

    $form['culturefeed_entry_ui_custom_tags'] = array(
      '#default_value' => variable_get('culturefeed_entry_ui_custom_tags'),
      '#description' => t('Custom tags specific for this site.'),
      '#title' => t('Custom options on the /tags page'),
      '#type' => 'culturefeed_tags_element',
      '#visible_default' => FALSE,
    );

  }
  else {

    $form['culturefeed_entry_ui_custom_tags'] = array(
      '#default_value' => variable_get('culturefeed_entry_ui_custom_tags'),
      '#description' => t('Custom tags specific for this site. Use a new line for each tag.'),
      '#title' => t('Custom options on the /tags page'),
      '#type' => 'textarea',
    );

  }

  if (version_compare($cdb_version, '3.3', '>=')) {

    $form['culturefeed_entry_ui_custom_entry_api_tags'] = array(
      '#default_value' => variable_get('culturefeed_entry_ui_custom_entry_api_tags'),
      '#description' => t('Tags pushed under the hood to Entry API.'),
      '#title' => t('Custom tags to push to entry API'),
      '#type' => 'culturefeed_tags_element',
      '#visible_default' => FALSE,
    );

  }
  else {

    $form['culturefeed_entry_ui_custom_entry_api_tags'] = array(
      '#default_value' => variable_get('culturefeed_entry_ui_custom_entry_api_tags'),
      '#description' => t('Tags pushed under the hood to Entry API. Use a new line for each tag.'),
      '#title' => t('Custom tags to push to entry API'),
      '#type' => 'textarea',
    );

  }

  if (version_compare($cdb_version, '3.3', '>=')) {

    $form['culturefeed_entry_ui_custom_collaboration_tags'] = array(
      '#default_value' => variable_get('culturefeed_entry_ui_custom_collaboration_tags'),
      '#description' => t('Tags used for the collaboration tab.'),
      '#title' => t('Custom tags to use as collaboration data'),
      '#type' => 'culturefeed_tags_element',
      '#visible_default' => FALSE,
    );

  }

  $form['culturefeed_entry_ui_default_country'] = array(
    '#type' => 'select',
    '#options' => country_get_list(),
    '#title' => t('Default country'),
    '#default_value' => variable_get('culturefeed_entry_ui_default_country') ? variable_get('culturefeed_entry_ui_default_country') : 'BE',
    '#description' => t('Select the default country for the entry form.'),
  );

  $form['culturefeed_entry_ui_sync_max_duration'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum duration for syncing an event with the search api'),
    '#default_value' => variable_get('culturefeed_entry_ui_sync_max_duration', CULTUREFEED_ENTRY_UI_SYNC_MAX_DURATION_DEFAULT),
    '#description' => t('After saving an event a batch wil run until the event is synced with the search api. When the maximum duration is reached, the batch will terminate with a warning message if the event was not synced.'),
  );

  return system_settings_form($form);

}
