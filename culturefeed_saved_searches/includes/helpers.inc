<?php

/**
 * Helper functions for culturefeed_saved_searches.
 */

/**
 * Get the frequency options for saved searches.
 */
function culturefeed_saved_searches_frequency_options() {

  return array(
    CultureFeed_SavedSearches_SavedSearch::ASAP => t('daily overview'),
    CultureFeed_SavedSearches_SavedSearch::DAILY => t('weekly overview'),
    CultureFeed_SavedSearches_SavedSearch::WEEKLY => t('immediately'),
    CultureFeed_SavedSearches_SavedSearch::NEVER => t('never'),
  );

}

/**
 * Get the query for the search url of a saved search.
 */
function culturefeed_saved_search_get_search_url_query($search) {
dsm($search->query);
  $query = array();
  foreach (explode('&', $search->query) as $key => $value) {

    $param = explode('=', $value);
    if ($param[0] == 'group') {
      continue;
    }

    if ($param[0] == 'q') {
      $query['search'] = $param[1];
    }
    elseif ($param[0] == 'fq') {

      $conditions = explode(' OR ', $param[1]);
      foreach ($conditions as $condition) {

        $search_on = explode(':', $condition);

        $facet_field = $search_on[0];
        $facet_field = str_replace(array('(', ')', '"'), '', $facet_field);

        $facet_value = $search_on[1];
        $facet_value = str_replace(array('(', ')', '"'), '', $facet_value);

        if ($facet_field == 'agefrom') {
          $query['min_age'] = culturefeed_saved_search_get_agefrom_value($facet_value);
        }
        else {
          $values = explode(',', $facet_value);
          if (!isset($query['facet'][$search_on[0]])) {
            $query['facet'][$search_on[0]] = array();
          }
          $query['facet'][$search_on[0]] = array_merge($query['facet'][$search_on[0]], $values);
        }

      }

    }

  }

  // Get the correct categorie facets.
  if (isset($query['facet']['category_id'])) {
    $terms = db_select('culturefeed_search_terms', 't')
      ->condition('tid', $query['facet']['category_id'])
      ->condition('language', LANGUAGE_NONE)
      ->fields('t', array('tid', 'did'))->execute();

    foreach ($terms as $term) {
      $query['facet']['category_' . $term->did . '_id'][] = $term->tid;
      unset($query['facet']['category_id']);
    }

  }

  return $query;

}

/**
 * Get the agefrom querystring to be set active.
 */
function culturefeed_saved_search_get_agefrom_value($facet_value) {
  $options = culturefeed_search_ui_get_age_range_facet_options();
  foreach ($options as $key => $option) {
    $option['query'] = str_replace(array('(', ')', '"'), '', $option['query']);
    if ($option['query'] == $facet_value) {
      return $key;
    }
  }
}