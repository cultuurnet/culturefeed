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

}

/**
 * Preprocess function for the search list.
 *
 * @param array $vars
 */
function culturefeed_search_ui_preprocess_culturefeed_search_list(&$vars) {

  $items = $vars['result']->getItems();

  $vars['noitems'] = count($items) <= 0 ? TRUE : FALSE;

  foreach ($items as $item) {
    $entity = $item->getEntity();
    if ($entity instanceof CultureFeed_Cdb_Item_Event) {
      $vars['items'][] = theme('culturefeed_event_summary', array('item' => $item));
    }

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

  $items = array();
  foreach ($facet->getResult()->getItems() as $facet_item) {

    $sub_item_output = array();
    // Add the subitems from this facet item.
    if ($facet_item->hasSubItems()) {
      $sub_items = $facet_item->getSubItems();
      foreach ($sub_items as $sub_item) {
        $sub_item_output[] = theme('culturefeed_search_facet_item', array(
          'facet_item' => $sub_item,
          'facet' => $facet,
          'query' => $query,
          'path' => $path,
        ));
      }
    }

    // Theme the item.
    $item_output = theme('culturefeed_search_facet_item', array(
      'facet_item' => $facet_item,
      'facet' => $facet,
      'query' => $query,
      'path' => $path,
    ));

    // Add it to the list.
    $items[] = array(
      'output' => $item_output,
      'sub_items' => $sub_item_output,
    );

  }

  $variables['items'] = $items;

}

/**
 * Preproces a search facet item.
 * @see culturefeed-search-facet-item.tpl.php
 */
function culturefeed_search_ui_preprocess_culturefeed_search_facet_item(&$variables) {

  $facet = $variables['facet'];
  $facet_item = $variables['facet_item'];
  $query = $variables['query'];

  $facet_option = $facet_item->getLabel();
  $active_options = isset($query['facet'][$facet->getKey()]) ? $query['facet'][$facet->getKey()] : array();

  $variables['active'] = in_array($facet_option, $active_options);
  if ($variables['active']) {
    // Remove this facet from URL, URL will function as removal URL.
    $key = array_search($facet_option, $query['facet'][$facet->getKey()]);
    if (FALSE !== $key) {
      unset($query['facet'][$facet->getKey()][$key]);
      if (!$query['facet'][$facet->getKey()]) {
        unset($query['facet'][$facet->getKey()]);
      }
    }
  }
  else {
    // Add this facet to URL, URL will function as additional filter URL.
    $query['facet'][$facet->getKey()][] = $facet_option;
  }

  // Remove facet completely if nothing left.
  if (empty($query['facet'])) {
    unset($query['facet']);
  }

  $variables['url'] = url($variables['path'], array('query' => $query, 'absolute' => TRUE));
  $variables['name'] = $facet_option;
  $variables['count'] = $facet_item->getTotalResults();

}
