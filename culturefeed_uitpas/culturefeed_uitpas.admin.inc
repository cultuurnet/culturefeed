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

  $form['culturefeed_uitpas_remind_user_without_uitpas'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show message to users without UiTPAS'),
    '#description' => t('When checked a message is shown on each page when the UiTiD user does not have a UiTPAS activated, to remind him/her to activate their card.'),
    '#default_value' => variable_get('culturefeed_uitpas_remind_user_without_uitpas', 1),
  );

  $form['culturefeed_uitpas_remind_user_without_uitpas_text'] = array(
    '#type' => 'textarea',
    '#title' => t('The message to show to users without UiTPAS'),
    '#default_value' => variable_get('culturefeed_uitpas_remind_user_without_uitpas_text', t('Je bent succesvol aangemeld, maar je UiTPAS werd nog niet online geregistreerd. Vul <a href="@register_url">de nodige gegevens</a> in om jouw persoonlijke voordelen en puntensaldo te bekijken.')),
    '#description' => check_plain(t('You can use @register_url to insert the registration URL in the message, for example: "Go <a href="@register_url">register your UiTPAS</a> now!".')),
    '#states' => array(
      'visible' => array(
        ':input[name="culturefeed_uitpas_remind_user_without_uitpas"]' => array('checked' => TRUE),
      ),
    ),
  );

  // Advantages promotions.
  form_load_include($form_state, 'inc', 'culturefeed_uitpas', 'includes/advantages_promotions');
  culturefeed_uitpas_advantages_promotions_settings_get($form);

  // Locations.
  $form['locations'] = array(
    '#type' => 'fieldset',
    '#title' => t('UiTPAS locations'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $locations = variable_get('culturefeed_uitpas_locations');
  if (!$locations) {
    $locations = array();
  }

  $form['locations']['culturefeed_uitpas_locations'] = array(
    '#type' => 'textarea',
    '#title' => t('Locations'),
    '#value_callback' => 'culturefeed_uitpas_locations_value_callback',
    '#default_value' => implode("\r\n", $locations),
  );

  // Profile actions.
  form_load_include($form_state, 'inc', 'culturefeed_uitpas', 'includes/profile_actions');
  culturefeed_uitpas_profile_actions_settings_get($form);

  // Profile advantages.
  form_load_include($form_state, 'inc', 'culturefeed_uitpas', 'includes/profile_advantages');
  culturefeed_uitpas_profile_advantages_settings_get($form);

  // Profile details.
  form_load_include($form_state, 'inc', 'culturefeed_uitpas', 'includes/profile_details');
  culturefeed_uitpas_profile_details_settings_get($form);

  // Profile notifications.
  form_load_include($form_state, 'inc', 'culturefeed_uitpas', 'includes/profile_notifications');
  culturefeed_uitpas_profile_notifications_settings_get($form);

  // Register where.
  form_load_include($form_state, 'inc', 'culturefeed_uitpas', 'includes/register_where');
  culturefeed_uitpas_register_where_settings_get($form);

  // Registration/validation page & block.
  form_load_include($form_state, 'inc', 'culturefeed_uitpas', 'includes/user_register');
  $form['user_register'] = array(
    '#type' => 'fieldset',
    '#title' => t('UiTPAS registration page & block settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  culturefeed_uitpas_user_register_settings_get($form['user_register'], TRUE);

  // Registration confirmation.
  form_load_include($form_state, 'inc', 'culturefeed_uitpas', 'includes/registration_confirmation');
  culturefeed_uitpas_registration_confirmation_settings_get($form);

  return system_settings_form($form);

}

/**
 * Value callback for culturefeed_uitpas_settings_form locations.
 */
function culturefeed_uitpas_locations_value_callback($element, $input = FALSE, $form_state = array()) {

  if ($input) {

    $input = str_replace(array("\r\n", "\r"), "\n", $input);
    return explode("\n", $input);

  }

}
