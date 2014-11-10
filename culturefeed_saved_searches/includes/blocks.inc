<?php

/**
 * CTA block to save current search.
 */
function culturefeed_saved_searches_block_cta() {

  return culturefeed_saved_searches_construct_save_link();

}

/**
 * CTA block to save current search, at the bottom of search results.
 */
function culturefeed_saved_searches_block_cta_bottom() {

  $results = culturefeed_get_search_result();

  if ($results && $results->getCurrentCount() > 0) {
    return culturefeed_saved_searches_construct_save_link();
  }

}

/**
 * Helper function taht constructs the 'save search'-link.
 */
function culturefeed_saved_searches_construct_save_link() {

  if ($type = culturefeed_get_searchable_type_by_path()) {
    $page = culturefeed_get_search_page($type);

    // Construct saved query parameters.
    $query = array();

    // Add page title.
    $query['title'] = drupal_get_title();

    // Add search parameters in string.
    $allowed_parameters = array('fq', 'group');
    $parameters = $page->getParameters();

    $has_date_filter = FALSE;
    $query_string = '';

    foreach ($parameters as $param) {
      $key = $param->getKey();

      if (in_array($key, $allowed_parameters)) {
        $value = $param->getValue();
        $query_string .= '&' . $key . '=' . $value;
      }
      if ($key == 'q') { // Query param comes first in string.
        $query_string = $key . '=' . $param->getValue() . $query_string;
      }
      if ($key == 'datetype') {
        $has_date_filter = TRUE;
      }
    }

    $query['query'] = $query_string;
    if ($has_date_filter) {
      $query['datefilter'] = TRUE;
    }

    $text = '<h5>' . t('Save this search') . '</h5>' . t('to search even faster next time');
    $link = l($text, 'culturefeed/search/save', array('query' => $query, 'html' => TRUE, 'attributes' => array('class' => 'btn-primary btn btn-block')));

    return $link;
  }
}
