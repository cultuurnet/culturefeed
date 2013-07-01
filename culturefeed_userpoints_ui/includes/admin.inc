<?php
/**
 * Settings file for culturefeed userpoints ui.
 */

function culturefeed_userpoints_ui_settings_form($form, $form_state = array()) {

  $form['culturefeed_userpoints_ui_exchange_ratio'] = array(
    '#type' => 'textfield',
    '#title' => t('Exchange ratio'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_ratio'),
    '#description' => t('Exchange ratio to recalculate points for this site. E.g. "0.03 points is worth 1 point on this site."'),
  );

  $form['culturefeed_userpoints_ui_exchange_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Destination email when an exchange has been done'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_email', ''),
  );

  return system_settings_form($form);

}