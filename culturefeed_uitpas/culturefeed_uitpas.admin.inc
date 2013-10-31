<?php
/**
 * @file
 * Administrative page callbacks.
 */

/**
 * Form builder for the UiTPAS settings form.
 */
function culturefeed_uitpas_settings_form($form, &$form_state) {

  $form['culturefeed_uitpas_cardsystem_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Card system ID'),
    '#default_value' => variable_get('culturefeed_uitpas_cardsystem_id'),
  );

  return system_settings_form($form);
}
