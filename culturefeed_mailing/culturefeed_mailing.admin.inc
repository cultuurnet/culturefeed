<?php

/**
 * @file
 * Callback and handler implementations for administration pages.
 */

/**
 * Configuration form for general settings.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function culturefeed_mailing_admin_settings() {

  $form = array();
  try {

    $query = new CultureFeed_SearchMailingsQuery();
    $lists = DrupalCultureFeed::getMailingList($query)->objects;
    $options = array('' => 'None');

    foreach ($lists as $list) {
      $options[$list->id] = $list->name;
    }

    $form['culturefeed_mailing_list'] = array(
      '#type' => 'select',
      '#title' => t('Mailing list'),
      '#options' => $options,
      '#default_value' => variable_get('culturefeed_mailing_list', ''),
    );

    $form['culturefeed_mailing_list_optional'] = array(
      '#type' => 'select',
      '#title' => t('Optional mailing list'),
      '#options' => $options,
      '#default_value' => variable_get('culturefeed_mailing_list_optional', ''),
    );

    $form['culturefeed_mailing_block_description'] = array(
      '#type' => 'textfield',
      '#title' => 'Nieuwsbrief block omschrijving',
      '#description' => 'Omschrijving die getoond wordt bij het formulier als block',
      '#default_value' => variable_get('culturefeed_mailing_block_description', ''),
    );

    $form['culturefeed_mailing_optional_description'] = array(
      '#type' => 'textarea',
      '#title' => 'Optionele nieuwsbrief omschrijving',
      '#description' => 'Omschrijving die getoond wordt bij de checkbox voor een optionele inschrijving',
      '#default_value' => variable_get('culturefeed_mailing_optional_description', ''),
    );

    $form['culturefeed_mailing_subscribed_message'] = array(
      '#type' => 'textarea',
      '#title' => 'Optionele nieuwsbrief omschrijving',
      '#description' => 'Omschrijving die getoond wordt bij de checkbox voor een optionele inschrijving',
      '#default_value' => variable_get('culturefeed_mailing_subscribed_message', 'U bent succesvol ingeschreven voor onze nieuwsbrief'),
    );

  }
  catch (Exception $e) {
    watchdog('culturefeed_mailing', $e->getMessage());
    drupal_set_message($e->getMessage(), WATCHDOG_ERROR);
  }
  return system_settings_form($form);
}
