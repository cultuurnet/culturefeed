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

  $collaboration_tags = variable_get('culturefeed_entry_ui_custom_collaboration_tags', array());
  foreach ($collaboration_tags as $tag) {

    $tag_name = $tag['value'];
    $tag_machine_name = str_replace(' ', '_', $tag['value']);

    $view = new view();
    $view->name = 'upcoming_collaboration_' . $tag_machine_name;
    $view->description = '';
    $view->tag = 'default';
    $view->base_table = 'cdb_items';
    $view->human_name = 'Upcoming collaboration ' . $tag_name;
    $view->core = 7;
    $view->api_version = '3.0';
    $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

    /* Display: Master */
    $handler = $view->new_display('default', 'Master', 'default');
    $handler->display->display_options['title'] = 'Binnenkort';
    $handler->display->display_options['use_more_always'] = FALSE;
    $handler->display->display_options['use_more_text'] = 'meer';
    $handler->display->display_options['access']['type'] = 'none';
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Pas toe';
    $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Opnieuw instellen';
    $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Sorteer op';
    $handler->display->display_options['exposed_form']['options']['sort_asc_label'] = 'Stijgend';
    $handler->display->display_options['exposed_form']['options']['sort_desc_label'] = 'Dalend';
    $handler->display->display_options['pager']['type'] = 'some';
    $handler->display->display_options['pager']['options']['items_per_page'] = '5';
    $handler->display->display_options['style_plugin'] = 'default';
    $handler->display->display_options['row_plugin'] = 'fields';
    /* Veld: UDB items: Afbeelding */
    $handler->display->display_options['fields']['picture']['id'] = 'picture';
    $handler->display->display_options['fields']['picture']['table'] = 'cdb_items';
    $handler->display->display_options['fields']['picture']['field'] = 'picture';
    $handler->display->display_options['fields']['picture']['label'] = '';
    $handler->display->display_options['fields']['picture']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['picture']['override'] = 1;
    $handler->display->display_options['fields']['picture']['width'] = '300';
    $handler->display->display_options['fields']['picture']['height'] = '300';
    $handler->display->display_options['fields']['picture']['link'] = 0;
    /* Veld: UDB items: Titel */
    $handler->display->display_options['fields']['title']['id'] = 'title';
    $handler->display->display_options['fields']['title']['table'] = 'cdb_items';
    $handler->display->display_options['fields']['title']['field'] = 'title';
    $handler->display->display_options['fields']['title']['label'] = '';
    $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['title']['override'] = 1;
    $handler->display->display_options['fields']['title']['link'] = 1;
    /* Veld: UDB items: Short Description */
    $handler->display->display_options['fields']['description']['id'] = 'description';
    $handler->display->display_options['fields']['description']['table'] = 'cdb_items';
    $handler->display->display_options['fields']['description']['field'] = 'description';
    $handler->display->display_options['fields']['description']['label'] = '';
    $handler->display->display_options['fields']['description']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['description']['override'] = 1;
    /* Veld: UDB items: Kalendersamenvatting */
    $handler->display->display_options['fields']['when']['id'] = 'when';
    $handler->display->display_options['fields']['when']['table'] = 'cdb_items';
    $handler->display->display_options['fields']['when']['field'] = 'when';
    $handler->display->display_options['fields']['when']['label'] = 'Wanneer';
    $handler->display->display_options['fields']['when']['element_label_colon'] = FALSE;
    /* Veld: UDB items: Locatie */
    $handler->display->display_options['fields']['location']['id'] = 'location';
    $handler->display->display_options['fields']['location']['table'] = 'cdb_items';
    $handler->display->display_options['fields']['location']['field'] = 'location';
    $handler->display->display_options['fields']['location']['label'] = 'Waar';
    $handler->display->display_options['fields']['location']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['location']['show'] = array(
      'city' => 'city',
      'title' => 0,
      'address' => 0,
    );
    /* Veld: UDB items: Link */
    $handler->display->display_options['fields']['link']['id'] = 'link';
    $handler->display->display_options['fields']['link']['table'] = 'cdb_items';
    $handler->display->display_options['fields']['link']['field'] = 'link';
    $handler->display->display_options['fields']['link']['label'] = '';
    $handler->display->display_options['fields']['link']['element_label_colon'] = FALSE;
    /* Veld: UDB items: Categorie */
    $handler->display->display_options['fields']['category']['id'] = 'category';
    $handler->display->display_options['fields']['category']['table'] = 'cdb_items';
    $handler->display->display_options['fields']['category']['field'] = 'category';
    $handler->display->display_options['fields']['category']['label'] = '';
    $handler->display->display_options['fields']['category']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['category']['link'] = 1;
    /* Sort criterion: UDB items: Gewicht */
    $handler->display->display_options['sorts']['weight_sort']['id'] = 'weight_sort';
    $handler->display->display_options['sorts']['weight_sort']['table'] = 'cdb_items';
    $handler->display->display_options['sorts']['weight_sort']['field'] = 'weight_sort';
    $handler->display->display_options['sorts']['weight_sort']['order'] = 'DESC';
    /* Sort criterion: UDB items: Datum */
    $handler->display->display_options['sorts']['date']['id'] = 'date';
    $handler->display->display_options['sorts']['date']['table'] = 'cdb_items';
    $handler->display->display_options['sorts']['date']['field'] = 'date';
    $handler->display->display_options['sorts']['date']['order'] = 'ASC';
    /* Filter criterion: UDB items: Datum */
    $handler->display->display_options['filters']['date']['id'] = 'date';
    $handler->display->display_options['filters']['date']['table'] = 'cdb_items';
    $handler->display->display_options['filters']['date']['field'] = 'date';
    $handler->display->display_options['filters']['date']['group'] = 1;
    $handler->display->display_options['filters']['date']['option_absolute'] = '1';
    $handler->display->display_options['filters']['date']['relative_date'] = '+2 weeks';
    /* Filter criterion: UDB items: Labels */
    $handler->display->display_options['filters']['tags']['id'] = 'tags';
    $handler->display->display_options['filters']['tags']['table'] = 'cdb_items';
    $handler->display->display_options['filters']['tags']['field'] = 'tags';
    $handler->display->display_options['filters']['tags']['operator'] = 'and';
    $handler->display->display_options['filters']['tags']['group'] = 1;
    $handler->display->display_options['filters']['tags']['keyword'] = array(
      0 => array(
        'value' => $tag_name,
        'not' => 0,
      ),
      1 => array(
        'value' => '',
        'not' => 0,
      ),
      2 => array(
        'value' => '',
        'not' => 0,
      ),
      3 => array(
        'value' => '',
        'not' => 0,
      ),
      4 => array(
        'value' => '',
        'not' => 0,
      ),
      5 => array(
        'value' => '',
        'not' => 0,
      ),
      6 => array(
        'value' => '',
        'not' => 0,
      ),
      7 => array(
        'value' => '',
        'not' => 0,
      ),
      8 => array(
        'value' => '',
        'not' => 0,
      ),
      9 => array(
        'value' => '',
        'not' => 0,
      ),
    );

    /* Display: Block */
    $handler = $view->new_display('block', 'Block', 'block');
    $translatables['upcoming_collaboration'] = array(
      t('Master'),
      t('Binnenkort'),
      t('meer'),
      t('Pas toe'),
      t('Opnieuw instellen'),
      t('Sorteer op'),
      t('Stijgend'),
      t('Dalend'),
      t('Wanneer'),
      t('Waar'),
      t('Block'),
    );

    $views[$view->name] = $view;

  }

  return $views;
}
