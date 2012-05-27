<?php

function uitpas_ui_block_view_filters_where() {
  $request = cnapi_ui_get_active_request();
  $request_p = cnapi_url_dp2p($request);
  $report_query = $request_p['query'];
  cnapi_ui_apply_context_query($report_query, $request['context']);
  unset($report_query['page']);
  $filter_options = array();
  $locations = array_map('trim', explode("\n", variable_get_value('uitpas_ui_locations')));
  foreach ($locations as $location) {
    $sql = "SELECT lid FROM {cnapi_location} WHERE type = 'region' && name = :name";
    $lid = db_query($sql, array(':name' => $location))->fetchField();
    if ($lid) {
      $report_query['regio'] = $lid;
      $report = cnapi_get_report('event', $report_query, TRUE);
      if (isset($report['geo']) && count($report['geo'])) {
        $geo_report = array_shift($report['geo']);
        //@TODO result always seems to group items on index 1, not sure if we should hardcode it here
        if (isset($geo_report['children'][1]['children'])) {
          $report_item = array_shift($geo_report['children'][1]['children']);
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