<?php
/**
 * @file
 * Block callbacks for the saved searches module.
 */

/**
 * CTA block to save current search.
 */
function culturefeed_saved_searches_block_cta() {

  return culturefeed_saved_searches_construct_save_link(t('and receive an email when there are new results'));

}

/**
 * CTA block to save current search, at the bottom of search results.
 */
function culturefeed_saved_searches_block_cta_bottom() {

  $results = culturefeed_get_search_result();

  if ($results && $results->getCurrentCount() > 0) {
    return culturefeed_saved_searches_construct_save_link(t('to search faster next time'));
  }

}

/**
 * Helper function taht constructs the 'save search'-link.
 */
function culturefeed_saved_searches_construct_save_link($text) {

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

      if (in_array($key, $supported_parameters)) {
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

    // We need to fetch the whole list of saved searches since we don't have ID.
    $savedSearchList = DrupalCultureFeed::getLoggedInUserInstance()->savedSearches()->getList();
    $exists = FALSE;
    foreach ($savedSearchList as $savedSearch) {
      if ($savedSearch->query == $query['query']) {
        $exists = TRUE;
      }
    }

    if ($exists) {
      return;
    }

    return theme('culturefeed_saved_searches_cta', array('text' => $text, 'query' => $query));

  }
}

/**
 * Render a block with a (short) list of saved searches.
 */
function culturefeed_saved_searches_block_teaser() {

  $block = array('title' => '', 'content' => '');

  $build = array();

  try {

    $block['content'] = array(
      '#theme' => 'culturefeed_saved_searches_list',
      '#searches' => DrupalCultureFeed::getLoggedInUserInstance()->savedSearches()->getList(),
    );

  }
  catch (Exception $e) {
    drupal_set_message(t('A problem occurred while loading your searches. Please try again later.'), 'error');
  }

  return $block;

}
