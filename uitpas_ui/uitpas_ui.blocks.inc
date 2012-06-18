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