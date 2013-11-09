<?php
/**
 * @file
 * Integration with the Views module.
 */

/**
 * Implements hook_views_plugins().
 */
function culturefeed_uitpas_views_plugins() {

  $path = drupal_get_path('module', 'culturefeed_uitpas') . '/views';

  $plugins = array();

  $plugins['query']['culturefeed_uitpas_promotions_query'] = array(
    'uses fields' => TRUE,
    'path' => $path . '/plugins',
    'title' => t('Culturefeed UiTPAS promotions query'),
    'help' => t('Query on UiTPAS promotions.'),
    'handler' => 'culturefeed_uitpas_promotions_query',
  );

  return $plugins;
}

/**
 * Implements hook_views_data().
 */
function culturefeed_uitpas_views_data() {

  $data['uitpas_promotions']['table'] = array(
    'group' => t('UiTPAS promotions'),
    'base' => array(
      'field' => 'id',
      'title' => t('UiTPAS promotions'),
      'query class' => 'culturefeed_uitpas_promotions_query',
      // @todo Check what this does!
      'defaults' => array(
        'field' => 'id',
      ),
    ),
  );

  $data['uitpas_promotions']['id'] = array(
    'title' => t('ID'),
    'help' => t('The promotion ID.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
  );

  $data['uitpas_promotions']['title'] = array(
    'title' => t('Title'),
    'help' => t('The promotion title.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );

  $data['uitpas_promotions']['points'] = array(
    'title' => t('Points'),
    'help' => t('The points necessary to make use of the promotion.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
  );

  return $data;
}
