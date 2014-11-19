<?php
/**
 * @file
 * Block callbacks for the saved searches module.
 */

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

  if ($page = culturefeed_search_get_active_search_page()) {

    // Construct saved query parameters.
    $query = array();

    // Add page title.
    $query['title'] = drupal_get_title();

    // Add search parameters in string.
    $supported_parameters = array('fq', 'q', 'zipcode');
    $parameters = $page->getParameters();

    $has_date_filter = FALSE;
    $params = array();
    foreach ($parameters as $param) {
      $key = $param->getKey();

      if (in_array($key, $disallowed_parameters)) {
        $params[] = $key . '=' . $param->getValue();
      }
      elseif ($key == 'datetype') {
        $has_date_filter = TRUE;
      }
    }

    $query['query'] = implode('&', $params);
    if ($has_date_filter) {
      $query['datefilter'] = TRUE;
    }

    $text = '<h5>' . t('Save this search') . '</h5>' . t('to search even faster next time');
    $link = l($text, 'culturefeed/search/save', array('query' => $query, 'html' => TRUE, 'attributes' => array('class' => 'btn-primary btn btn-block')));

    return $link;
  }
}
