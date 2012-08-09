<?php

/**
 * @file
 * Block functions for uitpas_ui
 */

/**
 * Helper function for uitpas_ui where filters
 */
function uitpas_ui_block_view_filters_where() {
  $request = cnapi_ui_get_active_request();
  $query = $request['query'];
  if (isset($query['regio'])) {
    unset($query['regio']);
  }
  cnapi_ui_apply_context_query($query, $request['context']);
  $report = cnapi_get_report('event', $query);
  if (isset($report['geo'])) {
    $filter_options = array();
    $locations = array_map('trim', explode("\n", variable_get_value('uitpas_ui_locations')));
    foreach ($locations as $location) {
      $sql = "SELECT lid FROM {cnapi_location} WHERE type = 'region' && name = :name";
      $lid = db_query($sql, array(':name' => $location))->fetchField();
      if ($lid) {
        $report_item = array();
        uitpas_ui_block_view_filters_where_parse_report($report_item, $report['geo'], $lid);
        if (count($report_item)) {
          $filter_option = array(
            'id' => $report_item['id'],
            'name' => $report_item['name'],
            'url' => cnapi_url_p2dp($report_item['link'], $request['context']),
            'total' => $report_item['total'],
            'active' => FALSE,
          );
          if (isset($request['query']['regio']) && $request['query']['regio'] == $report_item['id']) {
            $request_remove = $request;
            unset($request_remove['query']['page']);
            unset($request_remove['query']['regio']);
            $filter_option['url_remove'] = $request_remove;
            $filter_option['active'] = TRUE;
          }
          $filter_options[] = $filter_option;
        }
      }
    }
    if (count($filter_options)) {
      $render = array(
        'filter_options' => array(
          '#theme' => 'cnapi_browse_filter_options',
          '#options' => $filter_options,
        ),
      );
      $block['subject'] = t('Where ?');
      $block['content'] = drupal_render($render);
      return $block;
    }
  }
}

/**
 * Helper function for uitpas_ui where filters, recursive search
 */
function uitpas_ui_block_view_filters_where_parse_report(&$report_item, $reports, $lid) {
  foreach ($reports as $key => $report) {
    if ($key == $lid) {
      $report_item = $report;
    }
    else if (isset($report['children'])) {
      uitpas_ui_block_view_filters_where_parse_report($report_item, $report['children'], $lid);
    }
  }
}

/**
 * Helper function for uitpas_ui sidebar
 */
function uitpas_ui_block_view_sidebar() {
  $panel_mini = panels_mini_load('uitpas_sidebar');
  ctools_include('context');
  //we get our page manager location context with a short version of page_page_manager_page_execute()
  $item = menu_get_item();
  $location_context = ctools_context_create('uitpas_location');
  if ($item['page_callback'] == 'page_manager_page_execute') {
    $page = page_manager_page_load($item['page_arguments'][0]);
    if ($page->arguments && isset($page->arguments['uitpas_location'])) {
      $location_context = ctools_context_create('uitpas_location', arg(1));
    }
  }
  $panel_mini->context = $panel_mini->display->context = ctools_context_load_contexts($panel_mini);
  $panel_mini->display->context['requiredcontext_uitpas_location_1'] = $location_context;
  $block['subject'] = '';
  $block['content'] = panels_render_display($panel_mini->display);
  return $block;
}


function uitpas_ui_block_view_search() {
  $block['subject'] = 'Zoek';
  $block['content'] = drupal_get_form('uitpas_ui_search_form');

  return $block;
}

function uitpas_ui_search_form() {
  $form = array();

  $form['query'] = array(
    '#type' => 'textfield',
    '#size' => 20,
    '#default_value' => 'Zoek op trefwoord',
    '#attributes' => array('class' => array('cleardefault')),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Zoek',
  );

  return $form;
}

function uitpas_ui_search_form_submit($form, &$form_state) {
  // The search form relies on control of the redirect destination for its
  // functionality, so we override any static destination set in the request,
  // for example by drupal_access_denied() or drupal_not_found()
  // (see http://drupal.org/node/292565).
  if (isset($_GET['destination'])) {
    unset($_GET['destination']);
  }

  // initialising the request object with the event context
  $request = array('context' => 'event');

  cnapi_browse_process_form_values($request, $form_state['values']);

  cnapi_ui_goto($request);
}