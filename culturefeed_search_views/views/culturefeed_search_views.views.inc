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

  // Filter on group.
  $data['cdb_items']['group'] = array(
    'title' => t('Group'),
    'help' => t('Add a group filter'),
    'filter' => array(
      'handler' => 'views_handler_filter_culturefeed_search_group',
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

  // Type
  $data['cdb_items']['type'] = array(
    'title' => t('Type'),
    'help' => t('Type of the cdb item'),
    'filter' => array(
      'handler' => 'views_handler_filter_culturefeed_search_type',
    ),
  );

  // Title field
  $data['cdb_items']['title'] = array(
    'title' => t('Title'),
    'help' => t('Title of the cdb item'),
    'field' => array(
      'handler' => 'views_handler_field_cdb_item_title',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_culturefeed_search_filter_query',
    ),
  );

  // City field.
  $data['cdb_items']['city'] = array(
    'title' => t('City'),
    'help' => t('Perform a search on a city.'),
    'filter' => array(
      'handler' => 'views_handler_filter_culturefeed_search_city',
    ),
  );
  
  // Created by field.
  $data['cdb_items']['createdby'] = array(
    'title' => t('Created by'),
    'help' => t('Perform a search created by certain user'),
    'filter' => array(
      'handler' => 'views_handler_filter_culturefeed_search_createdby',
    ),
  );

  // Filter on location name.
  $data['cdb_items']['location_label'] = array(
    'title' => t('Location name'),
    'help' => t('Perform a search on the location name.'),
    'filter' => array(
      'handler' => 'views_handler_filter_culturefeed_search_filter_query',
    ),
  );

  // Filter on agefrom.
  $data['cdb_items']['agefrom'] = array(
    'title' => t('Agefrom'),
    'help' => t('Perform a search on minimum required age.'),
    'filter' => array(
      'handler' => 'views_handler_filter_culturefeed_search_agefrom',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_culturefeed_search_agefrom',
    ),
  );

  // Filter on organiser.
  $data['cdb_items']['organiser_label'] = array(
    'title' => t('Organiser'),
    'help' => t('Perform a search on organiser name.'),
    'filter' => array(
      'handler' => 'views_handler_filter_culturefeed_search_filter_query',
    ),
  );

  // Filter on free event.
  $data['cdb_items']['free'] = array(
    'title' => t('Free event'),
    'help' => t('Perform a search on free events.'),
    'filter' => array(
      'handler' => 'views_handler_filter_culturefeed_search_free',
    ),
  );

  // Filter on dates
  $data['cdb_items']['datetype'] = array(
    'title' => t('Date type'),
    'help' => t('Perform a search on a predefined type of date query'),
    'filter' => array(
      'handler' => 'views_handler_filter_culturefeed_search_datetype',
    ),
  );
  $data['cdb_items']['date'] = array(
    'title' => t('Date'),
    'help' => t('Perform a search on a date'),
    'filter' => array(
      'handler' => 'views_handler_filter_culturefeed_search_daterange',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_culturefeed_search_date',
    ),
  );

  // Fields.

  // Short Description.
  $data['cdb_items']['description'] = array(
    'title' => t('Short Description'),
    'help' => t('Short Description of the cdb item'),
    'field' => array(
      'handler' => 'views_handler_field_cdb_item_short_description',
    ),
  );

  // Calendar summary.
  $data['cdb_items']['when'] = array(
    'title' => t('Calendar summary'),
    'help' => t('Summary of the calendar'),
    'field' => array(
      'handler' => 'views_handler_field_cdb_item_calendar_summary',
    ),
  );

  // Location.
  $data['cdb_items']['location'] = array(
    'title' => t('Location'),
    'help' => t('Location of the cdb item'),
    'field' => array(
      'handler' => 'views_handler_field_cdb_item_location',
    ),
  );

  // Organiser.
  $data['cdb_items']['organiser'] = array(
    'title' => t('Organiser'),
    'help' => t('Organiser of the cdb item'),
    'field' => array(
      'handler' => 'views_handler_field_cdb_item_organiser',
    ),
  );

  // Picture.
  $data['cdb_items']['picture'] = array(
    'title' => t('Picture'),
    'help' => t('Picture of the cdb item'),
    'field' => array(
      'handler' => 'views_handler_field_cdb_item_picture',
    ),
  );

  $data['cdb_items']['link'] = array(
    'title' => t('Link'),
    'help' => t('Link to the cdb item'),
    'field' => array(
      'handler' => 'views_handler_field_cdb_item_link',
    ),
  );

  // Sorts
  $data['cdb_items']['recommend_sort'] = array(
    'title' => t('Total recommend'),
    'help' => t('Sort on recommend count.'),
    'sort' => array(
      'handler' => 'views_handler_sort_culturefeed_search_recommends',
    ),
  );

  $data['cdb_items']['comment_sort'] = array(
    'title' => t('Total comments'),
    'help' => t('Sort on comment count.'),
    'sort' => array(
      'handler' => 'views_handler_sort_culturefeed_search_comments',
    ),
  );

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
        'uses fields' => TRUE,
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