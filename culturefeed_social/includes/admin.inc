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

  $form['culturefeed_social_activities_page_intro'] = array(
    '#type' => 'textarea',
    '#title' => t('Activities page intro'),
    '#default_value' => variable_get('culturefeed_social_activities_page_intro'),
  );

  $form['culturefeed_social_notifications_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable notifications'),
    '#default_value' => variable_get('culturefeed_social_notifications_enabled', FALSE),
  );

  $form['culturefeed_social_report_destination_uid'] = array(
    '#type' => 'textfield',
    '#title' => t('Report as abuse messages destination uid (Drupal)'),
    '#default_value' => variable_get('culturefeed_social_report_destination_uid'),
    '#description' => t('Report as abuse messages will be sent to this account.
    Only if no destination page was available. Otherwise the message will be sent to all the admins of the page'
    ),
  );

  return system_settings_form($form);

}
