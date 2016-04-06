<?php
/**
 * @file
 * Defines blocks which add integration to culturefeed.
 */

/**
 * Form function for a search box in all searchable types.
 *
 * @param array $form
 * @param array $form_state
 * @return array
 */
function culturefeed_search_ui_search_block_form($form, & $form_state) {

  $searchable_types = culturefeed_get_searchable_types();
  uasort($searchable_types, 'drupal_sort_weight');

  $options = array();
  foreach ($searchable_types as $key => $type) {
    $options[$key] = $type['search_title'];
  }
  $active_page = _culturefeed_search_ui_get_active_search_page();
  $form['#action'] = url('culturefeed/search');

  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Search in'),
    '#default_value' => $active_page,
    '#options' => $options,
  );

  $form['search'] = array(
    '#type' => 'textfield',
    '#title' => t('Search terms'),
    '#default_value' => isset($_GET['search']) ? $_GET['search'] : '',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  $form_state['searchable_types'] = $searchable_types;

  global $user;
  if ($user->uid) {
    $form['#token'] = FALSE;
  }

  return $form;

}

/**
 * Submit function for a search box in all searchable types.
 *
 * @param array $form
 * @param array $form_state
 */
function culturefeed_search_ui_search_block_form_submit($form, & $form_state) {

  $query = drupal_get_query_parameters(NULL, array('q', 'page', 'search'));
  if (!empty($form_state['values']['search'])) {
    $query['search'] = $form_state['values']['search'];
  }

  // Add extra possible querystring provided by the hook.
  $search_page = $form_state['searchable_types'][$form_state['values']['type']];
  if (!empty($search_page['active_facets'])) {
    $query['facet'] = $search_page['active_facets'];
  }

  $form_state['redirect'] = array(
    $search_page['path'],
    array('query' => $query),
  );

}

/**
 * Renders a facet block to go along with the search form.
 * @param $facet_name
 *   Facet to show (ex category_theme_name)
 * @param $domain_id
 *   Domain id where this facet belongs to. This is used to show the title
 * @param $allowed_items
 *   List of allowed items, and the label that should be shown for every item.
 * @param array $settings
 *   Array containing setting information (sort, highest level and subitems).
 *   $settings['intersect'] only works when $allowed_items is set. Intersect
 *   enabled will only use the options which were given.
 *   Intersect disabled will override the filter term labels. If you use this,
 *   make sure the given allowed items have the label as key as well.
 */
function culturefeed_search_ui_search_facets_block($facet_name, $domain_id = '', $allowed_items = array(), $settings = array()) {

  // FacetingComponent is set in the page callback.
  $culturefeedFacetingComponent = culturefeed_get_facet_component();
  if (!$culturefeedFacetingComponent instanceof \CultuurNet\Search\Component\Facet\FacetComponent) {
    return;
  }

  $facets = $culturefeedFacetingComponent->getFacets();
  if (!isset($facets[$facet_name])) {
    return;
  }

  $facet = $facets[$facet_name];

  if (!empty($allowed_items)) {
    $all_items = $facet->getResult()->getItems();
    $items = array();
    // Show only the allowed values, and set the requested label.
    if (!isset($settings['intersect']) || !$settings['intersect']) {
      foreach ($all_items as $item) {
        if (isset($allowed_items[$item->getLabel()]) && $item->getTotalResults() > 0) {
          $item->setLabel($allowed_items[$item->getLabel()]);
          $items[] = $item;
        }
      }
    }
    // Show only the filtered allowed values.
    else {
      foreach ($all_items as $item) {
        if (isset($allowed_items[$item->getValue()]) && $item->getTotalResults() > 0) {
          $items[] = $item;
        }
      }
    }
  }
  else {
    $items = $facet->getResult()->getItems();

    // type facet should go through t()
    if ($facet_name == 'type') {
      foreach ($items as $item) {
        $item->setLabel(t($item->getLabel(), array(), array('context' => 'culturefeed-facets')));
      }
    }
    // If the facet is one of the category facets. Translate the items.
    elseif (substr($facet_name, strlen($facet_name) - 3) == '_id') {
      culturefeed_search_translate_facet($items);
    }

  }

  if (!empty($items)) {
    $subject = '';
    if (!empty($domain_id)) {
      $subject = db_query('SELECT label FROM {culturefeed_search_domains} WHERE did = :did', array(':did' => $domain_id))->fetchField();
    }

    // Do we have to use the theming compatibility mode?
    $theme_function = 'culturefeed_search_facet_with_depth_settings';
    if (variable_get('culturefeed_search_ui_facet_compatibility_mode', 0) === 1) {
      if ($domain_id == 'eventtype') {
        $theme_function = 'culturefeed_search_facet_eventtype';
      }
      else {
        $theme_function = 'culturefeed_search_facet';
      }
    }

    $return = array(
      'subject' => check_plain($subject),
      'content' => array(
        '#theme' => $theme_function,
        '#facet' => $facet,
        '#items' => $items,
        '#sort' => (isset($settings['sort'])) ? $settings['sort'] : '',
        '#highest_level' => (isset($settings['highest_level'])) ? $settings['highest_level'] : '',
        '#show_subitems' => (isset($settings['show_subitems'])) ? $settings['show_subitems'] : '',
      ),
    );
    return $return;
  }
}

/**
 * Block callback to show the date facet.
 */
function culturefeed_search_ui_block_date_facet() {
  $culturefeedFacetingComponent = culturefeed_get_facet_component();
  if (!$culturefeedFacetingComponent instanceof \CultuurNet\Search\Component\Facet\FacetComponent) {
    return;
  }

  $allowed_items = culturefeed_search_ui_get_date_facet_options();
  $facet_block = culturefeed_search_ui_search_facets_block('datetype', '', $allowed_items, array());
  if (!empty($facet_block)) {

    $build = array();
    $build['facets'] = $facet_block['content'];

    $build[] = drupal_get_form('culturefeed_search_ui_date_facet_form');

    return array(
      'subject' => t('When'),
      'content' => $build,
    );
  }

}

/**
 * Show the additional form for the date facet.
 */
function culturefeed_search_ui_date_facet_form() {

  $form = array();

  $form['#attributes']['class'][] = 'append-button';

  $form['date_range'] = array(
    '#type' => 'textfield',
    '#size' => 28,
    '#title' => t('Choose a date'),
    '#title_display' => 'invisible',
    '#default_value' => isset($_GET['date_range']) ? $_GET['date_range'] : '',
    '#attributes' => array('placeholder' => t('Or choose a custom date'), 'autocomplete' => 'off'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Ok'),
  );

  $form['#attached']['library'][] = array('culturefeed_search_ui', 'ui.daterangepicker');
  $form['#attached']['js'][] = drupal_get_path('module', 'culturefeed_search_ui') . '/js/search_ui.js';

  return $form;

}

/**
 * Validate the date facet form.
 */
function culturefeed_search_ui_date_facet_form_validate($form, &$form_state) {

  if (!empty($form_state['values']['date_range'])) {

    $form_state['values']['date_range'] = str_replace(t('till'), '-', $form_state['values']['date_range']);
    $dates = explode('-', $form_state['values']['date_range']);
    $startDate = DateTime::createFromFormat('d/m/Y', trim($dates[0]));
    if (!$startDate) {
      form_set_error('date_range', t('Please enter a valid date'));
    }
    elseif (isset($dates[1])) {
      $endDate = DateTime::createFromFormat('d/m/Y', trim($dates[1]));
      if (!$endDate) {
        form_set_error('date_range', t('Please enter a valid date'));
      }
    }
  }
}

/**
 * Submit the date facet form.
 */
function culturefeed_search_ui_date_facet_form_submit($form, &$form_state) {

  $query = drupal_get_query_parameters(NULL, array('q', 'page'));
  unset($query['facet']['datetype']);

  if (empty($query['facet'])) {
    unset($query['facet']);
  }

  if (!empty($form_state['values']['date_range'])) {
    $query['date_range'] = $form_state['values']['date_range'];
  }

  $form_state['redirect'] = array(
    $_GET['q'],
    array('query' => $query)
  );

}

/**
 * Block callback to show the city facet.
 */
function culturefeed_search_ui_block_city_facet() {
  $culturefeedFacetingComponent = culturefeed_get_facet_component();
  if (!$culturefeedFacetingComponent instanceof \CultuurNet\Search\Component\Facet\FacetComponent) {
    return;
  }

  $culturefeedSearchResult = culturefeed_get_search_result();
  if (!$culturefeedSearchResult instanceof \Cultuurnet\Search\SearchResult) {
    return;
  }

  $location = '';
  if (!empty($_GET['facet']['category_flandersregion_id'])) {
    $flanders_region = culturefeed_search_get_term_by_id($_GET['facet']['category_flandersregion_id'][0]);
    $location = $flanders_region->name;
  }

  // Show facet layout for location.
  elseif (!empty($_GET['location'])) {
    $location = $_GET['location'];
  }

  $noresults = FALSE;
  if ($culturefeedSearchResult->getTotalCount() == 0) {
    $noresults = TRUE;
    // Hide location selector for empty results only when no location was set.
    if (empty($location)) {
      return;
    }
  }

  $build = array();

  // Show facet layout for location.
  if (!empty($location)) {

    // Build facet for location.
    $facet = new \CultuurNet\Search\Component\Facet\Facet('location', new \CultuurNet\Search\Parameter\FacetField('location'));
    if (isset($_GET['distance'])) {
      $location .= ' + ' . $_GET['distance'] . ' km';
    }

    // Make a result item for location search.
    global $pager_total_items;
    $items = array(
      new \CultuurNet\Search\Component\Facet\FacetResultItem($location, $location, $pager_total_items[0]),
    );

    $build = array();
    $build[] = array(
      '#theme' => 'culturefeed_search_facet',
      '#facet' => $facet,
      '#items' => $items,
      '#noresults' => $noresults,
    );

    // If a postal was given. We can add proximity search.
    $city_parts = explode(' ', $location);
    if (is_numeric($city_parts[0])) {
      $build['distance-form'] = drupal_get_form('culturefeed_search_ui_proximity_distance_form');
    }

  }

  $build['form'] = drupal_get_form('culturefeed_search_ui_city_facet_form');

  return array(
    'subject' => t('Where'),
    'content' => $build,
  );

}

/**
 * Show the additional form for the where facet.
 */
function culturefeed_search_ui_city_facet_form() {

  $form = array();

  $filters = array_filter(array_values(variable_get('culturefeed_facet_city_parents', array())));

  $form['#attached']['library'][] = array(
    'system', 'ui.autocomplete',
  );
  $form['#attached']['js'][] = array(
    'data' => drupal_get_path('module', 'culturefeed_search_ui') . '/js/city_facet.js',
    'type' => 'file',
  );

  $form['#attached']['js'][] = array(
    'data' => array('culturefeed_search_ui' => array('city_filters' => $filters)),
    'type' => 'setting'
  );

  $form['location'] = array(
    '#type' => 'textfield',
    '#size' => 28,
    '#title' => t('Where'),
    '#title_display' => 'invisible',
    '#attributes' => array(
      'class' => array('city-facet', 'form-autocomplete'),
      'placeholder' => t('Zipcode or city')
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Ok'),
    //'#attributes' => array('class' => array('element-invisible')),
    '#attributes' => array('class' => array('city-facet-submit')),
  );

  return $form;

}

/**
 * Submit the where facet form.
 */
function culturefeed_search_ui_city_facet_form_submit($form, &$form_state) {

  $query = drupal_get_query_parameters(NULL, array('q', 'page', 'location', 'nearby', 'coordinates'));

  // Remove the region filter so choosing a city has results.
  if (isset($query['facet']['category_flandersregion_id'])) {
    unset($query['facet']['category_flandersregion_id']);
  }

  // Remove the distance filter.
  if (isset($query['distance'])) {
    unset($query['distance']);
  }

  if (!empty($form_state['values']['location'])) {
    $query['location'] = $form_state['values']['location'];
  }

  $form_state['redirect'] = array(
      $_GET['q'],
      array('query' => $query)
  );

}

/**
 * Form callback to show possibility to change the proximity distance.
 */
function culturefeed_search_ui_proximity_distance_form() {

  $options = array('' => '');
  for ($i = 10; $i <= 50; $i += 10) {
    $options[$i] = '+ ' . $i . ' km';
  }

  $form = array();

  $form['#attributes']['class'][] = 'append-button';

  $form['distance'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => isset($_GET['distance']) ? $_GET['distance'] : CULTUREFEED_SEARCH_DEFAULT_PROXIMITY_RANGE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Change radius'),
  );

  $form['new_search']['#markup'] = '<label>' . t('Choose another location') . '</label>';

  return $form;

}

/**
 * Submit the distance form.
 */
function culturefeed_search_ui_proximity_distance_form_submit($form, &$form_state) {

  $query = drupal_get_query_parameters(NULL, array('q', 'page', 'distance'));
  if (!empty($form_state['values']['distance'])) {
    $query['distance'] = $form_state['values']['distance'];
  }

  $form_state['redirect'] = array(
    $_GET['q'],
    array('query' => $query)
  );

}

/**
 * Block callback to show the Age Range facet.
 */
function culturefeed_search_ui_block_age_range_facet() {

  $facet_component = culturefeed_get_facet_component();
  if (!$facet_component instanceof \CultuurNet\Search\Component\Facet\FacetComponent) {
    return;
  }

  $facets = $facet_component->getFacets();
  if (!isset($facets['agefrom'])) {
    return;
  }

  $facet = $facets['agefrom'];
  $requested_items = culturefeed_search_ui_get_age_range_facet_options();
  $counts = array();
  foreach (array_keys($requested_items) as $key) {
    $counts[$key] = 0;
  }

  // Calculate the different age facets.
  $all_items = $facet->getResult()->getItems();
  foreach ($all_items as $item) {
    $age = $item->getValue();
    if ($age >= 0 && $age <= 2) {
      $counts[0] += $item->getTotalResults();
    }
    elseif ($age >= 3 && $age <= 5) {
      $counts[3] += $item->getTotalResults();
    }
    elseif ($age >= 6 && $age <= 8) {
      $counts[6] += $item->getTotalResults();
    }
    elseif ($age >= 9 && $age <= 11) {
      $counts[9] += $item->getTotalResults();
    }
  }

  // Create facet item for every count.
  foreach ($requested_items as $age => $data) {
    if (!empty($counts[$age])) {
      $items[] = new \CultuurNet\Search\Component\Facet\FacetResultItem($data['name'], $age, $counts[$age]);
    }
  }

  if (!empty($items)) {
    return array(
      'subject' => t('Age'),
      'content' => array(
        '#theme' => 'culturefeed_search_facet',
        '#facet' => $facet,
        '#items' => $items,
      ),
    );
  }
}

/**
 * Block callback to show the city only facet.
 */
function culturefeed_search_ui_block_city_only_facet() {
  $culturefeedFacetingComponent = culturefeed_get_facet_component();
  if (!$culturefeedFacetingComponent instanceof \CultuurNet\Search\Component\Facet\FacetComponent) {
    return;
  }

  $culturefeedSearchResult = culturefeed_get_search_result();
  if (!$culturefeedSearchResult instanceof \Cultuurnet\Search\SearchResult) {
    return;
  }

  if ($culturefeedSearchResult->getTotalCount() == 0) {
    return;
  }

  $build = array();

  // Show facet layout for location.
  $location = '';
  if (!empty($_GET['facet']['category_flandersregion_id'])) {
    $flanders_region = culturefeed_search_get_term_by_id($_GET['facet']['category_flandersregion_id'][0]);
    $location = $flanders_region->name;
  }

  // Show facet layout for location.
  elseif (!empty($_GET['location'])) {
    $location = $_GET['location'];
  }

  if (!empty($location)) {

    // Build facet for location.
    $facet = new \CultuurNet\Search\Component\Facet\Facet('location', new \CultuurNet\Search\Parameter\FacetField('location'));

    // Make a result item for location search.
    global $pager_total_items;
    $items = array(
      new \CultuurNet\Search\Component\Facet\FacetResultItem($location, $location, $pager_total_items[0]),
    );

    $build = array();
    $build[] = array(
      '#theme' => 'culturefeed_search_facet',
      '#facet' => $facet,
      '#items' => $items,
    );

  }

  $build['form'] = drupal_get_form('culturefeed_search_ui_city_only_facet_form');

  return array(
    'subject' => t('Where'),
    'content' => $build,
  );

}

/**
 * Show the additional form for the where facet.
 */
function culturefeed_search_ui_city_only_facet_form() {

  $form = array();

  $form['#attributes']['class'][] = 'append-button';

  $form['location'] = array(
    '#type' => 'textfield',
    '#size' => 28,
    '#title' => t('Where'),
    '#title_display' => 'invisible',
    '#autocomplete_path' => 'autocomplete/culturefeed/city-suggestion',
    '#attributes' => array('placeholder' => t('Zipcode or city')),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Ok'),
  );

  return $form;

}

/**
 * Submit the where facet form.
 */
function culturefeed_search_ui_city_only_facet_form_submit($form, &$form_state) {

  $query = drupal_get_query_parameters(NULL, array('q', 'page', 'location'));

  if (!empty($form_state['values']['location'])) {
    $query['location'] = $form_state['values']['location'];
  }

  $form_state['redirect'] = array(
      $_GET['q'],
      array('query' => $query)
  );

}

/**
 * Show the entity type facet.
 */
function culturefeed_search_ui_block_entity_type_facet() {

  $block_settings = array(
    'sort' => variable_get('culturefeed_search_sort_facet-type', CULTUREFEED_SEARCH_SORT_ALPHABETICALLY),
    'highest_level' => variable_get('culturefeed_facet_highest_level_facet-type', 1),
    'show_subitems' => variable_get('culturefeed_facet_show_subitems_facet-type', FALSE),
  );

  $block = culturefeed_search_ui_search_facets_block('type', '', array(), $block_settings);
  $block['subject'] = t('Type');

  return $block;

}

/**
 * Block callback for the active filters during search.
 */
function culturefeed_search_ui_search_active_filters_block() {
  $culturefeedFacetingComponent = culturefeed_get_facet_component();
  $culturefeedSearchResult = culturefeed_get_search_result();

  if (!$culturefeedFacetingComponent instanceof \CultuurNet\Search\Component\Facet\FacetComponent) {
    return '';
  }

  $build = module_invoke_all('culturefeed_search_ui_active_filters', $culturefeedFacetingComponent);

  if (!empty($build) && count($build) > 1) {
    $build['reset'] = array('#markup' => l(t('Remove all filters'), current_path(), array('html' => TRUE, 'attributes' => array('class' => array('remove-all')))));
  }

  // Suggestions if available.
  if ($culturefeedSearchResult instanceof \CultuurNet\Search\SearchResult) {
    $suggestions_keys = $culturefeedSearchResult->getSuggestions();
    if (!empty($suggestions_keys)) {
      $search_query = array();
      $suggestions = array();
      foreach ($suggestions_keys as $suggestion) {
        $suggestion = str_replace(' AND ',' ',$suggestion);
        $search_query['search'] = $suggestion;
        $suggestions[$suggestion] = url(current_path(), array('query' => $search_query));
      }

      $build['suggestions'] =  array(
        '#theme' => 'culturefeed_search_suggestions',
        '#suggestions' => $suggestions
      );
    }

  }

  return $build;

}

/**
 * Block callback for the suggestion of the parent region during flandersregion/location search.
 */
function culturefeed_search_ui_search_parent_region_suggestion_block() {
  $culturefeedFacetingComponent = culturefeed_get_facet_component();

  if (!$culturefeedFacetingComponent instanceof \CultuurNet\Search\Component\Facet\FacetComponent) {
    return '';
  }

  // Check if the required filters are active. Do nothing if not.
  if (empty($_GET['facet']['category_flandersregion_id']) && empty($_GET['location'])) {
    return;
  }


  // Get the parent region for the current location or flandersregion filter.
  $parent_query = db_select('culturefeed_search_terms', 't1');
  $parent_query->innerJoin('culturefeed_search_terms', 't2', 't1.p3 = t2.tid');
  $parent_query->fields('t2', array('name', 'tid'));
  $parent_query->condition('t1.did', 'flandersregion');
  $parent_query->condition('t1.language', culturefeed_get_preferred_language());
  $parent_query->condition('t2.language', culturefeed_get_preferred_language());

  if (!empty($_GET['facet']['category_flandersregion_id'])) {
    $parent_query->condition('t1.tid', $_GET['facet']['category_flandersregion_id'][0]);
  }
  else {
    $parent_query->condition('t1.name', db_like($_GET['location']) . '%', 'LIKE');
  }

  $parent = $parent_query->execute()->fetch();

  // Do nothing if we already are on a parent region.
  if (empty($parent)) {
    return;
  }

  return theme('culturefeed_search_parent_region_suggestion', array('parent_region' => $parent));

}

/**
 * Block callback to show the holidays facet.
 */
function culturefeed_search_ui_block_holidays_facet($filter_options) {
  $searches = culturefeed_get_searchable_types();
  $path = isset($searches['path']) ? $searches['path'] : NULL;
  $query = drupal_get_query_parameters(NULL, array('q', 'page'));

  return  theme('culturefeed_search_block_filter_links',
    array(
      'filter_options' => $filter_options,
      'query' => $query,
      'path' => $path
    )
  );
}

/**
 * Show the filter form for the search ui.
 */
function culturefeed_search_ui_block_filter_block($filter_options, $display_type, $filter_form_number) {

  if ($display_type == 'checkboxes') {
    // Show filters as checkboxes with submit button.
    return drupal_get_form('culturefeed_search_ui_block_filter_form_' . $filter_form_number, $filter_options);
  }
  elseif ($display_type == 'links') {
    // Show filters as links.
    $searches = culturefeed_get_searchable_types();
    $path = isset($searches['path']) ? $searches['path'] : NULL;
    $query = drupal_get_query_parameters(NULL, array('q', 'page'));

    return  theme('culturefeed_search_block_filter_links',
      array(
        'filter_options' => $filter_options,
        'query' => $query,
        'path' => $path
      )
     );
  }

}

/**
 * Block callback for the city and actor search.
 */
function culturefeed_search_ui_block_location_search() {

  $culturefeedFacetingComponent = culturefeed_get_facet_component();
  if (!$culturefeedFacetingComponent instanceof \CultuurNet\Search\Component\Facet\FacetComponent) {
    return;
  }

  $culturefeedSearchResult = culturefeed_get_search_result();
  if (!$culturefeedSearchResult instanceof \Cultuurnet\Search\SearchResult) {
    return;
  }

  if ($culturefeedSearchResult->getTotalCount() == 0) {
    return;
  }

  $build = array();

  $location = '';
  if (!empty($_GET['facet']['category_flandersregion_id'])) {
    $flanders_region = culturefeed_search_get_term_by_id($_GET['facet']['category_flandersregion_id'][0]);
    $location = $flanders_region->name;
  }

  // Show facet layout for location.
  elseif (!empty($_GET['location'])) {
    $location = $_GET['location'];
  }

  if (!empty($location)) {
    // Build facet for location.
    $facet = new \CultuurNet\Search\Component\Facet\Facet('location', new \CultuurNet\Search\Parameter\FacetField('location'));

    // Make a result item for location search.
    global $pager_total_items;
    $items = array(
      new \CultuurNet\Search\Component\Facet\FacetResultItem($location, $location, $pager_total_items[0]),
    );

    $build = array();
    $build[] = array(
      '#theme' => 'culturefeed_search_facet',
      '#facet' => $facet,
      '#items' => $items,
    );

  }

  $build['form'] = drupal_get_form('culturefeed_search_ui_block_location_search_form');

  return array(
    'subject' => t('Where'),
    'content' => $build,
  );
}

/**
 * Show the filter form for the city and actor search.
 */
function culturefeed_search_ui_block_location_search_form($form, $form_state) {

  $form = array();

  $form['#attached']['library'][] = array('system', 'ui.autocomplete');
  $form['#attached']['js'][] = drupal_get_path('module', 'culturefeed') . '/js/common.js';
  $form['#attached']['js'][] = drupal_get_path('module', 'culturefeed_search_ui') . '/js/location_autocomplete.js';

  $form['#attached']['css'][] = drupal_get_path('module', 'culturefeed_search_ui') . '/css/location_search.css';

  $form['block-location-search'] = array(
    '#type' => 'textfield',
    '#size' => 28,
    '#title' => t('Where'),
    '#title_display' => 'invisible',
    '#attributes' => array('class' => array('location-search', 'jquery-ui-autocomplete'), 'placeholder' => t('Zipcode or city')),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Ok'),
    '#attributes' => array('class' => array('location-search-submit')),
  );

  return $form;
}

/**
 * Submit the location search form.
 */
function culturefeed_search_ui_block_location_search_form_submit($form, &$form_state) {

  $query = drupal_get_query_parameters(NULL, array('q', 'page'));

  if (!empty($form_state['values']['block-location-search'])) {

    // Unset also the flanders region ids.
    unset($query['facet']['category_flandersregion_id']);

    $query['location'] = $form_state['values']['block-location-search'];
    $form_state['redirect'] = array(
      $_GET['q'],
      array('query' => $query)
    );
  }

}
