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
  if ($context->data && $context->data->passholder) {
    $uitpas_user = $context->data->user;
    $passholder = $context->data->passholder;
    $cf = $context->data->cf;
    $preferences = $cf->getUserPreferences($uitpas_user->id);
    $actions = array();
    try {
      $query = new CultureFeed_Uitpas_Passholder_Query_SearchCheckinsOptions();
      $query->uid = $uitpas_user->id;
      $query->max = 5;
      $result = $cf->uitpas()->searchCheckins($query);
      if ($result->total) {
        $actions = $result['objects'];
      }
    }
    catch (Exception $e) {
      watchdog_exception('uitpas_ui_content_type_user_profile_uitid', $e);
    }
    $form = drupal_get_form('uitpas_ui_uitpas_user_profile_uitid_content_type_render_form', $uitpas_user, $cf, $preferences);
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

function uitpas_ui_uitpas_user_profile_uitid_content_type_render_form($form, &$form_state, $uitpas_user, $cf, $preferences) {
  $form['uitpas_user'] = array(
    '#type' => 'value',
    '#value' => $uitpas_user,
  );
  $form['cf'] = array(
    '#type' => 'value',
    '#value' => $cf,
  );
  $form['preferences'] = array(
    '#type' => 'value',
    '#value' => $preferences,
  );
  $publish_actions = FALSE;
  foreach ($preferences->activityPrivacyPreferences as $preference) {
    if ($preference->activityType == CultureFeed_Activity::TYPE_UITPAS && !$preference->private) {
      $publish_actions = TRUE;
    }
  }
  $form['publish_actions'] = array(
    '#type' => 'checkbox',
    '#title' => t('Publish my UITID actions'),
    '#default_value' => $publish_actions,
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
  try {
    $uitpas_user = $form_state['values']['uitpas_user'];
    $cf = $form_state['values']['cf'];
    $preferences = new CultureFeed_Preferences();
    $private = ($form_state['values']['publish_actions']) ? FALSE : TRUE;
    $set = FALSE;
    foreach ($form_state['values']['preferences']->activityPrivacyPreferences as $preference) {
      if ($preference->activityType == CultureFeed_Activity::TYPE_UITPAS) {
        $preference->private = $private;
        $set = TRUE;
      }
      $preferences->activityPrivacyPreferences[] = $preference;
    }
    if (!$set) {
      $preferences->activityPrivacyPreferences[] = new CultureFeed_ActivityPrivacyPreference(CultureFeed_Activity::TYPE_UITPAS, $private);
    }
    $cf->setUserPreferences($uitpas_user->id, $preferences);
    drupal_set_message(t('Your changes have been saved.'));
  }
  catch (Exception $e) {
    watchdog_exception('uitpas_ui_content_type_user_profile_uitid', $e);
    drupal_set_message(t('An error occured.'), 'error');
  }
}