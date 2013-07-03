<?php
/**
 * @file
 * Theme / preprocess functions for culturefeed search UI.
 */

/**
 * Preprocess function for the search page main section.
 *
 * @param array $vars
 */
function culturefeed_search_ui_preprocess_culturefeed_search_page(&$vars) {

  $vars['content'] = theme('culturefeed_search_list', array('result' => $vars['searchresult']));

  $vars['results_found'] = $vars['searchresult']->getTotalCount();

}

/**
 * Preprocess function for the search list.
 *
 * @param array $vars
 */
function culturefeed_search_ui_preprocess_culturefeed_search_list(&$vars) {

  if (!empty($vars['items'])) {
    $vars['result'] = $vars['items'];
  }
  $items = $vars['result']->getItems();

  $vars['noitems'] = $vars['result']->getTotalCount() <= 0 ? TRUE : FALSE;
  $vars['items'] = array();
  foreach ($items as $item) {
    $vars['items'][] = theme('culturefeed_' . $item->getType() . '_summary', array('item' => $item));
  }

}

/**
 * Preprocess function for the search facet.
 * @see culturefeed-search-facet.tpl.php
 */
function culturefeed_search_ui_preprocess_culturefeed_search_facet(&$variables) {

  $facet = $variables['facet'];
  $path = empty($variables['path']) ? current_path() : $variables['path'];

  if (!isset($variables['query']) || !is_array($variables['query'])) {
    // Note that we need to remote the page parameter when adding/removing facet filters
    $query = drupal_get_query_parameters(NULL, array('q', 'page'));
  }
  else {
    $query = $variables['query'];
  }

  $query += array(
    'facet' => array()
  );

  $active_options = isset($query['facet'][$facet->getKey()]) ? $query['facet'][$facet->getKey()] : array();

  $items = array();
  // Add all the facet items to the build.
  foreach ($variables['items'] as $facet_item) {

    $sub_item_output = array();
    $has_active_subitem = FALSE;

    // Add the subitems from this facet item.
    if ($facet_item->hasSubItems()) {

      $sub_items = $facet_item->getSubItems();

      foreach ($sub_items as $sub_item) {

        // If the list has an active subitem. Add class.
        $sub_item_active = in_array($sub_item->getValue(), $active_options);
        if ($sub_item_active) {
          $has_active_subitem = TRUE;
        }

        $sub_item_output[] = theme('culturefeed_search_facet_item', array(
          'facet_item' => $sub_item,
          'facet' => $facet,
          'query' => $query,
          'path' => $path,
          'active' => $sub_item_active,
        ));
      }
    }

    $item_active = in_array($facet_item->getValue(), $active_options);

    // Theme the item.
    $item_output = theme('culturefeed_search_facet_item', array(
      'facet_item' => $facet_item,
      'facet' => $facet,
      'query' => $query,
      'path' => $path,
      'active' => $item_active,
    ));

    // Add it to the list.
    $items[] = array(
      'output' => $item_output,
      'sub_items' => $sub_item_output,
      'has_active_sub_item' => $has_active_subitem,
    );

  }

  $variables['items'] = $items;

}

/**
 * Preprocess function for the search facet.
 * @see culturefeed-search-facet.tpl.php
 */
function culturefeed_search_ui_preprocess_culturefeed_search_facet_eventtype(&$variables) {
  culturefeed_search_ui_preprocess_culturefeed_search_facet($variables);
}

/**
 * Preproces a search facet item.
 * @see culturefeed-search-facet-item.tpl.php
 */
function culturefeed_search_ui_preprocess_culturefeed_search_facet_item(&$variables) {

  $facet = $variables['facet'];
  $facet_item = $variables['facet_item'];
  $query = $variables['query'];

  // Every filter will reset the current facet. Remove this facet from URL, URL will function as removal URL.
  unset($query['facet'][$facet->getKey()]);

  if ($facet->getKey() == 'datetype' && isset($query['date_range'])) {
    unset($query['date_range']);
  }

  // Add this facet to URL, URL will function as additional filter URL.
  if (!$variables['active']) {
    $query['facet'][$facet->getKey()][] = $facet_item->getValue();
  }

  // Remove facet completely if nothing left.
  if (empty($query['facet'])) {
    unset($query['facet']);
  }

  $variables['url'] = url($variables['path'], array('query' => $query, 'absolute' => TRUE));
  $variables['label'] = $facet_item->getLabel();
  $variables['count'] = $facet_item->getTotalResults();

}

/**
 * Preproces a search facet item.
 * @see culturefeed-search-suggestions.tpl.php
 */
function culturefeed_search_ui_preprocess_culturefeed_search_suggestions(&$variables) {

}

/**
 * Theme the pager summary for a search result.
 */
function theme_culturefeed_search_pager_summary($variables) {

  $result = $variables['result'];

  $end = $variables['start'] + $result->getCurrentCount();
  $args = array(
    '@range' => ($variables['start'] + 1) . '-' . $end,
  );

  $pager_summary = format_plural($result->getTotalCount(), '@range van @count resultaat', '@range van @count resultaten', $args);

  return '<div class="pager-summary">' . $pager_summary . '</div>';

}
