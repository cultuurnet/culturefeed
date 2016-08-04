<?php

/**
 * @file
 * Helper functions for UiTPAS profile advantages.
 */

/**
 * Returns profile advantages.
 */
function culturefeed_uitpas_profile_advantages_get($filter = NULL) {

  $uitpas_user = culturefeed_uitpas_get_uitpas_user();

  $advantages = array();
  $advantages_max = variable_get('culturefeed_uitpas_profile_advantages_advantages_max', 20);
  $advantages_page = pager_find_page();
  $advantages_total = 0;

  if ($uitpas_user->user && $uitpas_user->passholder) {

    $card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');
    $card_systems = $uitpas_user->passholder->cardSystemSpecific;
    $first_card = key($card_systems);
    
    foreach ($uitpas_user->passholder->cardSystemSpecific as $cardsystem) {
      if (isset($cardsystem->currentCard->uitpasNumber)) {
        $uitpas_number = $cardsystem->currentCard->uitpasNumber;
        continue;
      }
    }

    // Welcome advantages.
    try {
      $query = new CultureFeed_Uitpas_Passholder_Query_WelcomeAdvantagesOptions();
      $query->uitpas_number = $uitpas_number;
      $query->cashedIn = FALSE;
      if ($filter) {
        if (strpos($filter, '+') === FALSE) {
          $query->owningCardSystemId = $filter;
        }
        else {
          $query->applicableCardSystemId = array_slice(explode('+', $filter), 1);
        }
      }

      $query->start = $advantages_page * $advantages_max;
      $query->max = $advantages_max;
      $result = $uitpas_user->cf->uitpas()->getWelcomeAdvantagesForPassholder($query);
      $advantages = $result->objects;
      pager_default_initialize($result->total, $advantages_max);
      $advantages_total = $result->total;
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_uitpas_profile_advantages', $e);
      pager_default_initialize(0, $advantages_max);
    }

    // Title.
    drupal_set_title(t('My advantages'), PASS_THROUGH);

  }

  // Solution csdco on from https://www.drupal.org/node/1049354
  $_GET['q'] = 'culturefeed/profile/uitpas/advantages';

  return array(
    array(
      '#theme' => 'culturefeed_uitpas_profile_advantages',
      '#advantages' => $advantages,
      '#advantages_total' => $advantages_total,
    ),
    array(
      '#theme' => 'pager',
      '#parameters' => array('filter' => $filter),
    ),
  );

}

/**
 * Returns form elements for profile advantages settings.
 */
function culturefeed_uitpas_profile_advantages_settings_get(&$form) {

  $form['profile_advantages'] = array(
    '#type' => 'fieldset',
    '#title' => t('My UiTPAS advantages settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['profile_advantages']['culturefeed_uitpas_profile_advantages_advantages_max'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of advantages to display'),
    '#default_value' => variable_get('culturefeed_uitpas_profile_advantages_advantages_max', 20),
  );

}

/**
 * Title callback for the profile advantages page.
 */
function culturefeed_uitpas_profile_advantages_title() {

  return t('My advantages');

}

/**
 * Returns form elements for filter & sort on profile promotions page.
 */
function culturefeed_uitpas_profile_advantages_filter_sort($form, $form_state) {

  $form['profile_advantages_link'] = array(
    '#attributes' => array(
      'id' => 'advantages_link'
    ),
    '#links' => array(
      'promotions' => array(
        'href' => 'culturefeed/profile/uitpas/promotions',
        'title' => t('My Promotions'),
      ),
      'advantages' => array(
        'href' => 'culturefeed/profile/uitpas/advantages',
        'title' => t('Welcome Advantages'),
      ),
    ),
    '#theme' => 'links',
  );

  // We can't show welcome advantages in other regions than owning card system.
  $other_regions = FALSE;

  // Get the card systems for the passholder
  $options = culturefeed_uitpas_get_user_card_systems_as_options($other_regions);

  $id = drupal_html_id('profile-advantages-filter-sort');
  $form['profile_advantages']['#prefix'] = '<div class="filters filters-regions" id="' . $id . '">';
  $form['profile_advantages']["#suffix"] = '</div>';

  $filter = isset($form_state['values']['filter']) ? $form_state['values']['filter'] : (isset($_GET['filter']) ? $_GET['filter'] : NULL);

  if (count($options) > 1) {
    $form['profile_advantages']['filter'] = array(
      '#type' => 'radios',
      '#title' => t('Distributed by'),
      '#default_value' => $filter,
      '#empty_option' => t('All card systems'),
      '#options' => $options,
      '#ajax' => array(
        'event' => 'change',
        'wrapper' => $id,
        'callback' => 'culturefeed_uitpas_profile_advantages_ajax_callback',
      ),
    );
  }

  $results = culturefeed_uitpas_profile_advantages_get($filter);
  $form['profile_advantages']['result'] = array(
    '#markup' => drupal_render($results),
  );

  $form['profile_advantages']['#attached']['css'][] = drupal_get_path('module', 'culturefeed_uitpas') . '/css/culturefeed_uitpas.css';

  return $form;
}

function culturefeed_uitpas_profile_advantages_ajax_callback($form, $form_state) {
  return $form['profile_advantages'];
}
