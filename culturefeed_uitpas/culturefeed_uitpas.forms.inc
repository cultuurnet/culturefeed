<?php

/**
 * @file
 * Form functions for Culturefeed uitpas.
 */

/**
 * Form constructor for the activity preferences form.
 */
function culturefeed_uitpas_activity_preferences_form($form, $form_state) {

  $uitpas_user = culturefeed_uitpas_get_uitpas_user();

  if ($uitpas_user->user) {

    // @codingStandardsIgnoreStart
    /** @var CultureFeed_Preferences $preferences */
    // @codingStandardsIgnoreEnd
    $preferences = $uitpas_user->cf->getUserPreferences($uitpas_user->user->id);
    $activity_preferences = $preferences->activityPrivacyPreferences;
    $publish_actions = (isset($activity_preferences[CultureFeed_Activity::TYPE_UITPAS]->private) && !$activity_preferences[CultureFeed_Activity::TYPE_UITPAS]->private);

    $form['uitpas_user'] = array(
      '#type' => 'value',
      '#value' => $uitpas_user,
    );

    $form['publish_actions'] = array(
      '#type' => 'checkbox',
      '#title' => t('Make my UiTPAS actions available for others'),
      '#default_value' => $publish_actions,
    );

    $form['actions'] = array(
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Confirm'),
      ),
    );

  }

  return $form;

}

/**
 * Submit handler for culturefeed_uitpas_activity_preferences_form.
 */
function culturefeed_uitpas_activity_preferences_form_submit($form, $form_state) {

  $uitpas_user = $form_state['values']['uitpas_user'];
  $cf = $uitpas_user->cf;

  $preferences = new CultureFeed_Preferences();
  $private = ($form_state['values']['publish_actions']) ? FALSE : TRUE;
  $preference = new CultureFeed_ActivityPrivacyPreference(CultureFeed_Activity::TYPE_UITPAS, $private);
  $preferences->activityPrivacyPreferences[] = $preference;
  $cf->setUserPreferences($uitpas_user->user->id, $preferences);

}
