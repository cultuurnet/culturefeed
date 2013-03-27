<?php
/**
 * @file
 * Theme / preprocess functions for culturefeed agenda.
 */

/**
 * Preprocess the variables for the event summary.
 * @see culturefeed-event-summary.tpl.php
 */
function template_preprocess_culturefeed_event_summary(&$variables) {

  _template_preprocess_culturefeed_agenda($variables);

}

/**
 * Preprocess the variables for the event detail.
 * @see culturefeed-event.tpl.php
 */
function template_preprocess_culturefeed_event(&$variables) {

  _template_preprocess_culturefeed_agenda($variables);

}

/**
 * Helper preprocess function to preprocess the general agenda variables.
 */
function _template_preprocess_culturefeed_agenda(&$variables) {

  global $language;

  $item = $variables['item'];

  $cdb_item = $item->getEntity();
  //$details = $cdb_item->getDetails()->getDetailByLanguage($language->language);
  $details = $cdb_item->getDetailByLanguage($language->language);

  $variables['title'] = $details->getTitle();

}

/**
 * Preprocess function for the search page main section.
 *
 * @param array $vars
 */
function culturefeed_agenda_preprocess_culturefeed_search_page(&$vars) {
  
  // Prepare the sortorder widget.
  if (!empty($vars['sortorder_form'])) {
    $vars['sortorder_widget'] = drupal_render($vars['sortorder_form']);
  }
  
}

/**
 * Theme function for the facets list.
 * 
 * @param array $variables
 *   \CultuurNet\Search\Component\Facet\Facet $facet, 
 *   String $path = NULL, 
 *   String $query = NULL
 */
function theme_culturefeed_search_facets_list($variables) {
  
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
  
  $facetListItems = array();
  
  foreach ($facet->getResult()->getItems() as $option => $number) {
    $facet_query = $query;
  
    $selected = in_array($option, $active_options);
  
    if ($selected) {
      // Remove this facet from URL, URL will function as removal URL.
      $key = array_search($option, $facet_query['facet'][$facet->getKey()]);
      if (FALSE !== $key) {
        unset($facet_query['facet'][$facet->getKey()][$key]);
      }
    }
    else {
      // Add this facet to URL, URL will function as additional filter URL.
      $facet_query['facet'][$facet->getKey()][] = $option;
    }
  
    $url = url($path, array('query' => $facet_query, 'absolute' => TRUE));
  
    $facetListItems[] = theme('culturefeed_search_facet', array(
      'name' => $option,
      'count' => $number,
      'active' => $selected,
      'url' => $url,
    ));
  }
  
  return implode('', $facetListItems);
  
}
