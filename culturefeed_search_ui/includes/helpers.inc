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
  $embed_url = str_replace('watch', 'embed', $url);

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
 * Strips a given url to a youtube id.
 */

function _culturefeed_search_ui_video_url_to_id($url) {

  // Construct embed url.
  $embed_url = str_replace('watch', 'embed', $url);

  $video_parts = explode('?v=', $embed_url);

  if (count($video_parts) < 2) {
    return FALSE;
  }

  return  $video_parts[1];

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
 * Put noindex and canonical metatags for search pages.
 */
function culturefeed_search_ui_set_canonical_metatag() {

  $canonical = FALSE;

  $query = drupal_get_query_parameters(NULL, array('q'));
  // If user is filtering on actor type, unset redundant facets for canonical metatag
  if (isset($query['facet']['category_actortype_id'])) {

    if (isset($query['facet']['type']) || isset($query['actors'])) {
      $canonical = array();

      $facets = $query['facet'];
      unset($facets['type']);
      unset($query['actors']);

      $canonical['path'] = 'agenda/search';
      $canonical['options']['query']['facet'] = $facets;
    }

  }

  // Add the rel="canonical" tag if needed.
  if ($canonical) {
    $canonical['options']['absolute'] = TRUE;
    drupal_add_html_head_link(array('rel' => 'canonical', 'href' => url($canonical['path'], $canonical['options'])), TRUE);
  }

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

    // No index is FALSE, if user is filtering on entity type actor.
    if (isset($query['facet']['type'])) {

      $facets = $query['facet'];
      if ($query['facet']['type'][0] == 'actor') {
        unset($facets['type']);
        if (isset($query['facet']['category_flandersregion_id'])) {
          if (count($query['facet']['category_flandersregion_id']) <= 1) {
            unset($facets['category_flandersregion_id']);
          }
        }
      }

      // If type=actor is the only parameter with flandersregion, set noindex to false.
      if (count($facets) == 0) {
        $noindex = FALSE;
      }

    }

    // No index is FALSE, if user is filtering on actor type.
    if (isset($query['facet']['category_actortype_id'])) {

      $facets = $query['facet'];
      unset($facets['category_actortype_id']);
      unset($facets['type']);

      if (isset($query['facet']['category_flandersregion_id'])) {
        if (count($query['facet']['category_flandersregion_id']) <= 1) {
          unset($facets['category_flandersregion_id']);
        }
      }

      unset($query['location']);
      unset($query['regId']);
      unset($query['actors']);
      unset($query['facet']);

      // If type, flandersregion, location and/or regId was the only extra facet / parameters, set noindex to false.
      if (count($facets) == 0 && count($query) == 0) {
        $noindex = FALSE;
      }

    }

    // No index is FALSE, if user is filtering on event type.
    elseif (isset($query['facet']['category_eventtype_id'])) {

      if (count($query['facet']['category_eventtype_id']) == 1) {

        $facets = $query['facet'];
        unset($facets['category_eventtype_id']);

        if (isset($query['facet']['category_theme_id'])) {
          if (count($query['facet']['category_theme_id']) <= 1) {
            unset($facets['category_theme_id']);
          }
        }

        if (isset($query['facet']['category_flandersregion_id'])) {
          if (count($query['facet']['category_flandersregion_id']) <= 1) {
            unset($facets['category_flandersregion_id']);
          }
        }

        unset($query['location']);
        unset($query['regId']);
        unset($query['facet']);

        // If theme_id, flandersregion and/or regId was the only extra facet / parameters, set noindex to false.
        if (count($facets) == 0 && count($query) == 0) {
          $noindex = FALSE;
        }

      }

    }

    // No index is FALSE, if user is filtering on umv category and/or region.
    elseif (isset($query['facet']['category_umv_id'])) {

      if (count($query['facet']['category_umv_id']) == 1) {

        $facets = $query['facet'];
        unset($facets['category_umv_id']);

        if (isset($query['facet']['category_flandersregion_id'])) {
          if (count($query['facet']['category_flandersregion_id']) <= 1) {
            unset($facets['category_flandersregion_id']);
          }
        }

        unset($query['location']);
        unset($query['regId']);
        unset($query['facet']);

        // If flandersregion or regId was the only extra facet / parameters, set noindex to false.
        if (count($facets) == 0 && count($query) == 0) {
          $noindex = FALSE;
        }

      }

    }

    // No index is FALSE, if user is filtering on flandersregion Id.
    elseif (isset($query['facet']['category_flandersregion_id'])) {

      if(count($query['facet']['category_flandersregion_id']) == 1) {

        $facets = $query['facet'];
        unset($facets['category_flandersregion_id']);
        unset($query['facet']);

        // If flandersregion Id was the only filter set noindex to false.
        if (count($facets) == 0 && count($query) == 0) {
          $noindex = FALSE;
        }

      }

    }

    // No index is FALSE, if user is filtering on location or region Id without additional facets.
    elseif (isset($query['regId']) || isset($query['location'])) {

      unset($query['location']);
      unset($query['regId']);

      // If location or regId was the only filter set noindex to false.
      if (count($query) == 0) {
        $noindex = FALSE;
      }

    }

    // No index is FALSE, if user is filtering on voor-kinderen.
    if (isset($query['voor-kinderen'])) {

      unset($query['voor-kinderen']);

      // Unset all facets that can be indexed in combination with voor-kinderen
      if (isset($query['facet'])) {
        $facets = $query['facet'];
        unset($facets['category_umv_id']);
        unset($facets['category_flandersregion_id']);
        unset($facets['category_eventtype_id']);
        unset($facets['category_theme_id']);
        unset($query['facet']);
      }

      // If voor-kinderen was the only filter set noindex to false.
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

  // City can be mapped to location.
  if (isset($_GET['city'])) {
    $query = $_GET['city'];
    $new_query['location'] = $query;
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

  // Headings are now a combination of facets.
  if (isset($_GET['heading'])) {

    if (strpos($_GET['heading'], ';') !== false) {
      $heading = explode(';', $_GET['heading']);
      if ($heading[0] !== '') {
        $heading = $heading[0];
      }
      else {
        $heading = $heading[1];
      }
    }
    else {
      $heading = $_GET['heading'];
    }

    $mapping = culturefeed_search_ui_get_headings_mapping($heading);

    // Voor kinderen is not a facet
    if (isset($mapping['voor_kinderen'])) {
      $new_query['voor-kinderen'] = '1';
      unset($mapping['voor_kinderen']);
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
function culturefeed_search_ui_get_headings_mapping($heading) {

  $mapping = array(
    44 => array('category_eventtype_id' => '0.50.4.0.0'),
    45 => array('category_eventtype_id' => '0.50.4.0.0','category_theme_id' => '1.8.1.0.0'),
    46 => array('category_eventtype_id' => '0.50.4.0.0','category_theme_id' => '1.8.2.0.0'),
    47 => array('category_eventtype_id' => '0.50.4.0.0','category_theme_id' => '1.8.3.1.0'),
    48 => array('category_eventtype_id' => '0.50.4.0.0','category_theme_id' => '1.8.3.2.0'),
    49 => array('category_eventtype_id' => '0.50.4.0.0','category_theme_id' => '1.8.3.3.0'),
    50 => array('category_eventtype_id' => '0.50.4.0.0','category_theme_id' => '1.8.4.0.0'),
    51 => array('category_eventtype_id' => '0.50.4.0.0','category_theme_id' => '1.8.3.5.0'),
    52 => array('category_eventtype_id' => '0.54.0.0.0'),
    53 => array('category_eventtype_id' => '0.54.0.0.0','category_theme_id' => '1.9.1.0.0'),
    54 => array('category_eventtype_id' => '0.54.0.0.0','category_theme_id' => '1.9.2.0.0'),
    55 => array('category_eventtype_id' => '0.54.0.0.0','category_theme_id' => '1.9.3.0.0'),
    56 => array('category_eventtype_id' => '0.54.0.0.0','category_theme_id' => '1.9.5.0.0'),
    57 => array('category_eventtype_id' => '0.55.0.0.0','category_theme_id' => '1.3.1.0.0'),
    58 => array('category_eventtype_id' => '0.55.0.0.0','category_theme_id' => '1.3.1.0.0'),
    59 => array('category_eventtype_id' => '0.55.0.0.0','category_theme_id' => '1.3.10.0.0'),
    60 => array('category_eventtype_id' => '0.55.0.0.0','category_theme_id' => '1.4.0.0.0'),
    61 => array('category_eventtype_id' => '0.55.0.0.0','category_theme_id' => '1.3.5.0.0'),
    62 => array('category_eventtype_id' => '0.55.0.0.0','category_theme_id' => '1.5.0.0.0'),
    63 => array('category_eventtype_id' => '0.55.0.0.0','category_theme_id' => '1.3.4.0.0'),
    64 => array('category_eventtype_id' => '0.50.6.0.0'),
    65 => array('category_eventtype_id' => '0.3.2.0.0'),
    66 => array('category_eventtype_id' => '0.5.0.0.0'),
    67 => array('category_eventtype_id' => '0.5.0.0.0','category_theme_id' => '1.8.1.0.0'),
    68 => array('category_eventtype_id' => '0.5.0.0.0','category_theme_id' => '1.3.1.0.0 '),
    69 => array('category_eventtype_id' => '0.5.0.0.0','category_theme_id' => '1.9.1.0.0'),
    70 => array('category_eventtype_id' => '0.5.0.0.0','category_theme_id' => '1.7.2.0.0'),
    71 => array('category_eventtype_id' => '0.5.0.0.0','category_theme_id' => '1.10.0.0.0'),
    73 => array('category_eventtype_id' => '0.19.0.0.0','category_theme_id' => '1.51.13.0.0'),
    74 => array('category_eventtype_id' => '0.0.0.0.0'),
    75 => array('category_eventtype_id' => '0.0.0.0.0','category_theme_id' => '1.1.0.0.0'),
    76 => array('category_eventtype_id' => '0.0.0.0.0','category_theme_id' => '1.0.2.0.0'),
    77 => array('category_eventtype_id' => '0.0.0.0.0','category_theme_id' => '1.0.1.0.0'),
    78 => array('category_eventtype_id' => '0.0.0.0.0','category_theme_id' => '1.0.3.0.0'),
    79 => array('category_eventtype_id' => '0.0.0.0.0','category_theme_id' => '1.2.1.0.0'),
    80 => array('category_eventtype_id' => '0.0.0.0.0','category_theme_id' => '1.2.2.0.0'),
    81 => array('category_eventtype_id' => '0.14.0.0.0'),
    82 => array('category_eventtype_id' => '0.15.0.0.0'),
    83 => array('category_eventtype_id' => '0.6.0.0.0'),
    84 => array('category_eventtype_id' => '0.37.0.0.0'),
    85 => array('category_eventtype_id' => '0.3.1.0.0'),
    86 => array('category_eventtype_id' => '0.50.21.0.0'),
    87 => array('category_eventtype_id' => '0.49.0.0.0'),
    89 => array('category_eventtype_id' => '0.53.0.0.0'),
    90 => array('category_eventtype_id' => '0.28.0.0.0'),
    91 => array('category_eventtype_id' => '0.7.0.0.0'),
    92 => array('category_eventtype_id' => '0.59.0.0.0'),
    93 => array('category_eventtype_id' => '0.17.0.0.0'),
    94 => array('category_umv_id' => 'umv.6','voor_kinderen' => '1'),
    95 => array('category_eventtype_id' => '0.50.4.0.0','voor_kinderen' => '1'),
    96 => array('category_eventtype_id' => '0.54.0.0.0','voor_kinderen' => '1'),
    97 => array('category_eventtype_id' => '0.55.0.0.0','voor_kinderen' => '1'),
    98 => array('category_eventtype_id' => '0.50.6.0.0','voor_kinderen' => '1'),
    99 => array('category_umv_id' => 'umv.1','voor_kinderen' => '1'),
    100 => array('category_eventtype_id' => '0.5.0.0.0','voor_kinderen' => '1'),
    101 => array('category_umv_id' => 'umv.2','voor_kinderen' => '1'),
    102 => array('category_umv_id' => 'umv.8','voor_kinderen' => '1'),
    103 => array('category_umv_id' => 'umv.4','voor_kinderen' => '1'),
    104 => array('category_umv_id' => 'umv.5','voor_kinderen' => '1'),
    105 => array('category_umv_id' => 'umv.2','voor_kinderen' => '1'),
    107 => array('category_umv_id' => 'umv.7','voor_kinderen' => '1'),
    108 => array('category_umv_id' => 'umv.1','voor_kinderen' => '1'),
    109 => array('category_umv_id' => 'umv.2','voor_kinderen' => '1'),
    110 => array('category_umv_id' => 'umv.2','voor_kinderen' => '1'),
    111 => array('category_umv_id' => 'umv.7','voor_kinderen' => '1'),
    112 => array('category_umv_id' => 'umv.5','voor_kinderen' => '1'),
    121 => array('category_eventtype_id' => '0.55.0.0.0','category_theme_id' => '1.4.0.0.0'),
    122 => array('category_eventtype_id' => '0.55.0.0.0','category_theme_id' => '1.5.0.0.0'),
    123 => array('category_umv_id' => 'umv.9','voor_kinderen' => '1'),
    124 => array('category_umv_id' => 'umv.8','voor_kinderen' => '1'),
    125 => array('category_eventtype_id' => '0.50.6.0.0','category_theme_id' => '1.7.2.0.0'),
    126 => array('category_eventtype_id' => '0.50.6.0.0','category_theme_id' => '1.7.12.0.0'),
    127 => array('category_eventtype_id' => '0.50.6.0.0','category_theme_id' => '1.7.1.0.0'),
    128 => array('category_eventtype_id' => '0.50.6.0.0','category_theme_id' => '1.7.6.0.0 '),
    129 => array('category_eventtype_id' => '0.50.6.0.0','category_theme_id' => '1.7.8.0.0'),
    130 => array('category_eventtype_id' => '0.50.6.0.0','category_theme_id' => '1.7.3.0.0'),
    131 => array('category_eventtype_id' => '0.50.6.0.0','category_theme_id' => '1.7.13.0.0'),
    132 => array('category_eventtype_id' => '0.50.6.0.0','category_theme_id' => '1.7.10.0.0'),
    133 => array('category_eventtype_id' => '0.50.6.0.0','category_theme_id' => '1.7.4.0.0'),
    134 => array('category_eventtype_id' => '0.50.6.0.0','category_theme_id' => '1.7.7.0.0'),
    135 => array('category_eventtype_id' => '0.50.6.0.0','category_theme_id' => '1.7.11.0.0'),
    136 => array('category_eventtype_id' => '0.50.6.0.0','category_theme_id' => '1.7.14.0.0'),
    137 => array('category_eventtype_id' => '0.3.2.0.0'),
    138 => array('category_eventtype_id' => '0.3.2.0.0'),
    139 => array('category_eventtype_id' => '0.19.0.0.0','category_theme_id' => '1.51.13.0.0'),
    140 => array('category_eventtype_id' => '0.19.0.0.0','category_theme_id' => '1.51.3.0.0'),
    141 => array('category_eventtype_id' => '0.19.0.0.0','category_theme_id' => '1.51.6.0.0'),
    142 => array('category_eventtype_id' => '0.19.0.0.0','category_theme_id' => '1.51.11.0.0'),
    143 => array('category_eventtype_id' => '0.19.0.0.0','category_theme_id' => '1.58.8.0.0'),
    144 => array('category_eventtype_id' => '0.19.0.0.0','category_theme_id' => '1.51.10.0.0'),
    145 => array('category_eventtype_id' => '0.19.0.0.0','category_theme_id' => '1.51.12.0.0'),
    146 => array('category_eventtype_id' => '0.19.0.0.0','category_theme_id' => '1.51.14.0.0'),
    147 => array('category_eventtype_id' => '0.56.0.0.0'),
    148 => array('category_eventtype_id' => '0.0.0.0.0','category_theme_id' => '1.0.5.0.0'),
    149 => array('category_eventtype_id' => '0.0.0.0.0','category_theme_id' => '1.0.9.0.0'),
    150 => array('category_eventtype_id' => '0.15.0.0.0'),
    151 => array('category_eventtype_id' => '0.16.0.0.0'),
    152 => array('category_eventtype_id' => '0.12.0.0.0'),
    153 => array('category_eventtype_id' => '0.3.1.0.0','category_theme_id' => '1.42.0.0.0'),
    154 => array('category_eventtype_id' => '0.3.1.0.0','category_theme_id' => '1.21.0.0.0'),
    155 => array('category_eventtype_id' => '0.3.1.0.0','category_theme_id' => '1.37.0.0.0'),
    156 => array('category_eventtype_id' => '0.3.1.0.0','category_theme_id' => '1.61.0.0.0'),
    157 => array('category_eventtype_id' => '0.3.1.0.0','category_theme_id' => '1.43.0.0.0'),
    158 => array('category_eventtype_id' => '0.3.1.0.0','category_theme_id' => '1.41.0.0.0'),
    159 => array('category_eventtype_id' => '0.3.1.0.0','category_theme_id' => '1.37.1.0.0'),
    160 => array('category_eventtype_id' => '0.3.1.0.0','category_theme_id' => '1.37.2.0.0'),
    161 => array('category_eventtype_id' => '0.3.1.0.0','category_theme_id' => '1.44.0.0.0'),
    162 => array('category_eventtype_id' => '0.3.1.0.0','category_theme_id' => '1.64.0.0.0'),
    163 => array('category_eventtype_id' => '0.3.1.0.0','category_theme_id' => '1.25.0.0.0'),
    164 => array('category_eventtype_id' => '0.3.1.0.0','category_theme_id' => '1.51.13.0.0'),
    165 => array('category_eventtype_id' => '0.57.0.0.0'),
    166 => array('category_eventtype_id' => '1.50.0.0.0'),
    167 => array('category_eventtype_id' => '0.59.0.0.0','category_theme_id' => '1.51.13.0.0'),
    168 => array('category_eventtype_id' => '0.59.0.0.0','category_theme_id' => '1.51.14.0.0'),
    169 => array('category_eventtype_id' => '0.59.0.0.0','category_theme_id' => '1.51.3.0.0'),
    170 => array('category_eventtype_id' => '0.59.0.0.0','category_theme_id' => '1.51.6.0.0'),
    171 => array('category_eventtype_id' => '0.59.0.0.0','category_theme_id' => '1.51.11.0.0'),
    172 => array('category_eventtype_id' => '0.59.0.0.0','category_theme_id' => '1.58.8.0.0'),
    173 => array('category_eventtype_id' => '0.59.0.0.0','category_theme_id' => '1.51.10.0.0'),
    174 => array('category_eventtype_id' => '0.59.0.0.0','category_theme_id' => '1.51.12.0.0'),
    175 => array('category_umv_id' => 'umv.9','voor_kinderen' => '1'),
    176 => array('category_eventtype_id' => '0.100.0.0.0'),
    177 => array('category_eventtype_id' => '0.100.2.0.0'),
    178 => array('category_eventtype_id' => '0.100.1.0.0'),
    179 => array('voor_kinderen' => '1'),
    205 => array('category_eventtype_id' => '0.50.6.0.0','category_theme_id' => '1.7.15.0.0'),
    206 => array('category_eventtype_id' => '0.3.1.0.0','category_theme_id' => '1.65.0.0.0')
  );

  return $mapping[$heading];

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
    // If path contains % argument for rss of ajax we use agenda/search
    if ($path == 'agenda/search/%') {
      $path = 'agenda/search';
    }
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
function culturefeed_search_ui_default_filter_options($filter_form_number) {

  $defaults = array(
    1 => array(
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
        'query-string' => 'voor-kinderen',
        'api-filter-query' => '(agefrom:[* TO 12] OR keywords:"ook voor kinderen")'
      ),
      array(
        'exposed' => TRUE,
        'title' => 'For UiTPAS and Paspartoe',
        'query-string' => 'uitpas',
        'api-filter-query' => '(keywords:uitpas* OR Paspartoe)'
      ),
    ),
    2 => array(
      array(
        'exposed' => TRUE,
        'title' => 'Hide long-running events',
        'query-string' => 'no-periods',
        'api-filter-query' => 'periods:false'
      ),
      array(
        'exposed' => TRUE,
        'title' => 'Hide permanent events',
        'query-string' => 'no-permanent',
        'api-filter-query' => 'permanent:false'
      ),
    ),
    3 => array(
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
        'query-string' => 'voor-kinderen',
        'api-filter-query' => '(agefrom:[* TO 12] OR keywords:"ook voor kinderen")'
      ),
      array(
        'exposed' => TRUE,
        'title' => 'For UiTPAS and Paspartoe',
        'query-string' => 'uitpas',
        'api-filter-query' => '(keywords:uitpas* OR Paspartoe)'
      ),
    ),
  );

  return isset($defaults[$filter_form_number]) ? $defaults[$filter_form_number] : array();

}

/**
 * Return the default options for the holiday facet block.
 */
function culturefeed_search_ui_default_holidays_options() {

  return array(
    array(
      'exposed' => TRUE,
      'holiday' => 'Krokusvakantie 2016',
      'query-string' => 'krokusvakantie',
      'start-date' => array(
        'year' => '2016',
        'month' => '2',
        'day' => '6'
      ),
      'end-date' => array(
        'year' => '2016',
        'month' => '2',
        'day' => '14'
      ),
      'api-filter-query' => 'startdate:[2016-02-06T00:00:00Z TO 2016-02-14T23:59:59Z]'
    ),
    array(
      'exposed' => TRUE,
      'holiday' => 'Paasvakantie 2016',
      'query-string' => 'paasvakantie',
      'start-date' => array(
        'year' => '2016',
        'month' => '3',
        'day' => '26'
      ),
      'end-date' => array(
        'year' => '2016',
        'month' => '4',
        'day' => '10'
      ),
      'api-filter-query' => 'startdate:[2016-03-26T00:00:00Z TO 2016-04-10T23:59:59Z]'
    ),
    array(
      'exposed' => TRUE,
      'holiday' => 'Zomervakantie 2016',
      'query-string' => 'zomervakantie',
      'start-date' => array(
        'year' => '2016',
        'month' => '7',
        'day' => '1'
      ),
      'end-date' => array(
        'year' => '2016',
        'month' => '8',
        'day' => '31'
      ),
      'api-filter-query' => 'startdate:[2016-07-01T00:00:00Z TO 2016-08-31T23:59:59Z]'
    ),
    array(
      'exposed' => TRUE,
      'holiday' => 'Herfstvakantie 2016',
      'query-string' => 'herfstvakantie',
      'start-date' => array(
        'year' => '2016',
        'month' => '10',
        'day' => '29'
      ),
      'end-date' => array(
        'year' => '2016',
        'month' => '11',
        'day' => '6'
      ),
      'api-filter-query' => 'startdate:[2016-10-29T00:00:00Z TO 2016-11-06T23:59:59Z]'
    ),
    array(
      'exposed' => TRUE,
      'holiday' => 'Kerstvakantie 2016',
      'query-string' => 'kerstvakantie',
      'start-date' => array(
        'year' => '2016',
        'month' => '12',
        'day' => '24'
      ),
      'end-date' => array(
        'year' => '2017',
        'month' => '1',
        'day' => '8'
      ),
      'api-filter-query' => 'startdate:[2016-12-24T00:00:00Z TO 2017-01-08T23:59:59Z]'
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

/**
 * Sanitize the user input of a search field.
 */
function _culturefeed_search_ui_sanitize_query_term($term) {
  // Replace special characters with normal ones.
  $term = culturefeed_search_transliterate($term);

  // Replace AND to a space.
  $term = str_replace(' AND ', ' ', $term);

  $query_parts = explode(' OR ', $term);
  array_walk($query_parts, function(&$search_string) {

    // Strip of words between quotes. The spaces don't need to be replaced to AND for them.
    preg_match_all('/".*?"/', $search_string, $matches);
    foreach ($matches[0] as $match) {
      $search_string = str_replace($match, '', $search_string);
    }

    $search_string = str_replace('  ', ' ', $search_string);

    // Put words with a special character between quotes.
    $words = explode(' ', trim($search_string));
    $parts = array();
    $special_characters = '-!?&/';
    foreach ($words as $word) {
      if (strpbrk($word, $special_characters)) {
        $word = '"' . $word . '"';
      }
      $parts[] = $word;
    }

    // Replace spaces between multiple search words by 'AND'.
    $search_string = implode(' AND ', $parts);

    // Add back the words between quotes.
    if (!empty($matches[0])) {
      if (empty($search_string)) {
        $search_string .= implode(' AND ', $matches[0]);
      }
      else {
        $search_string .= ' AND ' . implode(' AND ', $matches[0]);
      }
    }

  });

  return implode(' OR ', $query_parts);
}
