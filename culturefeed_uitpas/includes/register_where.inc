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
  $card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');

  $pos = array();
  $pos_max = variable_get('culturefeed_uitpas_register_where_pos_max', 10);
  $pos_pager_element = 0;
  $pos_page = pager_find_page($pos_pager_element);
  $pos_total = 0;

  $actors = array();

  // POS.
  if ($card_system_id) {

    try {

      $cf = DrupalCultureFeed::getConsumerInstance();
      $query = new CultureFeed_Uitpas_Counter_Query_SearchPointsOfSaleOptions();
      $query->start = $pos_page * $pos_max;
      $query->max = $pos_max;
      $query->cardSystemId = $card_system_id;
      $query->visible = TRUE;
      if ($location != 'all') {
        $query->city = $location;
      }
      $result = $cf->uitpas()->searchPointOfSales($query);

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
  $form['register_where']['culturefeed_uitpas_register_where_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => variable_get('culturefeed_uitpas_register_where_title', t('Register your UiTPAS')),
  );
  $form['register_where']['culturefeed_uitpas_register_where_filters_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Filters title'),
    '#default_value' => variable_get('culturefeed_uitpas_register_where_filters_title', t('Registration counters')),
  );
  $form['register_where']['culturefeed_uitpas_register_where_intro'] = array(
    '#type' => 'textarea',
    '#title' => t('Intro'),
    '#default_value' => variable_get('culturefeed_uitpas_register_where_intro'),
  );
  $form['register_where']['culturefeed_uitpas_register_where_outro'] = array(
    '#type' => 'textarea',
    '#title' => t('Outro'),
    '#default_value' => variable_get('culturefeed_uitpas_register_where_outro'),
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

  return variable_get('culturefeed_uitpas_register_where_title', t('Register your UiTPAS'));

}

/**
 * Form builder for the POS filters.
 */
function culturefeed_uitpas_register_where_filter_form($form, $form_state) {

  $locations = variable_get('culturefeed_uitpas_locations');
  $location = culturefeed_uitpas_get_uitpas_location();

  $options = array('all' => t('All')) + drupal_map_assoc($locations);

  $title = variable_get('culturefeed_uitpas_register_where_filters_title');
  if ($title) {

    $form['title'] = array(
      '#type' => 'markup',
      '#markup' => $title,
    );

  }

  $form['location'] = array(
    '#type' => 'select',
    '#title' => t('City'),
    '#options' => $options,
    '#default_value' => $location,
  );

  $form['location'] = array(
    '#type' => 'select',
    '#title' => t('City'),
    '#options' => $options,
    '#default_value' => $location,
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

  $form_state['redirect'] = array(current_path(), array('query' => $query));

}
