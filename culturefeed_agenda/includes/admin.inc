<?php

/**
 * @file
 * Admin settings for culturefeed_agenda.
 */

/**
 * General settings for culturefeed_agenda.
 */
function culturefeed_agenda_admin_settings_form() {

  $form = array();

  $form['culturefeed_agenda_active_entity_types'] = array(
    '#title' => t('Entity types to be searched'),
    '#type' => 'checkboxes',
    '#options' => array(
      'event' => t('event', array(), array('context' => 'culturefeed-facets')),
      'production' => t('production', array(), array('context' => 'culturefeed-facets')),
      'actor' => t('actor', array(), array('context' => 'culturefeed-facets')),
    ),
    '#default_value' => variable_get('culturefeed_agenda_active_entity_types', array('event', 'production')),
    '#description' => t('Select the different entity types that can be shown on agenda/search'),
  );

  $form = system_settings_form($form);
  $form['#submit'] = array('culturefeed_agenda_admin_settings_form_submit');

  return $form;

}

/**
 * Submit the settings form.
 */
function culturefeed_agenda_admin_settings_form_submit($form, $form_state) {

  $form_state['values']['culturefeed_agenda_active_entity_types'] = array_filter($form_state['values']['culturefeed_agenda_active_entity_types']);
  system_settings_form_submit($form, $form_state);

}

