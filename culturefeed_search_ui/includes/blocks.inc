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

  $default = isset($query['search']) ? $query['search']: '';

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
    '#title' => 'Zoeken in',
    '#default_value' => $active_page,
    '#options' => $options,
  );

  $form['search'] = array(
    '#type' => 'textfield',
    '#title' => 'Zoekopdracht',
    '#default_value' => $default,
    '#autocomplete_path' => 'autocomplete/culturefeed/suggestions',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Zoeken',
  );

  $form_state['searchable_types'] = $searchable_types;

  return $form;

}

/**
 * Submit function for a search box in all searchable types.
 *
 * @param array $form
 * @param array $form_state
 */
function culturefeed_search_ui_search_block_form_submit($form, & $form_state) {

  $query = drupal_get_query_parameters(NULL, array('q', 'page'));
  $query['search'] = $form_state['values']['search'];

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
  if (!$active_page && !isset($searchable_types[$active_page]) & !empty($searchable_types[$active_page]['sort_options'])) {
    return;
  }

  $options = isset($searchable_types[$active_page]['sort_options']) ? $searchable_types[$active_page]['sort_options'] : array();
  $default = $searchable_types[$active_page]['sort_default'];
  $default = isset($query['sort']) ? $query['sort'] : $default;

  if (!$options) {
    return;
  }

  $form['sort'] = array(
    '#title' => t('Sorteer resultaten op'),
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
  global $culturefeedFacetingComponent;
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
        '#theme' => 'culturefeed_search_facet',
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

  // FacetingComponent is set in the page callback.
  global $culturefeedFacetingComponent;
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
      'subject' => 'When',
      'content' => $build,
    );
  }

}

/**
 * Show the additional form for the date facet.
 */
function culturefeed_search_ui_date_facet_form() {

  $form = array();

  $form['date_range'] = array(
    '#type' => 'textfield',
    '#title' => 'Kies een datum',
    '#default_value' => isset($_GET['date_range']) ? $_GET['date_range'] : '',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Bevestig',
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

  // FacetingComponent is set in the page callback.
  global $culturefeedFacetingComponent;
  if (!$culturefeedFacetingComponent instanceof \CultuurNet\Search\Component\Facet\FacetComponent) {
    return;
  }

  global $culturefeedSearchResult;
  if (!$culturefeedSearchResult instanceof \Cultuurnet\Search\SearchResult) {
    return;
  }

  if ($culturefeedSearchResult->getTotalCount() == 0) {
    return;
  }

  $build = array();

  // Show facet layout for location.
  if (isset($_GET['location'])) {

    // Build facet for location.
    $facet = new \CultuurNet\Search\Component\Facet\Facet('location', new \CultuurNet\Search\Parameter\FacetField('location'));
    $label = $_GET['location'];
    if (isset($_GET['distance'])) {
      $label .= ' +' . $_GET['distance'] . ' km';
    }

    // Make a result item for location search.
    global $pager_total_items;
    $items = array(
      new \CultuurNet\Search\Component\Facet\FacetResultItem($label, $pager_total_items[0]),
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
    'subject' => 'Where',
    'content' => $build,
  );

}

/**
 * Show the additional form for the where facet.
 */
function culturefeed_search_ui_city_facet_form() {

  $form = array();

  $form['location'] = array(
    '#type' => 'textfield',
    '#title' => 'Waar',
    '#title_display' => 'invisible',
    '#default_value' => isset($_GET['location']) ? $_GET['location'] : '',
    '#autocomplete_path' => 'autocomplete/culturefeed/city-region-suggestion',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Selecteer',
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
  for ($i = 10; $i <= 100; $i += 5) {
    $options[$i] = '+' . $i . ' km';
  }

  $form = array();
  $form['distance'] = array(
    '#type' => 'select',
    '#title' => 'Zoek in omgeving',
    '#options' => $options,
    '#default_value' => isset($_GET['distance']) ? $_GET['distance'] : '',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Ok',
  );

  return $form;

}

/**
 * Submit the distance form.
 */
function culturefeed_search_ui_proximity_distance_form_submit($form, &$form_state) {

  $query = drupal_get_query_parameters(NULL, array('q', 'page'));
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
  // This currently means the page callback has already been invoked.
  global $culturefeedFacetingComponent;
  global $culturefeedSearchResult;

  if (!$culturefeedFacetingComponent instanceof \CultuurNet\Search\Component\Facet\FacetComponent) {
    return '';
  }

  $query = drupal_get_query_parameters(NULL, array('q', 'page'));
  $path = current_path();
  $build = array();

  // Check the current search term.
  if (!empty($query['search'])) {
    $search_query = $query;
    unset($search_query['search']);
    $url = url($path, array('query' => $search_query));
    $build['item_search'] =  array(
      '#theme' => 'culturefeed_search_active_filters_item',
      '#label' => $query['search'],
      '#url' => $url
    );
  }

  // Check the facets.
  if (isset($query['facet'])) {

    // Load all possible human names for special facets.
    $human_names = array();
    if (isset($query['facet']['datetype'])) {
      $human_names['datetype'] = culturefeed_search_ui_get_date_facet_options();
    }
    if (isset($query['facet']['agefrom'])) {
      $human_names['agefrom'] = culturefeed_search_ui_get_agefrom_facet_options();
    }

    $facets = $culturefeedFacetingComponent->getFacets();
    // Loop through all active facets, and generate a remove link.
    foreach ($query['facet'] as $active_facet_name => $selected_value) {
      foreach ($selected_value as $key => $value) {

        $search_query = $query;
        unset($search_query['facet'][$active_facet_name][$key]);

        // If this was the only facet. Also remove the global facet key.
        if (empty($search_query['facet'])) {
          unset($search_query['facet']);
        }

        $url = url($path, array('query' => $search_query));
        $label = $selected_value[$key];

        // Get translated label from terms.
        $label = culturefeed_search_get_term_translation($label);

        $build['item_' . $active_facet_name][$value] = array(
          '#theme' => 'culturefeed_search_active_filters_item',
          '#label' => isset($human_names[$active_facet_name][$label]) ? $human_names[$active_facet_name][$label] : $label,
          '#url' => $url
        );

      }
    }

  }

  // Check if user is filtering on date range.
  if (isset($query['date_range'])) {
    $search_query = $query;
    unset($search_query['date_range']);
    $url = url($path, array('query' => $search_query));
    $build['date_search'] =  array(
      '#theme' => 'culturefeed_search_active_filters_item',
      '#label' => $query['date_range'],
      '#url' => $url
    );
  }

  // Check if user selected 'nearby activities'
  if (isset($query['nearby']) && isset($query['coordinates'])) {
    $search_query = $query;
    unset($search_query['nearby']);
    unset($search_query['coordinates']);
    $url = url($path, array('query' => $search_query));
    $build['nearby'] =  array(
      '#theme' => 'culturefeed_search_active_filters_item',
      '#label' => 'In de buurt van ' . check_plain($query['nearby']),
      '#url' => $url
    );
  }
  
  // Check the actor key.
  if (isset($query['actor'])) {
    $actor = culturefeed_agenda_actor_load($query['actor']);
    $search_query = $query;
    unset($search_query['actor']);
    $url = url($path, array('query' => $search_query));
    $build['nearby'] =  array(
      '#theme' => 'culturefeed_search_active_filters_item',
      '#label' => 'In de buurt van ' . check_plain($actor->getTitle(culturefeed_search_get_preferred_language())),
      '#url' => $url
    );
  }

  // Check if user is searching on a location.
  if (isset($query['location'])) {
    $search_query = $query;
    unset($search_query['location']);
    $url = url($path, array('query' => $search_query));
    $build['location'] =  array(
      '#theme' => 'culturefeed_search_active_filters_item',
      '#label' => check_plain($query['location']),
      '#url' => $url
    );
  }

  if (!empty($build) && count($build) > 1) {
    $build['reset'] = array('#markup' => l('Alle filters verwijderen', $path, array('attributes' => array('class' => array('delete-all')))));
  }

  // Suggestions if available.
  if ($culturefeedSearchResult instanceof \CultuurNet\Search\SearchResult) {
    $suggestions_keys = $culturefeedSearchResult->getSuggestions();
    if (!empty($suggestions_keys)) {
      $search_query = $query;
      $suggestions = array();
      foreach ($suggestions_keys as $suggestion) {
        $search_query['search'] = $suggestion;
        $suggestions[$suggestion] = url($path, array('query' => $search_query));
      }
      
      $build['suggestions'] =  array(
        '#theme' => 'culturefeed_search_suggestions',
        '#suggestions' => $suggestions
      );
    }
    
  }

  /*$build['item_search'] =  array(
    '#theme' => 'culturefeed_search_active_filters_item',
    '#label' => $query['search'],
    '#url' => $url
  );*/

  return $build;

}