<?php

/**
 * @file
 * Callback and handler implementations for administration pages for culturefeed pages.
 */

/**
 * Configuration form for general settings.
 */
function culturefeed_pages_admin_settings() {

  $actortypes = culturefeed_search_get_actortype_categories();

  $defaults = variable_get('culturefeed_pages_actor_types', $actortypes);

  $form['culturefeed_pages_actor_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Enabled actor types'),
    '#description' => t('All actor types available for this site.'),
    '#options' => $actortypes,
    '#default_value' => $defaults,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration')
  );

  return $form;
}

/**
 * Save the general settings form.
 */
function culturefeed_pages_admin_settings_submit($form, $form_state) {
  variable_set('culturefeed_pages_actor_types', array_filter($form_state['values']['culturefeed_pages_actor_types']));
}
