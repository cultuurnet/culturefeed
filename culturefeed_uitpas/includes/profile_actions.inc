<?php

/**
 * @file
 * Helper functions for UiTPAS profile actions.
 */

/**
 * Returns profile actions.
 */
function culturefeed_uitpas_profile_actions_get() {

  $uitpas_user = culturefeed_uitpas_get_uitpas_user();

  if ($uitpas_user->user && $uitpas_user->passholder) {

    $actions = array();
    $actions_max = variable_get('culturefeed_uitpas_profile_actions_actions_max', 20);
    $actions_pager_element = 0;
    $actions_page = pager_find_page($actions_pager_element);
    $actions_total = 0;

    try {

      $query = new CultureFeed_Uitpas_Event_Query_SearchCheckinsOptions();
      $query->uid = $uitpas_user->passholder->uitIdUser->id;
      $query->start = $actions_page * $actions_max;
      $query->max = $actions_max;
      $result = $uitpas_user->cf->uitpas()->searchCheckins($query);
      if ($result->total) {

        $actions = $result->objects;
        pager_default_initialize($result->total, $actions_max, $actions_pager_element);
        $actions_total = $result->total;

      }

    }
    catch (Exception $e) {

      watchdog_exception('uitpas_ui_content_type_user_profile_promotions_cashed_in', $e);
      pager_default_initialize(0, $actions_max, $actions_pager_element);

    }

  }

  return array(
    '#theme' => 'culturefeed_uitpas_profile_actions',
    '#actions' => $actions,
    '#actions_total' => $actions_total,
    '#actions_pager_element' => $actions_pager_element,
  );

}

/**
 * Returns form elements for profile actions settings.
 */
function culturefeed_uitpas_profile_actions_settings_get(&$form) {

  $form['profile_actions'] = array(
    '#type' => 'fieldset',
    '#title' => t('UiTPAS profile actions settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['profile_actions']['culturefeed_uitpas_profile_actions_intro'] = array(
    '#type' => 'textarea',
    '#title' => t('Intro'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_actions_intro', t('This page shows an overview of all your UiTPAS actions')),
  );
  $form['profile_actions']['culturefeed_uitpas_profile_actions_actions_max'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of actions to display'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_actions_actions_max', 20),
  );

}

/**
 * Title callback for the profile actions page.
 */
function culturefeed_uitpas_profile_actions_title() {

  return variable_get('culturefeed_uitpas_profile_details_menu_actions_title', t('Points history'));

}
