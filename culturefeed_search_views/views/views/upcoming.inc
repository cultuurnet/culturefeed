<?php

$view = new view();
$view->name = 'upcoming';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'cdb_items';
$view->human_name = 'upcoming';
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
$handler->display->display_options['fields']['picture']['override'] = 0;
$handler->display->display_options['fields']['picture']['width'] = '300';
$handler->display->display_options['fields']['picture']['height'] = '300';
$handler->display->display_options['fields']['picture']['link'] = 0;
/* Veld: UDB items: Titel */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'cdb_items';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['title']['override'] = 0;
$handler->display->display_options['fields']['title']['link'] = 1;
/* Veld: UDB items: Short Description */
$handler->display->display_options['fields']['description']['id'] = 'description';
$handler->display->display_options['fields']['description']['table'] = 'cdb_items';
$handler->display->display_options['fields']['description']['field'] = 'description';
$handler->display->display_options['fields']['description']['label'] = '';
$handler->display->display_options['fields']['description']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['description']['override'] = 0;
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
/* Filter criterion: UDB items: Has Image */
$handler->display->display_options['filters']['hasimage']['id'] = 'hasimage';
$handler->display->display_options['filters']['hasimage']['table'] = 'cdb_items';
$handler->display->display_options['filters']['hasimage']['field'] = 'hasimage';
$handler->display->display_options['filters']['hasimage']['group'] = 1;
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
$handler->display->display_options['filters']['tags']['group'] = 1;
$handler->display->display_options['filters']['tags']['keyword'] = array(
  0 => array(
    'value' => '',
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
$translatables['upcoming'] = array(
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
