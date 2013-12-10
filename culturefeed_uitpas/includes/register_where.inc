<?php

/**
 * @file
 * Helper functions for UiTPAS register where.
 */

/**
 * Returns list of points of sales.
 */
function culturefeed_uitpas_register_where_get() {

  $location = culturefeed_uitpas_get_location();

  $pos = array();
  $pos_max = variable_get('culturefeed_uitpas_register_where_pos_max', 10);
  $pos_pager_element = 0;
  $pos_page = pager_find_page($pos_pager_element);
  $pos_total = 0;

  try {

    $cf = DrupalCultureFeed::getConsumerInstance();
    $query = new CultureFeed_Uitpas_Counter_Query_SearchPointsOfSaleOptions();
    $query->start = $pos_page * $pos_max;
    $query->max = $pos_max;
    if ($location) {
      $query->city = $location;
    }
    $result = $cf->uitpas()->searchPointOfSales($query);

    if ($result->total) {

      $pos = $result->objects;
      $pos_total = $result->total;
      $pos_page = pager_default_initialize($result->total, $pos_max, 0);

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
  }

  return array(
    '#theme' => 'culturefeed_uitpas_register_where',
    '#pos' => $pos,
    '#pos_pager_element' => $pos_pager_element,
    '#pos_total' => $pos_total,
    'actors' => $actors,
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
  $form['register_where']['culturefeed_uitpas_register_where_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => variable_get('culturefeed_uitpas_register_where_title', t('Register your UITPAS')),
  );
  $form['register_where']['culturefeed_uitpas_register_where_info'] = array(
    '#type' => 'textarea',
    '#title' => t('Info'),
    '#default_value' => variable_get('culturefeed_uitpas_register_where_info'),
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

  return variable_get('culturefeed_uitpas_register_where_title', t('Register your UITPAS'));

}
