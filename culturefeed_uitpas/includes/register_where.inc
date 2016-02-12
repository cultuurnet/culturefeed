<?php

/**
 * @file
 * Helper functions for UiTPAS register where.
 */

/**
 * Returns list of points of sales.
 */
function culturefeed_uitpas_register_where_get() {

  $location = culturefeed_uitpas_get_uitpas_location();

  $card_system_id = isset($_GET['region']) ? $_GET['region'] : 'all';
  $kansenstatuut = isset($_GET['kansenstatuut']) && $_GET['kansenstatuut'];
  
  $card_systems = culturefeed_uitpas_get_card_systems();

  $pos = array();
  $pos_max = variable_get('culturefeed_uitpas_register_where_pos_max', 10);
  $pos_pager_element = 0;
  $pos_page = pager_find_page($pos_pager_element);
  $pos_total = 0;

  $actors = array();

  try {

    $cf = DrupalCultureFeed::getConsumerInstance();
    $query = new CultureFeed_Uitpas_Counter_Query_SearchCounterOptions();
    $query->start = $pos_page * $pos_max;
    $query->max = $pos_max;
    $query->visible = 'true';

    if ($card_system_id != 'all') {
      $query->cardSystemId = $card_system_id;
    }
    elseif ($card_systems) {
      $card_systems_ids = array();
      foreach ($card_systems as $card_system) {
        $card_systems_ids[] = $card_system->id;
      }
      $query->cardSystemId = $card_systems_ids;
    }
    $query->groupName = array(
      CultureFeed_Uitpas_Counter_Query_SearchCounterOptions::GROUP_AUTHORIZED,
    );
    if (!$kansenstatuut) {
      $query->groupName[] = CultureFeed_Uitpas_Counter_Query_SearchCounterOptions::GROUP_NOT_AUTHORIZED;
    }
    if ($location != 'all') {
      $query->city = $location;
    }

    $result = $cf->uitpas()->searchCounters($query);

    if ($result->total) {

      $pos = $result->objects;
      $pos_total = $result->total;
      pager_default_initialize($result->total, $pos_max, 0);

      foreach ($result->objects as $object) {

        $actor = culturefeed_agenda_actor_load($object->id);
        if ($actor) {
          $actors[$object->id] = $actor;
        }

      }

    }

  }
  catch (Exception $e) {

    watchdog_exception('uitpas_ui_content_type_register_where', $e);
    pager_default_initialize(0, $pos_max, 0);

  }

  // Form.
  $filters = drupal_get_form('culturefeed_uitpas_register_where_filter_form');

  return array(
    '#theme' => 'culturefeed_uitpas_register_where',
    '#pos' => $pos,
    '#pos_pager_element' => $pos_pager_element,
    '#pos_total' => $pos_total,
    '#filters' => drupal_render($filters),
    '#actors' => $actors,
  );

}

/**
 * Returns form elements for profile summary settings.
 */
function culturefeed_uitpas_register_where_settings_get(&$form) {

  $form['register_where'] = array(
    '#type' => 'fieldset',
    '#title' => t('UiTPAS register where settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['register_where']['culturefeed_uitpas_register_where_pos_max'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of points of sale to display'),
    '#default_value' => variable_get('culturefeed_uitpas_register_where_pos_max', 10),
  );

}

/**
 * Title callback for the register where page.
 */
function culturefeed_uitpas_register_where_title() {

  return t('Get your UiTPAS');

}

/**
 * Form builder for the POS filters.
 */
function culturefeed_uitpas_register_where_filter_form($form, $form_state) {

  $multiple_card_systems = culturefeed_uitpas_multiple_cardsystems();
  
  if ($multiple_card_systems) {
    // Use card system regions as filter when multiple card sytems are enabled
    $options = array('all' => t('All regions'));
    $card_systems = culturefeed_uitpas_get_card_systems();
    foreach ($card_systems as $card_system) {
      $options[$card_system->id] = $card_system->name;
    }
    $default_value = isset($_GET['region']) ? $_GET['region'] : '';
  
    $form['region'] = array(
      '#type' => 'select',
      '#title' => t('UiTPAS Region'),
      '#options' => $options,
      '#default_value' => $default_value,
    );
  }
  
  else {
    // Use custom UiTPAS locations as filter when only one card sytem is enabled
    $locations = variable_get('culturefeed_uitpas_locations');		
    $location = culturefeed_uitpas_get_uitpas_location();		
    $options = array('all' => t('All')) + drupal_map_assoc($locations);
    $form['location'] = array(
      '#type' => 'select',
      '#title' => t('City'),
      '#options' => $options,
      '#default_value' => $location,
    );
  }

  $form['kansenstatuut'] = array(
    '#type' => 'checkbox',
    '#title' => t('I have a kansenstatuut'),
    '#default_value' => isset($_GET['kansenstatuut']) ? $_GET['kansenstatuut'] : 0,
  );

  $form['actions'] = array(
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Search'),
    ),
  );

  return $form;

}

/**
 * Submit handler for culturefeed_uitpas_register_where_filter_form.
 */
function culturefeed_uitpas_register_where_filter_form_submit($form, &$form_state) {

  $query = array();
  if (isset($form_state['values']['location'])) {
    $query['uitpas_location'] = $form_state['values']['location'];
  }
  if (isset($form_state['values']['region'])) {
    $query['region'] = $form_state['values']['region'];
  }
  if (isset($form_state['values']['kansenstatuut'])) {
    $query['kansenstatuut'] = $form_state['values']['kansenstatuut'];
  }

  $form_state['redirect'] = array(current_path(), array('query' => $query));

}
