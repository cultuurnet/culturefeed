<?php

/**
 * @file
 * Plugin to provide a content type for uitpas user profile uitid.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Uitpas: user profile uitid'),
  'description' => t('Uitpas: user profile uitid.'),
  'category' => t('Uitpas'),
  'required context' => new ctools_context_required(t('Uitpas user'), 'uitpas_user'),
);

function uitpas_ui_uitpas_user_profile_uitid_content_type_edit_form($form, &$form_state) {
  return $form;
}

function uitpas_ui_uitpas_user_profile_uitid_content_type_render($subtype, $conf, $args, $context) {
  if ($context->data) {
    $uitpas_user = $context->data->user;
    $passholder = $context->data->passholder;
    $cf = $context->data->cf;
    $consumer_cf = DrupalCultureFeed::getConsumerInstance();
    $preferences = $consumer_cf->getUserPreferences($uitpas_user->id);
    dpm($preferences);
    $actions = array();
    try {
      $query = new CultureFeed_Uitpas_Passholder_Query_SearchCheckinsOptions();
      $query->max = 5;
      $result = $cf->uitpas()->searchCheckins($query);
      if ($result->total) {
        $actions = $result['objects'];
      }
    }
    catch (Exception $e) {
      watchdog_exception('uitpas_ui_content_type_user_profile_uitid', $e);
    }
    $form = drupal_get_form('uitpas_ui_uitpas_user_profile_uitid_content_type_render_form', $passholder, $cf);
    $block = new stdClass();
    $block->title = t('My UITID profile');
    $block->content = theme('uitpas_ui_user_profile_uitid', array(
      'uitpas_user' => $uitpas_user,
      'passholder' => $passholder,
      'actions' => $actions,
      'form' => $form,
    ));
    return $block;
  }
}

function uitpas_ui_uitpas_user_profile_uitid_content_type_render_form($form, &$form_state, $passholder, $cf) {
  $form['passholder'] = array(
    '#type' => 'value',
    '#value' => $passholder,
  );
  $form['cf'] = array(
    '#type' => 'value',
    '#value' => $cf,
  );
  $form['publish_actions'] = array(
    '#type' => 'checkbox',
    '#title' => t('Publish my UITID actions'),
    '#default_value' => $passholder->publishActions,
  );
  $form['actions'] = array(
    'save' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    ),
  );
  return $form;
}

function uitpas_ui_uitpas_user_profile_uitid_content_type_render_form_submit(&$form, &$form_state) {
  $passholder = $form_state['values']['passholder'];
  $cf = $form_state['values']['cf'];
  $passholder->publishActions = $form_state['values']['publish_actions'];
  try {
    $cf->uitpas()->updatePassholder($passholder);
    drupal_set_message(t('Your changes have been saved.'));
  }
  catch (Exception $e) {
    watchdog_exception('uitpas_ui_content_type_user_profile_uitid', $e);
    drupal_set_message(t('An error occured.'), 'error');
  }
}