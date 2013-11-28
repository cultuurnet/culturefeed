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

  // Advantages promotions.
  form_load_include($form_state, 'inc', 'culturefeed_uitpas', 'includes/advantages_promotions');
  culturefeed_uitpas_advantages_promotions_settings_get($form);

  // Profile details.
  form_load_include($form_state, 'inc', 'culturefeed_uitpas', 'includes/profile_details');
  culturefeed_uitpas_profile_details_settings_get($form);

  return system_settings_form($form);

}
