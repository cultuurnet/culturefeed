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
    'argument' => array(
      'handler' => 'views_handler_argument_culturefeed_search_q',
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

  // Keywords
  $data['cdb_items']['tags'] = array(
    'title' => t('Tags'),
    'help' => t('Tags'),
    'filter' => array(
      'handler' => 'views_handler_filter_culturefeed_search_tags',
    ),
  );

  // Type
  $data['cdb_items']['type'] = array(
    'title' => t('Type'),
    'help' => t('Type of the cdb item'),
    'filter' => array(
      'handler' => 'views_handler_filter_culturefeed_search_type',
    ),
  );

  // Cdbid field
  $data['cdb_items']['cdbid'] = array(
    'title' => t('Cdbid'),
    'help' => t('Cdbid of the cdb item'),
    'field' => array(
      'handler' => 'views_handler_field_cdb_item_cdbid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_culturefeed_search_filter_query',
    ),
  );

  // Title field
  $data['cdb_items']['title'] = array(
    'title' => t('Title'),
    'help' => t('Title of the cdb item'),
    'field' => array(
      'click sortable' => TRUE,
      'handler' => 'views_handler_field_cdb_item_title',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_culturefeed_search_filter_query',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_culturefeed_search_title',
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

  // Created by field.
  $data['cdb_items']['createdbycurrentuser'] = array(
    'title' => t('Created by current user'),
    'help' => t('Perform a search created by current user'),
    'filter' => array(
      'handler' =>
        'views_handler_filter_culturefeed_search_createdbycurrentuser',
    ),
  );


  // Creation date.
  $data['cdb_items']['creation_date'] = array(
    'title' => t('Creation date'),
    'help' => t('Perform a search on the creation date.'),
    'field' => array(
      'handler' => 'views_handler_field_cdb_item_creation_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_culturefeed_search_creation_date',
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

  // Age from.
  $data['cdb_items']['agefrom'] = array(
    'title' => t('Age from'),
    'help' => t('The age from.'),
    'field' => array(
      'handler' => 'views_handler_field_cdb_item_age',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_culturefeed_search_agefrom',
      'help' => t('Perform a search on minimum required age.'),
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
    'title' => t('Price'),
    'help' => t('The price of the event.'),
    'field' => array(
      'handler' => 'views_handler_field_cdb_item_price',
    ),
    'filter' => array(
      'title' => t('Free event'),
      'help' => t('Perform a search on free events.'),
      'handler' => 'views_handler_filter_culturefeed_search_free',
    ),
  );

	// Filter on events with images.
	$data['cdb_items']['hasimage'] = array(
		'title' => t('Has Image'),
		'help' => t('Perform a search on events with images.'),
		'filter' => array(
			'handler' => 'views_handler_filter_culturefeed_search_hasimage',
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

  // Long Description.
  $data['cdb_items']['long_description'] = array(
    'title' => t('Long Description'),
    'help' => t('Long Description of the cdb item'),
    'field' => array(
      'handler' => 'views_handler_field_cdb_item_long_description',
    ),
  );

  // Keywords.
  $data['cdb_items']['keywords'] = array(
    'title' => t('Keywords'),
    'help' => t('Keywords of the cdb item'),
    'field' => array(
      'handler' => 'views_handler_field_cdb_item_keywords',
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
      'click sortable' => TRUE,
      'handler' => 'views_handler_field_cdb_item_location',
    ),
  );

	// Category.
	$data['cdb_items']['category'] = array(
		'title' => t('Category'),
		'help' => t('Category of the cdb item'),
		'field' => array(
			'handler' => 'views_handler_field_cdb_item_category',
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

  $data['cdb_items']['lastupdated'] = array(
    'title' => t('Last updated'),
    'help' => t('Last updated date/time of the cdb item'),
    'field' => array(
      'click sortable' => TRUE,
      'handler' => 'views_handler_field_cdb_item_lastupdated',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_culturefeed_search_lastupdated',
    ),
  );

  // Performers.
  $data['cdb_items']['performers'] = array(
    'title' => t('Performers'),
    'help' => t('Performers of the cdb item'),
    'field' => array(
      'handler' => 'views_handler_field_cdb_item_performers',
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

	$data['cdb_items']['weight_sort'] = array(
		'title' => t('Weight'),
		'help' => t('Sort on weight.'),
		'sort' => array(
			'handler' => 'views_handler_sort_culturefeed_search_weight'
		),
	);

  $data['cdb_items']['permanent'] = array(
    'title' => t('Permanent'),
    'help' => t('Sort on permanent status.'),
    'sort' => array(
      'handler' => 'views_handler_sort_culturefeed_search_permanent'
    ),
  );

  $data['cdb_items']['availableto'] = array(
    'title' => t('Available to'),
    'help' => t('Sort on availableto.'),
    'sort' => array(
      'handler' => 'views_handler_sort_culturefeed_search_availableto'
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
