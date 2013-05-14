<?php
/**
 * @file
 * Administration forms for culturefeed social.
 */

/**
 * Administration settings for culturefeed social.
 */
function culturefeed_social_admin_settings_form() {

  $filter_options = culturefeed_social_get_activity_filter_options();
  $options = array();
  foreach ($filter_options as $activity_id => $activity_config) {
    $options[$activity_id] = $activity_config->label;
  }

  $form = array();

  $form['culturefeed_social_activity_filters'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Visible filters on activity lists'),
    '#options' => $options,
    '#default_value' => variable_get('culturefeed_social_activity_filters', array()),
  );

  return system_settings_form($form);

}