<?php
/**
 * @file
 * Helper functions for the culturefeed search ui.
 */

/**
 * Convert a given url to a youtube embed.
 */
function _culturefeed_search_ui_video_url_to_embed($url, $width, $height) {

  // Construct embed url.
  $embed_url = str_replace('watch', 'v', $url);

  $video_parts = explode('?v=', $embed_url);

  if (count($video_parts) < 2) {
    return FALSE;
  }
  $embed_url = $video_parts[0] . '/' . $video_parts[1];
  $embed_url .= '?fs=1&showinfo=0';

  $embed = '';
  $embed .= '<object width="' . $width . '" height="' . $height . '">';
  $embed .= '<param name="movie" value="' .  $embed_url . '"></param>';
  $embed .= '<param name="allowFullScreen" value="true"></param>';
  $embed .= '<param name="wmode" value="transparent"></param>';
  $embed .= '<embed src="' .  $embed_url . '"';
  $embed .= 'type="application/x-shockwave-flash" width="' . $width . '" height="' . $height . '" allowfullscreen="true" wmode="transparent">';
  $embed .= '</embed>';
  $embed .= '</object>';

  return $embed;

}

/**
 * Get the key value from the currently active search page.
 */
function _culturefeed_search_ui_get_active_search_page() {

  $query = drupal_get_query_parameters(NULL, array('q', 'page'));
  $searchable_types = culturefeed_get_searchable_types();

  foreach ($searchable_types as $key => $type) {

    // Check if this is the current page.
    if ($type['path'] == $_GET['q'] || $type['path'] . '/nojs' == $_GET['q']) {

      // If this page has active facets in the info definition. Check if all the facets matches.
      if (!empty($type['active_facets']) && !empty($query['facet'])) {
        $total_requested = count($type['active_facets']);
        $total_matches = 0;
        // Loop through the requested facets, and check if this is active in current search.
        foreach ($type['active_facets'] as $requested_facet => $requested_values) {

          // If the requested facet is active. Calculate the intersection, and check if all requested values are in the current page facets.
          if (isset($query['facet'][$requested_facet])) {
            $matches = array_intersect($requested_values, $query['facet'][$requested_facet]);
            if (count($matches) == count($requested_values)) {
              $total_matches++;
            }
          }
        }

        // If all the requested facets are found, this type should be default.
        if ($total_matches == $total_requested) {
          return $key;
        }

      }
      else {
        return $key;
      }

    }
  }

  return NULL;

}

/**
 * Get the allowed date facet options.
 */
function culturefeed_search_ui_get_date_facet_options() {

  return array(
    'today' => t('Today'),
    'tomorrow' => t('Tomorrow'),
    'next7days' => t('Next 7 days'),
    'thisweekend' => t('This weekend'),
    'next14days' => t('Next 14 days'),
    'next30days' => t('Next 30 days'),
  );

}

/**
 * Get the allowed age range facet options.
 */
function culturefeed_search_ui_get_age_range_facet_options() {
  return array(
    0 => array(
      'name' => '0-2 ' . t('year'),
      'query' => '(agefrom:[0 TO 2] OR keywords:"ook voor kinderen")',
    ),
    3 => array(
      'name' => '3-5 ' . t('year'),
      'query' => '(agefrom:[3 TO 5] OR keywords:"ook voor kinderen")',
    ),
    6 => array(
      'name' => '6-8 ' . t('year'),
      'query' => '(agefrom:[6 TO 8] OR keywords:"ook voor kinderen")',
    ),
    9 => array(
      'name' => '9-11 ' . t('year'),
      'query' => '(agefrom:[9 TO 11] OR keywords:"ook voor kinderen")',
    ),
  );
}

/**
 * Set the correct metatags for search pages.
 */
function culturefeed_search_ui_set_noindex_metatag() {

  // Default noindex should be shown.
  $noindex = TRUE;

  $query = drupal_get_query_parameters(NULL, array('q'));
  // Start page should be indexed.
  if (empty($query)) {
    $noindex = FALSE;
  }
  else {

    // No index is FALSE, if user is filtering on event type.
    if (isset($query['facet']['category_eventtype_id'])) {

      $facets = $query['facet'];
      unset($facets['category_eventtype_id']);
      unset($facets['category_theme_id']);

      unset($query['location']);
      unset($query['regId']);
      unset($query['facet']);

      // If theme_id and/or regId was the only extra facet / parameters, set noindex to false.
      if (count($facets) == 0 && count($query) == 0) {
        $noindex = FALSE;
      }

    }

    // No index is FALSE, if user is filtering on region Id without additional facets.
    elseif ((isset($query['regId']) || isset($query['location'])) && !isset($query['facet'])) {

      unset($query['location']);
      unset($query['regId']);

      // If regId was the only filter set noindex to false.
      if (count($query) == 0) {
        $noindex = FALSE;
      }

    }

  }

  // Add the "noindex, follow" meta tag if needed.
  if ($noindex) {
    culturefeed_set_noindex_metatag();
  }

}

/**
 * Check if current url is a cnapi version. If it is, redirect to
 */
function culturefeed_search_ui_redirect_cnapi_urls() {

  $new_query = array();

  // Check if we are on a searchable page.
  $current_search_page = culturefeed_get_searchable_type_by_path();
  if (!$current_search_page) {
    return;
  }

  // Regio is now location.
  if (isset($_GET['regio'])) {
    $region = db_query('SELECT name FROM {culturefeed_search_terms} WHERE tid = :tid', array(':tid' => 'reg.' . $_GET['regio']))->fetchField();
    if ($region) {
      $new_query['location'] = $region;
    }
  }

  // City id is now location.
  if (isset($_GET['cityid'])) {
    $result = db_query('SELECT name, zip FROM {culturefeed_search_cities} WHERE cid = :cityid', array(':cityid' => $_GET['cityid']))->fetchObject();
    if ($result) {
      $new_query['location'] = $result->zip . ' ' . $result->name;
    }
  }

  if (!empty($new_query)) {
    drupal_goto(current_path(), array('query' => $new_query), 301);
  }

}

/**
 * Load all searchable types.
 * This wil invoke a hook. Modules can define following stuff.
 *  return array(
 *   'activiteiten' => array(
 *     'id' => 'activiteiten',
 *     'path' => 'agenda/search',
 *     'page_title' => 'Agenda',
 *     'search_title' => 'Activiteiten',
 *     'class_name' => 'CultureFeedAgendaPage',
 *     'sort_default' => 'relevancy',
 *     'sort_options' => array(
 *       'relevancy' => array(
 *         'label' => 'Relevantie',
 *         'exposed' => TRUE
 *         'query' => 'weight asc'
 *       ),
 *       'date' => array(
 *         'label' => 'Date',
 *         'exposed' => TRUE
 *         'query' => 'startdateday asc'
 *       ),
 *     ),
 *     'active_facets' => array('category_actortype_name' => array('Organisator(en)')),  => optional
 *   ),
 * );
 */
function culturefeed_get_searchable_types() {

  $options = drupal_static('culturefeed_searchable_types');
  if ($options !== NULL) {
    return $options;
  }

  // Get all searchable page types.
  $options = module_invoke_all('culturefeed_search_page_info');

  foreach ($options as $key => $data) {
    if (!isset($data['class_name'])) {
      $options[$key]['class_name'] = 'CultureFeedSearchPage';
    }
  }

  // Make sure you can alter the searchable data per site.
  drupal_alter('culturefeed_search_page_info', $options);

  return $options;
}

/**
 * Load a searchable type.
 * @param String $name
 * @return Array searchable type.
 */
function culturefeed_get_searchable_type($name) {
  $options = culturefeed_get_searchable_types();
  return isset($options[$name]) ? $options[$name] : NULL;
}

/**
 * Returns a searchable type by path.
 *
 * @param string $path
 *   An optional menu path. Defaults to the current path.
 *
 * @return array
 *   The requested searchable type, or FALSE if no searchable type matches the
 *   path.
 */
function culturefeed_get_searchable_type_by_path($path = '') {
  if (!$path) {
    $menu_item = menu_get_item();
    $path = $menu_item['path'];
  }
  foreach (culturefeed_get_searchable_types() as $name => $type) {
    if ($type['path'] == $path) {
      $type['name'] = $name;
      return $type;
    }
  }
  return FALSE;
}

/**
 * Returns an instance of a search page.
 *
 * @param array $search_type
 *   A searchable type, as supplied by hook_culturefeed_searchable_types().
 *
 * @return CultureFeedSearchPage
 *   An instance of the requested search page.
 */
function culturefeed_get_search_page($search_type) {
  $pages = &drupal_static(__FUNCTION__);
  $class_name = $search_type['class_name'];
  if (!isset($pages[$class_name])) {
    $pages[$class_name] = new $search_type['class_name']();
    $pages[$class_name]->setResultsPerPage(variable_get('culturefeed_search_ui_pager_items_per_page', CULTUREFEED_SEARCH_UI_PAGER_ITEMS_PER_PAGE));
    $pages[$class_name]->setDefaultSort($search_type['sort_default']);
    $pages[$class_name]->setGroup(variable_get('culturefeed_search_group', TRUE));
    $pages[$class_name]->initialize();
  }
  return $pages[$class_name];
}

/**
 * Returns the search facets for the current page.
 *
 * @return \CultuurNet\Search\Component\Facet\FacetComponent
 *   The search facets for the current page or FALSE if the current page is not
 *   a search page.
 */
function culturefeed_get_facet_component() {
  if ($type = culturefeed_get_searchable_type_by_path()) {
    $page = culturefeed_get_search_page($type);
    return $page->getFacetComponent();
  }
  return FALSE;
}

/**
 * Returns the search result for the current page.
 *
 * @return \CultuurNet\Search\SearchResult
 *   The search result for the current page or FALSE if the current page is not
 *   a search page.
 */
function culturefeed_get_search_result() {
  if ($type = culturefeed_get_searchable_type_by_path()) {
    $page = culturefeed_get_search_page($type);
    return $page->getSearchResult();
  }
  return FALSE;
}

/**
 * Return the default options for the filter form.
 */
function culturefeed_search_ui_default_filter_options() {

  return array(
    array(
      'exposed' => TRUE,
      'title' => 'Free only',
      'query-string' => 'free-only',
      'api-filter-query' => 'price:0'
    ),
    array(
      'exposed' => TRUE,
      'title' => 'No courses and workshops',
      'query-string' => 'no-courses-workshops',
      'api-filter-query' => '!category_id:0.3.1.0.0'
    ),
    array(
      'exposed' => TRUE,
      'title' => 'For toddlers',
      'query-string' => 'for-toddlers',
      'api-filter-query' => 'agefrom:(3 OR 4 OR 5)'
    ),
  );
}

/**
 * Get the sort options of a culturefeed search page.
 */
function culturefeed_search_ui_get_sort_options_for_page($search_page) {

  $sort_options = variable_get('culturefeed_search_sortoptions_' . $search_page['id'], array());

  // Load default ones.
  if (empty($sort_options)) {
    $sort_options = $search_page['sort_options'];
    $sort_options['default'] = $search_page['sort_default'];
  }

  return $sort_options;

}
