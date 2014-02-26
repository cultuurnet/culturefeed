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
  $facets = $variables['items'];
  $sort_values = array();
  $sort = FALSE;

  // Add all the facet items to the build.
  foreach ($facets as $key => $facet_item) {

    $sub_item_output = array();
    $has_active_subitem = FALSE;

    if ($variables['sort'] == CULTUREFEED_SEARCH_SORT_ALPHABETICALLY) {
      $sort_values[] = $facet_item->getLabel();
      $sort = TRUE;
    }
    elseif ($variables['sort'] == CULTUREFEED_SEARCH_SORT_TOTAL_RESULTS) {
      $sort_values[] = $facet_item->getTotalResults();
      $sort = TRUE;
    }

    $sub_item_sort_values = array();
    // Add the subitems from this facet item.
    if ($facet_item->hasSubItems()) {

      $sub_items = $facet_item->getSubItems();

      // Create link for every sub item.
      foreach ($sub_items as $sub_item_key => $sub_item) {

        // Construct unique key for sorting.
        if ($variables['sort'] == CULTUREFEED_SEARCH_SORT_ALPHABETICALLY) {
          $sub_item_sort_values[] = $sub_item->getLabel();
        }
        elseif ($variables['sort'] == CULTUREFEED_SEARCH_SORT_TOTAL_RESULTS) {
          $sub_item_sort_values[] = $sub_item->getTotalResults();
        }

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

      // Sort based on the sorting values.
      if ($sort) {
        array_multisort($sub_item_sort_values, $sub_item_output);
        if ($variables['sort'] == CULTUREFEED_SEARCH_SORT_TOTAL_RESULTS) {
          $sub_item_output = array_reverse($sub_item_output);
          $sub_item_sort_values = array_reverse($sub_item_sort_values);
        }
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
      'subitem_sort_values' => $sub_item_sort_values,
    );

  }

  // Sort based on the sorting values.
  if ($sort) {
    array_multisort($sort_values, $items);
    if ($variables['sort'] == CULTUREFEED_SEARCH_SORT_TOTAL_RESULTS) {
      $items = array_reverse($items);
    }
  }

  $variables['items'] = $items;

}

/**
 * Preprocess function for the search facet.
 * @see culturefeed-search-facet.tpl.php
 */
function culturefeed_search_ui_preprocess_culturefeed_search_facet_eventtype(&$variables) {

  culturefeed_search_ui_preprocess_culturefeed_search_facet($variables);

  $sort_values = array();
  foreach ($variables['items'] as $item) {
    if (!empty($item['sub_items'])) {
      foreach ($item['sub_items'] as $subitem_key => $sub_item) {
        $variables['event_types'][] = $sub_item;
        $sort_values[] = $item['subitem_sort_values'][$subitem_key];
      }
    }
  }

  // Sort based on the sorting values.
  if ($variables['sort'] == CULTUREFEED_SEARCH_SORT_ALPHABETICALLY || $variables['sort'] == CULTUREFEED_SEARCH_SORT_TOTAL_RESULTS) {
    array_multisort($sort_values, $variables['event_types']);
    if ($variables['sort'] == CULTUREFEED_SEARCH_SORT_TOTAL_RESULTS) {
      $variables['event_types'] = array_reverse($variables['event_types']);
    }
  }
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

  // When the facet is datetype. We don't want to keep a date range filter when clicking on it.
  if ($facet->getKey() == 'datetype' && isset($query['date_range'])) {
    unset($query['date_range']);
  }

  // Location facet items are always active when location is in query string.
  if ($facet->getKey() == 'location' && isset($_GET['location'])) {
    $variables['active'] = TRUE;
    unset($query['location']);
    unset($query['distance']);
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
 * Theme the pager summary for a search result.
 */
function theme_culturefeed_search_pager_summary($variables) {

  $result = $variables['result'];

  $end = $variables['start'] + $result->getCurrentCount();
  $args = array(
    '@range' => ($variables['start'] + 1) . '-' . $end,
  );

  $pager_summary = format_plural($result->getTotalCount(), '@range from @count result', '@range from @count results', $args);

  return '<div class="pager-summary">' . $pager_summary . '</div>';

}
