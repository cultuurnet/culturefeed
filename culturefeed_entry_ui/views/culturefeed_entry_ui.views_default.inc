<?php
/**
 * @file
 */

/**
 * Implements hook_views_default_views().
 */
function culturefeed_entry_ui_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'culturefeed_events_createdbycurrentuser';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'cdb_items';
  $view->human_name = 'CultureFeed events created by current user';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Events created by me';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'create culturefeed events';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Header: UDB items: Create event */
  $handler->display->display_options['header']['create']['id'] = 'create';
  $handler->display->display_options['header']['create']['table'] = 'cdb_items';
  $handler->display->display_options['header']['create']['field'] = 'create';
  $handler->display->display_options['header']['create']['empty'] = TRUE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'Notification that no results were found';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'Unable to find any events created by you.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: UDB items: Titel */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'cdb_items';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link'] = 1;
  /* Field: UDB items: Afbeelding */
  $handler->display->display_options['fields']['picture']['id'] = 'picture';
  $handler->display->display_options['fields']['picture']['table'] = 'cdb_items';
  $handler->display->display_options['fields']['picture']['field'] = 'picture';
  $handler->display->display_options['fields']['picture']['label'] = '';
  $handler->display->display_options['fields']['picture']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['picture']['width'] = '100';
  $handler->display->display_options['fields']['picture']['height'] = '100';
  $handler->display->display_options['fields']['picture']['link'] = 1;
  /* Field: UDB items: Short Description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'cdb_items';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  $handler->display->display_options['fields']['description']['label'] = '';
  $handler->display->display_options['fields']['description']['element_label_colon'] = FALSE;
  /* Field: UDB items: Locatie */
  $handler->display->display_options['fields']['location']['id'] = 'location';
  $handler->display->display_options['fields']['location']['table'] = 'cdb_items';
  $handler->display->display_options['fields']['location']['field'] = 'location';
  $handler->display->display_options['fields']['location']['label'] = '';
  $handler->display->display_options['fields']['location']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['location']['show'] = array(
    'city' => 'city',
    'title' => 0,
    'address' => 0,
  );
  /* Field: UDB items: Kalendersamenvatting */
  $handler->display->display_options['fields']['when']['id'] = 'when';
  $handler->display->display_options['fields']['when']['table'] = 'cdb_items';
  $handler->display->display_options['fields']['when']['field'] = 'when';
  $handler->display->display_options['fields']['when']['label'] = '';
  $handler->display->display_options['fields']['when']['element_label_colon'] = FALSE;
  /* Field: UDB items: Last updated */
  $handler->display->display_options['fields']['lastupdated']['id'] = 'lastupdated';
  $handler->display->display_options['fields']['lastupdated']['table'] = 'cdb_items';
  $handler->display->display_options['fields']['lastupdated']['field'] = 'lastupdated';
  $handler->display->display_options['fields']['lastupdated']['date_format'] = 'short';
  /* Field: UDB items: Operations */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'cdb_items';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['label'] = '';
  $handler->display->display_options['fields']['operations']['element_label_colon'] = FALSE;
  /* Sort criterion: Cdb items: Last updated */
  $handler->display->display_options['sorts']['lastupdated']['id'] = 'lastupdated';
  $handler->display->display_options['sorts']['lastupdated']['table'] = 'cdb_items';
  $handler->display->display_options['sorts']['lastupdated']['field'] = 'lastupdated';
  $handler->display->display_options['sorts']['lastupdated']['order'] = 'DESC';
  /* Filter criterion: UDB items: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'cdb_items';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'event' => 'event',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: UDB items: Created by current user */
  $handler->display->display_options['filters']['createdbycurrentuser']['id'] = 'createdbycurrentuser';
  $handler->display->display_options['filters']['createdbycurrentuser']['table'] = 'cdb_items';
  $handler->display->display_options['filters']['createdbycurrentuser']['field'] = 'createdbycurrentuser';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $translatables['culturefeed_events_createdbycurrentuser'] = array(
    t('Master'),
    t('Events created by me'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Notification that no results were found'),
    t('Unable to find any events created by you.'),
    t('Last updated'),
    t('Block'),
  );

  $views[$view->name] = $view;

  return $views;
}
