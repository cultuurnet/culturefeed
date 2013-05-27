<?php

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
  $embed_url = $video_parts[0] .'/' . $video_parts[1];
  $embed_url .= '?fs=1&showinfo=0';

  $embed = '';
  $embed .= '<object width="' . $width . '" height="' . $height . '">';
  $embed .= '<param name="movie" value="'.  $embed_url .'"></param>';
  $embed .= '<param name="allowFullScreen" value="true"></param>';
  $embed .= '<param name="wmode" value="transparent"></param>';
  $embed .= '<embed src="'.  $embed_url .'"';
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
    if ($type['path'] == $_GET['q']) {

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
    'today' => 'Vandaag',
    'next7days' => 'Deze week',
    'next14days' => 'Volgende 14 dagen',
    'next30days' => 'Volgende 30 dagen',
    'next12months' => 'Volgende 12 maanden',
  );

}

/**
 * Get the allowed agefrom facet options.
 */
function culturefeed_search_ui_get_agefrom_facet_options() {

  return array(
    '3' => 'Vanaf 3 jaar',
    '5' => 'Vanaf 5 jaar',
    '7' => 'Vanaf 7 jaar',
    '9' => 'Vanaf 9 jaar',
  );

}