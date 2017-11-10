<?php

/**
 * @file
 * Callback and handler implementations for administration pages.
 */

/**
 * Configuration form for general settings.
 *
 * @ingroup forms
 *
 * @see system_settings_form()
 */
function culturefeed_mailing_admin_settings() {

  $form = array();
  try {

    $query = new CultureFeed_SearchMailingsQuery();
    $lists = DrupalCultureFeed_Mailing::getMailingList($query)->objects;
    $options = array('' => t('None'));

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
      '#title' => t('Newsletter block description'),
      '#description' => t('Description displayed in the form as block'),
      '#default_value' => variable_get('culturefeed_mailing_block_description', ''),
    );

    $form['culturefeed_mailing_optional_description'] = array(
      '#type' => 'textarea',
      '#title' => t('Optional newsletter description'),
      '#description' => t('Description displayed beside the checkbox for an optional registration'),
      '#default_value' => variable_get('culturefeed_mailing_optional_description', ''),
    );

    $form['culturefeed_mailing_subscribed_message'] = array(
      '#type' => 'textarea',
      '#title' => t('Subscribed message'),
      '#description' => t('Message after a successfull subscription'),
      '#default_value' => variable_get('culturefeed_mailing_subscribed_message', t('Successfully subscribed to our newsletter')),
    );

  }
  catch (Exception $e) {
    watchdog('culturefeed_mailing', $e->getMessage());
    drupal_set_message($e->getMessage(), WATCHDOG_ERROR);
  }
  return system_settings_form($form);
}
