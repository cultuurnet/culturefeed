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
    'today' => t('Today'),
    'next7days' => t('Next 7 days'),
    'thisweekend' => t('This weekend'),
    'nextweekend' => t('Next weekend'),
    'next14days' => t('Next 14 days'),
    'next30days' => t('Next 30 days'),
    'next12months' => t('Next 12 months'),
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

    // No index can only be FALSE, if user is filtering on event type.
    if (isset($query['facet']['category_eventtype_id'])) {

      $facets = $query['facet'];
      unset($facets['category_eventtype_id']);
      unset($facets['category_theme_id']);

      unset($query['location']);
      unset($query['facet']);

      // If theme_id was the only extra facet / parameters, set noindex to false.
      if (count($facets) == 0 && count($query) == 0) {
        $noindex = FALSE;
      }

    }

  }

  // Add the "noindex, follow" meta tag if needed.
  if ($noindex) {
    $element = array(
      '#tag' => 'meta',
      '#attributes' => array(
        'name' => 'robots',
        'content' => 'noindex, follow',
      ),
    );

    drupal_add_html_head($element, 'robots');
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
