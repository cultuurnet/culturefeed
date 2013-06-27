<?php

/**
 * @file
 * Views integration.
 */

/**
 * Implements hook_views_data().
 */
function culturefeed_search_views_data() {

  $data['cdb_items']['table']['group'] = t('Cdb items');
  $data['cdb_items']['table']['base'] = array(
    'field' => 'cdbid',
    'title' => t('Cdb item'),
    'query class' => 'culturefeed_search_views_query',
  );

  // Filter on a custom query
  $data['cdb_items']['q'] = array(
    'title' => t('Query'),
    'help' => t('Perform a free text search.'),
    'filter' => array(
      'handler' => 'views_handler_filter_culturefeed_search_q',
    ),
  );

  // Add filter on category per category type.
  $domains = culturefeed_search_get_domains();
  foreach ($domains as $did => $label) {
    $data['cdb_items']['domain_' . $did] = array(
      'title' => t('Category: @domain', array('@domain' => $label)),
      'help' => t('Perform a search on category of this type.'),
      'filter' => array(
        'handler' => 'views_handler_filter_culturefeed_search_category',
      ),
    );
  }

  // Filter on city.
  $data['cdb_items']['city'] = array(
    'title' => t('City'),
    'help' => t('Perform a search on the name of a city.'),
    'filter' => array(
      'handler' => 'views_handler_filter_culturefeed_search_city',
    ),
  );
  $data['cdb_items']['zip'] = array(
    'title' => t('City'),
    'help' => t('Perform a search on the zipcode of a city.'),
    'filter' => array(
      'handler' => 'views_handler_filter_culturefeed_search_zip',
    ),
  );

  // Filter on location name.
  $data['cdb_items']['location_label'] = array(
    'title' => t('Location name'),
    'help' => t('Perform a search on the location name.'),
    'filter' => array(
      'handler' => 'views_handler_filter_culturefeed_search_string',
    ),
  );

  // Filter on agefrom.
  $data['cdb_items']['agefrom'] = array(
    'title' => t('Agefrom'),
    'help' => t('Perform a search on minimum required age.'),
    'filter' => array(
      'handler' => 'views_handler_filter_culturefeed_search_agefrom',
    ),
  );

  // Filter on organiser.
  $data['cdb_items']['organiser_label'] = array(
    'title' => t('Organiser'),
    'help' => t('Perform a search on organiser name.'),
    'filter' => array(
      'handler' => 'views_handler_filter_culturefeed_search_string',
    ),
  );

  // @todo Filter on free event.
  // @todo Filter on permanent activity.

  return $data;
}

/**
 * Implements hook_views_plugins().
 */
function culturefeed_search_views_views_plugins() {

  $path = drupal_get_path('module', 'culturefeed_search_views') . '/views';

  return array(
    'module' => 'culturefeed_search_views',
    'query' => array(
      'culturefeed_search_views_query' => array(
        'uses fields' => FALSE,
        'path' => $path . '/plugins',
        'title' => t('Culturefeed Search Query'),
        'help' => t('Query that allows you to search with culturefeed.'),
        'handler' => 'culturefeed_search_views_query',
        'parent' => 'views_query',
      ),
    ),
    'row' => array(
      'cdb_items' => array(
        'title' => t('Culturefeed cdb items'),
        'handler' => 'culturefeed_search_views_row_cdb',
        'path' => $path . '/plugins',
        'base' => array('cdb_items'),
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
  );

}