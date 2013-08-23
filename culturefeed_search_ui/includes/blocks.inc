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

  $default = isset($_GET['search']) ? check_plain($_GET['search']) : '';

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
    '#default_value' => $default,
    '#autocomplete_path' => 'autocomplete/culturefeed/suggestions',
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
 * Creates a form to change the sortorder of the search results.
 */
function culturefeed_search_ui_search_sortorder_form($form, & $form_state) {

  $query = drupal_get_query_parameters();
  $searchable_types = culturefeed_get_searchable_types();
  $active_page = _culturefeed_search_ui_get_active_search_page();

  // Don't show sort form, if we have no options.
  if (!$active_page || !isset($searchable_types[$active_page]) || empty($searchable_types[$active_page]['sort_options'])) {
    return;
  }

  $options = isset($searchable_types[$active_page]['sort_options']) ? $searchable_types[$active_page]['sort_options'] : array();
  $default = $searchable_types[$active_page]['sort_default'];
  $default = isset($query['sort']) ? $query['sort'] : $default;

  if (!$options) {
    return;
  }

  $form['sort'] = array(
    '#title' => t('Sort'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $default,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  $form['#attached']['js'][] = drupal_get_path('module', 'culturefeed_search_ui') . '/js/search_ui.js';
  $form['#attributes']['class'] = array('sortorder-form');

  return $form;

}

/**
 * Submits and changes the sortorder of the search results.
 */
function culturefeed_search_ui_search_sortorder_form_submit($form, & $form_state) {

  // When changing sort method, we remove the page parameter.
  $query = drupal_get_query_parameters(NULL, array('q', 'page'));
  $query['sort'] = $form_state['values']['sort'];

  drupal_goto($_GET['q'], array('query' => $query));

}

/**
 * Renders a facet block to go along with the search form.
 * @param $facet_name
 *   Facet to show (ex category_theme_name)
 * @param $domain_id
 *   Domain id where this facet belongs to. This is used to show the title
 * @param $allowed_items
 *   List of allowed items, and the label that should be shown for every item.
 */
function culturefeed_search_ui_search_facets_block($facet_name, $domain_id = '', $allowed_items = array()) {

  // FacetingComponent is set in the page callback.
  $culturefeedFacetingComponent = culturefeed_get_facet_component();
  if (!$culturefeedFacetingComponent instanceof \CultuurNet\Search\Component\Facet\FacetComponent) {
    return;
  }

  $build = array();

  $facets = $culturefeedFacetingComponent->getFacets();
  if (!isset($facets[$facet_name])) {
    return;
  }

  $facet = $facets[$facet_name];

  // Show only the allowed values, and set the requested label.
  if (!empty($allowed_items)) {
    $all_items = $facet->getResult()->getItems();
    $items = array();
    foreach ($all_items as $item) {
      if (isset($allowed_items[$item->getLabel()]) && $item->getTotalResults() > 0) {
        $item->setLabel($allowed_items[$item->getLabel()]);
        $items[] = $item;
      }
    }
  }
  else {
    $items = $facet->getResult()->getItems();
  }

  if (!empty($items)) {

    $subject = '';
    if (!empty($domain_id)) {
      $subject = db_query('SELECT label FROM {culturefeed_search_domains} WHERE did = :did', array(':did' => $domain_id))->fetchField();
    }

    return array(
      'subject' => check_plain($subject),
      'content' => array(
        '#theme' => ($domain_id == 'eventtype' ? 'culturefeed_search_facet_eventtype' : 'culturefeed_search_facet'),
        '#facet' => $facet,
        '#items' => $items,
      ),
    );
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

  $build = array();

  $allowed_items = culturefeed_search_ui_get_date_facet_options();
  $facet_block = culturefeed_search_ui_search_facets_block('datetype', '', $allowed_items);

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
 * Block callback to show the date facet.
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

  if ($culturefeedSearchResult->getTotalCount() == 0) {
    return;
  }

  $build = array();

  // Show facet layout for location.
  if (!empty($_GET['location'])) {

    // Build facet for location.
    $facet = new \CultuurNet\Search\Component\Facet\Facet('location', new \CultuurNet\Search\Parameter\FacetField('location'));
    $label = $_GET['location'];
    if (isset($_GET['distance'])) {
      $label .= ' +' . $_GET['distance'] . ' km';
    }

    // Make a result item for location search.
    global $pager_total_items;
    $items = array(
      new \CultuurNet\Search\Component\Facet\FacetResultItem($label, $label, $pager_total_items[0]),
    );

    $build = array();
    $build[] = array(
      '#theme' => 'culturefeed_search_facet',
      '#facet' => $facet,
      '#items' => $items,
    );

    // If a postal was given. We can add proximity search.
    $city_parts = explode(' ', $_GET['location']);
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

  $form['#attributes']['class'][] = 'append-button';

  $form['location'] = array(
    '#type' => 'textfield',
    '#title' => t('Where'),
    '#title_display' => 'invisible',
    '#autocomplete_path' => 'autocomplete/culturefeed/city-region-suggestion',
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
function culturefeed_search_ui_city_facet_form_submit($form, &$form_state) {

  $query = drupal_get_query_parameters(NULL, array('q', 'page', 'location', 'nearby', 'coordinates'));

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
    $options[$i] = '+' . $i . ' km';
  }

  $form = array();

  $form['#attributes']['class'][] = 'append-button';

  $form['distance'] = array(
    '#type' => 'select',
    '#title' => t('Search nearby'),
    '#options' => $options,
    '#default_value' => isset($_GET['distance']) ? $_GET['distance'] : CULTUREFEED_SEARCH_DEFAULT_PROXIMITY_RANGE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Ok'),
  );

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
    $build['reset'] = array('#markup' => l(t('Remove all filters') . ' <i class="icon-remove icon-large text-red"></i>', current_path(), array('html' => TRUE, 'attributes' => array('class' => array('remove-all')))));
  }

  // Suggestions if available.
  if ($culturefeedSearchResult instanceof \CultuurNet\Search\SearchResult) {
    $suggestions_keys = $culturefeedSearchResult->getSuggestions();
    if (!empty($suggestions_keys)) {
      $search_query = array();
      $suggestions = array();
      foreach ($suggestions_keys as $suggestion) {
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
