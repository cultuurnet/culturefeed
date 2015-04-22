<?php
/**
 * @file
 * Helper functions for the culturefeed search ui.
 */

/**
 * Convert a given url to a youtube embed.
 */
function _culturefeed_search_ui_video_url_to_embed($url) {

  // Construct embed url.
  $embed_url = str_replace('watch', 'v', $url);

  $video_parts = explode('?v=', $embed_url);

  if (count($video_parts) < 2) {
    return FALSE;
  }
  $embed_url = $video_parts[0] . '/' . $video_parts[1];
  $embed_url .= '?fs=1&showinfo=0';

  $embed = '';
  $embed .= '<div class="embed-container">';
  $embed .= '<iframe src="' .  $embed_url . '" frameborder="0" allowfullscreen></iframe>';
  $embed .= '</div>';

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
      'range' => '[0 TO 2]',
    ),
    3 => array(
      'name' => '3-5 ' . t('year'),
      'query' => '(agefrom:[3 TO 5] OR keywords:"ook voor kinderen")',
      'range' => '[3 TO 5]',
    ),
    6 => array(
      'name' => '6-8 ' . t('year'),
      'query' => '(agefrom:[6 TO 8] OR keywords:"ook voor kinderen")',
      'range' => '[6 TO 8]',
    ),
    9 => array(
      'name' => '9-11 ' . t('year'),
      'query' => '(agefrom:[9 TO 11] OR keywords:"ook voor kinderen")',
      'range' => '[9 TO 11]',
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
    culturefeed_search_set_noindex_metatag();
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

  // Query is now search.
  if (isset($_GET['query'])) {
    $query = $_GET['query'];
    $new_query['search'] = $query;
  }

  // K is now keyword.
  if (isset($_GET['k'])) {
    $k = $_GET['k'];
    $new_query['keyword'] = $k;
  }

  // Datetype is now facet[datetype][0].
  if (isset($_GET['datetype'])) {
    $datetype = $_GET['datetype'];
    $new_query['facet']['datetype'][0] = $datetype;
  }

  // Date is now date_range.
  if (isset($_GET['date'])) {
    $date = $_GET['date'];
    $new_date = date("d/m/Y", strtotime($date));
    $new_query['date_range'] = $new_date;
  }
  
  // Headingid's
  if (isset($_GET['headingid'])) {
    
    $mapping = culturefeed_search_ui_get_headings_mapping($_GET['headingid']);
    
    // Voor kinderen is not a facet
    if (isset($mapping['voorkinderen'])) {
      $new_query['voor-kinderen'] = '1';
      unset($mapping['voorkinderen']);
    }
    foreach ($mapping as $category => $id) {
      $new_query['facet'][$category][0] = $id;
    }
  }

  if (!empty($new_query)) {
    drupal_goto(current_path(), array('query' => $new_query), 301);
  }

}


/**
 * Helper fucntion to redirect all old headingid's
 */
function culturefeed_search_ui_get_headings_mapping($headingid) {

  $mapping = array(
    44 => array('eventtype' => '0.50.4.0.0'),
    45 => array('eventtype' => '0.50.4.0.0','theme' => '1.8.1.0.0'),
    46 => array('eventtype' => '0.50.4.0.0','theme' => '1.8.2.0.0'),
    47 => array('eventtype' => '0.50.4.0.0','theme' => '1.8.3.1.0'),
    48 => array('eventtype' => '0.50.4.0.0','theme' => '1.8.3.2.0'),
    49 => array('eventtype' => '0.50.4.0.0','theme' => '1.8.3.3.0'),
    50 => array('eventtype' => '0.50.4.0.0','theme' => '1.8.4.0.0'),
    51 => array('eventtype' => '0.50.4.0.0','theme' => '1.8.3.5.0'),
    52 => array('eventtype' => '0.54.0.0.0'),
    53 => array('eventtype' => '0.54.0.0.0','theme' => '1.9.1.0.0'),
    54 => array('eventtype' => '0.54.0.0.0','theme' => '1.9.2.0.0'),
    55 => array('eventtype' => '0.54.0.0.0','theme' => '1.9.3.0.0'),
    56 => array('eventtype' => '0.54.0.0.0','theme' => '1.9.5.0.0'),
    57 => array('eventtype' => '0.55.0.0.0','theme' => '1.3.1.0.0 '),
    58 => array('eventtype' => '0.55.0.0.0','theme' => '1.3.1.0.0 '),
    59 => array('eventtype' => '0.55.0.0.0','theme' => '1.3.10.0.0'),
    60 => array('eventtype' => '0.55.0.0.0','theme' => '1.4.0.0.0'),
    61 => array('eventtype' => '0.55.0.0.0','theme' => '1.3.5.0.0'),
    62 => array('eventtype' => '0.55.0.0.0','theme' => '1.5.0.0.0'),
    63 => array('eventtype' => '0.55.0.0.0','theme' => '1.3.4.0.0'),
    64 => array('eventtype' => '0.50.6.0.0'),
    65 => array('eventtype' => '0.3.2.0.0'),
    66 => array('eventtype' => '0.5.0.0.0'),
    67 => array('eventtype' => '0.5.0.0.0','theme' => '1.8.1.0.0'),
    68 => array('eventtype' => '0.5.0.0.0','theme' => '1.3.1.0.0 '),
    69 => array('eventtype' => '0.5.0.0.0','theme' => '1.9.1.0.0'),
    70 => array('eventtype' => '0.5.0.0.0','theme' => '1.7.2.0.0'),
    71 => array('eventtype' => '0.5.0.0.0','theme' => '1.10.0.0.0'),
    73 => array('eventtype' => '0.19.0.0.0','theme' => '1.51.13.0.0'),
    74 => array('eventtype' => '0.0.0.0.0'),
    75 => array('eventtype' => '0.0.0.0.0','theme' => '1.1.0.0.0'),
    76 => array('eventtype' => '0.0.0.0.0','theme' => '1.0.2.0.0'),
    77 => array('eventtype' => '0.0.0.0.0','theme' => '1.0.1.0.0'),
    78 => array('eventtype' => '0.0.0.0.0','theme' => '1.0.3.0.0'),
    79 => array('eventtype' => '0.0.0.0.0','theme' => '1.2.1.0.0'),
    80 => array('eventtype' => '0.0.0.0.0','theme' => '1.2.2.0.0'),
    81 => array('eventtype' => '0.14.0.0.0'),
    82 => array('eventtype' => '0.15.0.0.0'),
    83 => array('eventtype' => '0.6.0.0.0'),
    84 => array('eventtype' => '0.37.0.0.0'),
    85 => array('eventtype' => '0.3.1.0.0'),
    86 => array('eventtype' => '0.50.21.0.0'),
    87 => array('eventtype' => '0.49.0.0.0'),
    89 => array('eventtype' => '0.53.0.0.0'),
    90 => array('eventtype' => '0.28.0.0.0'),
    91 => array('eventtype' => '0.7.0.0.0'),
    92 => array('eventtype' => '0.59.0.0.0'),
    93 => array('eventtype' => '0.17.0.0.0'),
    94 => array('umv' => 'umv.6','voorkinderen' => '1'),
    95 => array('eventtype' => '0.50.4.0.0','voorkinderen' => '1'),
    96 => array('eventtype' => '0.54.0.0.0','voorkinderen' => '1'),
    97 => array('eventtype' => '0.55.0.0.0','voorkinderen' => '1'),
    98 => array('eventtype' => '0.50.6.0.0','voorkinderen' => '1'),
    99 => array('umv' => 'umv.1','voorkinderen' => '1'),
    100 => array('eventtype' => '0.5.0.0.0','voorkinderen' => '1'),
    101 => array('umv' => 'umv.2','voorkinderen' => '1'),
    102 => array('umv' => 'umv.8','voorkinderen' => '1'),
    103 => array('umv' => 'umv.4','voorkinderen' => '1'),
    104 => array('umv' => 'umv.5','voorkinderen' => '1'),
    121 => array('eventtype' => '0.55.0.0.0','theme' => '1.4.0.0.0'),
    122 => array('eventtype' => '0.55.0.0.0','theme' => '1.5.0.0.0'),
    125 => array('eventtype' => '0.50.6.0.0','theme' => '1.7.2.0.0'),
    126 => array('eventtype' => '0.50.6.0.0','theme' => '1.7.12.0.0'),
    127 => array('eventtype' => '0.50.6.0.0','theme' => '1.7.1.0.0'),
    128 => array('eventtype' => '0.50.6.0.0','theme' => '1.7.6.0.0 '),
    129 => array('eventtype' => '0.50.6.0.0','theme' => '1.7.8.0.0'),
    130 => array('eventtype' => '0.50.6.0.0','theme' => '1.7.3.0.0'),
    131 => array('eventtype' => '0.50.6.0.0','theme' => '1.7.13.0.0'),
    132 => array('eventtype' => '0.50.6.0.0','theme' => '1.7.10.0.0'),
    133 => array('eventtype' => '0.50.6.0.0','theme' => '1.7.4.0.0'),
    134 => array('eventtype' => '0.50.6.0.0','theme' => '1.7.7.0.0'),
    135 => array('eventtype' => '0.50.6.0.0','theme' => '1.7.11.0.0'),
    136 => array('eventtype' => '0.50.6.0.0','theme' => '1.7.14.0.0'),
    137 => array('eventtype' => '0.3.2.0.0'),
    138 => array('eventtype' => '0.3.2.0.0'),
    139 => array('eventtype' => '0.19.0.0.0','theme' => '1.51.13.0.0'),
    140 => array('eventtype' => '0.19.0.0.0','theme' => '1.51.3.0.0'),
    141 => array('eventtype' => '0.19.0.0.0','theme' => '1.51.6.0.0'),
    142 => array('eventtype' => '0.19.0.0.0','theme' => '1.51.11.0.0'),
    143 => array('eventtype' => '0.19.0.0.0','theme' => '1.58.8.0.0'),
    144 => array('eventtype' => '0.19.0.0.0','theme' => '1.51.10.0.0'),
    145 => array('eventtype' => '0.19.0.0.0','theme' => '1.51.12.0.0'),
    146 => array('eventtype' => '0.19.0.0.0','theme' => '1.51.14.0.0'),
    147 => array('eventtype' => '0.56.0.0.0'),
    148 => array('eventtype' => '0.0.0.0.0','theme' => '1.0.5.0.0'),
    149 => array('eventtype' => '0.0.0.0.0','theme' => '1.0.9.0.0'),
    150 => array('eventtype' => '0.15.0.0.0'),
    151 => array('eventtype' => '0.16.0.0.0'),
    152 => array('eventtype' => '0.12.0.0.0'),
    153 => array('eventtype' => '0.3.1.0.0','theme' => '1.42.0.0.0'),
    154 => array('eventtype' => '0.3.1.0.0','theme' => '1.21.0.0.0'),
    155 => array('eventtype' => '0.3.1.0.0','theme' => '1.37.0.0.0'),
    156 => array('eventtype' => '0.3.1.0.0','theme' => '1.61.0.0.0'),
    157 => array('eventtype' => '0.3.1.0.0','theme' => '1.43.0.0.0'),
    158 => array('eventtype' => '0.3.1.0.0','theme' => '1.41.0.0.0'),
    159 => array('eventtype' => '0.3.1.0.0','theme' => '1.37.1.0.0'),
    160 => array('eventtype' => '0.3.1.0.0','theme' => '1.37.2.0.0'),
    161 => array('eventtype' => '0.3.1.0.0','theme' => '1.44.0.0.0'),
    162 => array('eventtype' => '0.3.1.0.0','theme' => '1.64.0.0.0'),
    163 => array('eventtype' => '0.3.1.0.0','theme' => '1.25.0.0.0'),
    164 => array('eventtype' => '0.3.1.0.0','theme' => '1.51.13.0.0'),
    165 => array('eventtype' => '0.57.0.0.0'),
    166 => array('eventtype' => '1.50.0.0.0'),
    167 => array('eventtype' => '0.59.0.0.0','theme' => '1.51.13.0.0'),
    168 => array('eventtype' => '0.59.0.0.0','theme' => '1.51.14.0.0'),
    169 => array('eventtype' => '0.59.0.0.0','theme' => '1.51.3.0.0'),
    170 => array('eventtype' => '0.59.0.0.0','theme' => '1.51.6.0.0'),
    171 => array('eventtype' => '0.59.0.0.0','theme' => '1.51.11.0.0'),
    172 => array('eventtype' => '0.59.0.0.0','theme' => '1.58.8.0.0'),
    173 => array('eventtype' => '0.59.0.0.0','theme' => '1.51.10.0.0'),
    174 => array('eventtype' => '0.59.0.0.0','theme' => '1.51.12.0.0'),
    175 => array('umv' => 'umv.9','voorkinderen' => '1'),
    176 => array('eventtype' => '0.100.0.0.0'),
    177 => array('eventtype' => '0.100.2.0.0'),
    178 => array('eventtype' => '0.100.1.0.0'),
    179 => array('voorkinderen' => '1'),
    205 => array('eventtype' => '0.50.6.0.0','theme' => '1.7.15.0.0'),
    206 => array('eventtype' => '0.3.1.0.0','theme' => '1.65.0.0.0')
  );

  return $mapping[$headingid];

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
 * Get the search page that's currently shown.
 */
function culturefeed_search_get_active_search_page() {
  if ($type = culturefeed_get_searchable_type_by_path()) {
    return culturefeed_get_search_page($type);
  }
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
    $pages[$class_name]->setGroup(variable_get('culturefeed_search_group', 'event_production'));
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

/**
 * Add facebook JS share if an app id is configured.
 */
function culturefeed_search_ui_add_facebook_share() {

  $fb_app_id = variable_get('culturefeed_search_ui_fb_app_id', '');
  if (!empty($fb_app_id)) {
    drupal_add_js(drupal_get_path('module', 'culturefeed') . '/js/facebook_share.js');
    drupal_add_js(array('culturefeed' => array('fbAppId' => $fb_app_id)), 'setting');
  }

}

/**
 * Get the user location cookie.
 */
function culturefeed_search_ui_get_user_location_cookie() {

  if (!empty($_COOKIE['Drupal_visitor_uitid_userLocation'])) {
    $user_location = json_decode($_COOKIE['Drupal_visitor_uitid_userLocation']);
    if ($user_location instanceof stdClass) {
      return $user_location;
    }
  }

  return new stdClass();

}
