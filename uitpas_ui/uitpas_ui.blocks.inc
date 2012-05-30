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
  cnapi_ui_apply_context_query($request['query'], $request['context']);
  $report = cnapi_get_report('event', $request['query']);
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