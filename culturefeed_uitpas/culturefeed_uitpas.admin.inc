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
    '#title' => t('Your cardsystem id'),
    '#default_value' => variable_get('culturefeed_uitpas_cardsystem_id', 3),
    '#description' => t('Use the id of your cardsystem. Leave empty when your site is using multiple cardsystems.'),
  );

  $card_system_ids = array();
  $cf = DrupalCultureFeed::getConsumerInstance();
  $card_systems = $cf->uitpas()->getCardSystems('permanent');
  foreach ($card_systems as $card_system) {
    $card_system_ids[$card_system->id] = $card_system->name;
  }

  $form['culturefeed_uitpas_cardsystem_ids'] = array(
    '#default_value' => variable_get('culturefeed_uitpas_cardsystem_ids', array()),
    '#options' => $card_system_ids,
    '#title' => t('Or select multiple cardsystems'),
    '#type' => 'checkboxes',
    '#description' => t('Only use this if your site is intended for passholders of multiple UiTPAS regions'),
  );

  $form['culturefeed_uitpas_card_name'] = array(
    '#default_value' => variable_get('culturefeed_uitpas_card_name', 'UiTPAS'),
    '#description' => t('The card name, used as title in profile sections block.'),
    '#title' => t('Card name'),
    '#type' => 'textfield',
  );

  $form['culturefeed_uitpas_remind_user_without_uitpas'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show message to users without UiTPAS'),
    '#description' => t('When checked a message is shown on each page when the UiTiD user does not have a UiTPAS activated, to remind him/her to activate their card.'),
    '#default_value' => variable_get('culturefeed_uitpas_remind_user_without_uitpas', 1),
  );

  // Advantages.
  form_load_include($form_state, 'inc', 'culturefeed_uitpas', 'includes/advantages');
  culturefeed_uitpas_advantages_settings_get($form);

  // Promotions.
  form_load_include($form_state, 'inc', 'culturefeed_uitpas', 'includes/promotions');
  culturefeed_uitpas_promotions_settings_get($form);

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
    '#description' => t('These locations will only be used when only 1 card system is enabled'),
  );

  // Profile promotions.
  form_load_include($form_state, 'inc', 'culturefeed_uitpas', 'includes/profile_promotions');
  culturefeed_uitpas_profile_promotions_settings_get($form);

  // Profile upcoming promotions
  //form_load_include($form_state, 'inc', 'culturefeed_uitpas', 'includes/profile_upcoming_promotions');
  //culturefeed_uitpas_profile_upcoming_promotions_settings_get($form);

  // Profile advantages.
  form_load_include($form_state, 'inc', 'culturefeed_uitpas', 'includes/profile_advantages');
  culturefeed_uitpas_profile_advantages_settings_get($form);

  // Register where.
  form_load_include($form_state, 'inc', 'culturefeed_uitpas', 'includes/register_where');
  culturefeed_uitpas_register_where_settings_get($form);

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
