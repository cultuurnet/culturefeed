<?php

/**
 * @file
 * Plugin to provide a content type for uitpas recent actions.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Uitpas: recent actions'),
  'description' => t('Uitpas: recent actions.'),
  'category' => t('Uitpas'),
  'required context' => array(
    new ctools_context_required(t('Uitpas loation'), 'uitpas_location'),
    new ctools_context_required(t('Uitpas user'), 'uitpas_user'),
  ),
);

function uitpas_ui_uitpas_recent_actions_content_type_edit_form($form, &$form_state) {
  return $form;
}

function uitpas_ui_uitpas_recent_actions_content_type_render($subtype, $conf, $args, $context) {
  $location = $context[0]->data;
  $actions = array();
  $form = array();
  try {
    $cf = DrupalCultureFeed::getConsumerInstance();
    $query = new CultureFeed_SearchActivitiesQuery();
    $query->type = CultureFeed_Activity::TYPE_UITPAS;
    $query->private = FALSE;
    $query->max = 3;
    if ($location) {
      //$query->city = $location;
    }
    $result = $cf->searchActivities($query);
    if ($result->total) {
      $actions = $result->objects;
    }
    //preferences button
    if ($context[1]->data && $context[1]->data->passholder) {
      $uitpas_user = $context[1]->data->user;
      $passholder = $context[1]->data->passholder;
      $preferences = $context[1]->data->cf->getUserPreferences($uitpas_user->id);
      $publish_actions = FALSE;
      foreach ($preferences->activityPrivacyPreferences as $preference) {
        if ($preference->activityType == CultureFeed_Activity::TYPE_UITPAS && !$preference->private) {
          $publish_actions = TRUE;
        }
      }
      if (!$publish_actions) {
        $form_id = 'uitpas_ui_uitpas_recent_actions_content_type_render_form';
        $form = drupal_get_form($form_id, $uitpas_user, $context[1]->data->cf, $preferences);
      }
    }
  }
  catch (Exception $e) {
    watchdog_exception('uitpas_ui_content_type_recent_actions', $e);
  }
  
  $block = new stdClass();
  $block->title = t('Recent actions');
  $block->content = theme('uitpas_ui_recent_actions', array('actions' => $actions, 'form' => $form));
  return $block;
}

function uitpas_ui_uitpas_recent_actions_content_type_render_form($form, &$form_state, $uitpas_user, $cf, $preferences) {
  $form['uitpas_user'] = array(
    '#type' => 'value',
    '#value' => $uitpas_user,
  );
  $form['cf'] = array(
    '#type' => 'value',
    '#value' =>$cf,
  );
  $form['preferences'] = array(
    '#type' => 'value',
    '#value' =>$preferences,
  );
  $form['publish'] = array(
    '#type' => 'submit',
    '#value' => t('Also show my UITPAS actions'),
  );
  return $form;
}

function uitpas_ui_uitpas_recent_actions_content_type_render_form_submit($form, &$form_state) {
  $uitpas_user = $form_state['values']['uitpas_user'];
  $cf = $form_state['values']['cf'];
  $preferences = new CultureFeed_Preferences();
  $set = FALSE;
  foreach ($form_state['values']['preferences']->activityPrivacyPreferences as $preference) {
    if ($preference->activityType == CultureFeed_Activity::TYPE_UITPAS) {
      $preference->private = FALSE;
      $set = TRUE;
    }
    $preferences->activityPrivacyPreferences[] = $preference;
  }
  if (!$set) {
    $preferences->activityPrivacyPreferences[] = new CultureFeed_ActivityPrivacyPreference(CultureFeed_Activity::TYPE_UITPAS, FALSE);
  }
  $cf->setUserPreferences($uitpas_user->id, $preferences);
  drupal_set_message(t('Your actions will be shown.'));
}