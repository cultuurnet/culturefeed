<?php
/**
 * @file
 */

/**
 * Implements hook_views_default_views().
 */
function culturefeed_entry_ui_views_default_views() {

  $view = new view();
  $view->name = 'culturefeed_events_createdbycurrentuser';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'cdb_items';
  $view->human_name = 'CultureFeed edit events';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Events created by me';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_text'] = t('more');
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'create culturefeed events';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = t('Select');
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = t('Reset');
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = t('Sort');
  $handler->display->display_options['exposed_form']['options']['sort_asc_label'] = t('Ascending');
  $handler->display->display_options['exposed_form']['options']['sort_desc_label'] = t('Descending');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Kop: Cdb items: Maak activiteit aan */
  $handler->display->display_options['footer']['create']['id'] = 'create';
  $handler->display->display_options['footer']['create']['table'] = 'cdb_items';
  $handler->display->display_options['footer']['create']['field'] = 'create';
  $handler->display->display_options['footer']['create']['empty'] = TRUE;
  /* Gedrag bij ontbreken van resultaten: Algemeen: Tekstveld */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = t('Notification that no results were found');
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = t('Unable to find any events created by you.');
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Veld: Cdb items: Titel */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'cdb_items';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link'] = 1;
  /* Veld: Cdb items: Afbeelding */
  $handler->display->display_options['fields']['picture']['id'] = 'picture';
  $handler->display->display_options['fields']['picture']['table'] = 'cdb_items';
  $handler->display->display_options['fields']['picture']['field'] = 'picture';
  $handler->display->display_options['fields']['picture']['label'] = '';
  $handler->display->display_options['fields']['picture']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['picture']['width'] = '100';
  $handler->display->display_options['fields']['picture']['height'] = '100';
  $handler->display->display_options['fields']['picture']['link'] = 1;
  /* Veld: Cdb items: Short Description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'cdb_items';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  $handler->display->display_options['fields']['description']['label'] = '';
  $handler->display->display_options['fields']['description']['element_label_colon'] = FALSE;
  /* Veld: Cdb items: Locatie */
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
  /* Veld: Cdb items: Calendar summary */
  $handler->display->display_options['fields']['when']['id'] = 'when';
  $handler->display->display_options['fields']['when']['table'] = 'cdb_items';
  $handler->display->display_options['fields']['when']['field'] = 'when';
  $handler->display->display_options['fields']['when']['label'] = '';
  $handler->display->display_options['fields']['when']['element_label_colon'] = FALSE;
  /* Veld: Cdb items: Laatst bijgewerkt */
  $handler->display->display_options['fields']['lastupdated']['id'] = 'lastupdated';
  $handler->display->display_options['fields']['lastupdated']['table'] = 'cdb_items';
  $handler->display->display_options['fields']['lastupdated']['field'] = 'lastupdated';
  $handler->display->display_options['fields']['lastupdated']['date_format'] = 'short';
  /* Veld: Cdb items: Bewerkingen */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'cdb_items';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['label'] = '';
  $handler->display->display_options['fields']['operations']['element_label_colon'] = FALSE;
  /* Sorteercriterium: Cdb items: Laatst bijgewerkt */
  $handler->display->display_options['sorts']['lastupdated']['id'] = 'lastupdated';
  $handler->display->display_options['sorts']['lastupdated']['table'] = 'cdb_items';
  $handler->display->display_options['sorts']['lastupdated']['field'] = 'lastupdated';
  $handler->display->display_options['sorts']['lastupdated']['order'] = 'DESC';
  /* Filtercriterium: Cdb items: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'cdb_items';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'event' => 'event',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filtercriterium: Cdb items: Created by current user */
  $handler->display->display_options['filters']['createdbycurrentuser']['id'] = 'createdbycurrentuser';
  $handler->display->display_options['filters']['createdbycurrentuser']['table'] = 'cdb_items';
  $handler->display->display_options['filters']['createdbycurrentuser']['field'] = 'createdbycurrentuser';
  $handler->display->display_options['filters']['createdbycurrentuser']['exposed'] = TRUE;
  $handler->display->display_options['filters']['createdbycurrentuser']['expose']['operator_id'] = 'createdbycurrentuser_op';
  $handler->display->display_options['filters']['createdbycurrentuser']['expose']['label'] = t('Created by current user');
  $handler->display->display_options['filters']['createdbycurrentuser']['expose']['operator'] = 'createdbycurrentuser_op';
  $handler->display->display_options['filters']['createdbycurrentuser']['expose']['identifier'] = 'createdbycurrentuser';
  
  /* Display: Block */
  $handler = $view->new_display('block', t('Block'), 'block');
  
  /* Display: Pagina */
  $handler = $view->new_display('page', t('Page'), 'page_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = t('Edit events');
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_label'] = t('Items on page');
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all_label'] = t('- All -');
  $handler->display->display_options['pager']['options']['expose']['offset_label'] = t('Start');
  $handler->display->display_options['pager']['options']['tags']['first'] = t('« first');
  $handler->display->display_options['pager']['options']['tags']['previous'] = t('‹ previous');
  $handler->display->display_options['pager']['options']['tags']['next'] = t('next ›');
  $handler->display->display_options['pager']['options']['tags']['last'] = t('last »');
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filtercriterium: Cdb items: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'cdb_items';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'event' => 'event',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  $handler->display->display_options['path'] = 'agenda/edit';
  $translatables['culturefeed_events_createdbycurrentuser'] = array(
    t('Master'),
    t('Events created by me'),
    t('more'),
    t('Select'),
    t('Reset'),
    t('Sort'),
    t('Ascending'),
    t('Descending'),
    t('Notification that no results were found'),
    t('Unable to find any events created by you.'),
    t('Last updated'),
    t('Created by current user'),
    t('Block'),
    t('Page'),
    t('Edit events'),
    t('Items on page'),
    t('- All -'),
    t('Start'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
  );

  $views[$view->name] = $view;

  return $views;
}
