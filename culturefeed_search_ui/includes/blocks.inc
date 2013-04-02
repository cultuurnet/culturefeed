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
  $query = drupal_get_query_parameters(NULL, array('q', 'page'));
  $default = isset($query['search']) ? $query['search']: '';

  $searchable_types = culturefeed_get_searchable_types();

  $options = array();
  foreach ($searchable_types as $key => $type) {
    $options[$type['path']] = $type['search_title'];
  }

  $form['type'] = array(
    '#type' => 'select',
    '#title' => 'Zoeken in',
    '#default_value' => key($options),
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

  return $form;
}

/**
 * Submit function for a search box in all searchable types.
 *
 * @param array $form
 * @param array $form_state
 */
function culturefeed_search_ui_search_block_form_submit($form, & $form_state) {
  // When changing sort method, we remove the page parameter.
  $query = drupal_get_query_parameters(NULL, array('q', 'page'));
  $query['search'] = $form_state['values']['search'];

  $form_state['redirect'] = array(
    $form_state['values']['type'],
    array('query' => $query),
  );
}

/**
 * Creates a form to change the sortorder of the search results.
 *
 * @param array $form
 * @param array $form_state
 * @return array
 */
function culturefeed_search_ui_search_sortorder_form($form, & $form_state) {
  $query = drupal_get_query_parameters();

  $default = isset($query['sort']) ? $query['sort'] : 'relevancy';

  $form['sort'] = array(
    '#title' => t('Sorteer resultaten op'),
    '#type' => 'select',
    '#options' => array(
      'relevancy' => t('Relevantie'),
      'date' => t('Datum'),
      'recommends' => t('Aantal keer aangeraden'),
      'comments' => t('Aantal beoordelingen'),
    ),
    '#default_value' => $default,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  $form['#attached']['js'][] = drupal_get_path('module', 'culturefeed_search_ui') . '/js/search_ui.js';

  return $form;
}

/**
 * Submits and changes the sortorder of the search results.
 *
 * @param array $form
 * @param array $form_state
 * @return array
 */
function culturefeed_search_ui_search_sortorder_form_submit($form, & $form_state) {

  // When changing sort method, we remove the page parameter.
  $query = drupal_get_query_parameters(NULL, array('q', 'page'));
  $query['sort'] = $form_state['values']['sort'];

  drupal_goto($_GET['q'], array('query' => $query));
}

/**
 * Renders a facet block to go along with the search form.
 * @return string
 */
function culturefeed_search_ui_search_facets_block($facet_name, $domain_id = '') {
  // This currently means the page callback has already been invoked.
  global $facetingComponent;

  if (!$facetingComponent instanceof \CultuurNet\Search\Component\Facet\FacetComponent) {
    return;
  }

  $build = array();

  $facets = $facetingComponent->getFacets();
  if (!isset($facets[$facet_name])) {
    return;
  }

  $facet = $facets[$facet_name];

  $items = $facet->getResult()->getItems();

  if (!empty($items)) {

    $subject = '';
    if (!empty($domain_id)) {
      $subject = db_query('SELECT label FROM {culturefeed_search_domains} WHERE did = :did', array(':did' => $domain_id))->fetchField();
    }

    return array(
      'subject' => $subject,
      'content' => array(
        '#theme' => 'culturefeed_search_facet',
        '#facet' => $facet,
      ),
    );
  }

}

/**
 * Block callback for the active filters during search.
 */
function culturefeed_search_ui_search_active_filters_block() {
  // This currently means the page callback has already been invoked.
  global $facetingComponent;

  if (!$facetingComponent instanceof \CultuurNet\Search\Component\Facet\FacetComponent) {
    return '';
  }

  $query = drupal_get_query_parameters(NULL, array('q', 'page'));
  $path = current_path();
  $build = array();

  // Check the current search term.
  if (!empty($query['search'])) {
    $search_query = $query;
    unset($search_query['search']);
    $url = url($path, array('query' => $search_query, 'absolute' => TRUE));
    $build['item_search'] =  array(
      '#theme' => 'culturefeed_search_active_filters_item',
      '#label' => check_plain($query['search']),
      '#url' => $url
    );
  }

  // Check the facets.
  if (isset($query['facet'])) {

    $facets = $facetingComponent->getFacets();
    foreach ($query['facet'] as $active_facet_name => $selected_value) {

      $search_query = $query;
      unset($search_query['facet'][$active_facet_name]);
      $url = url($path, array('query' => $search_query, 'absolute' => TRUE));
      $build['item_' . $active_facet_name] = array(
        '#theme' => 'culturefeed_search_active_filters_item',
        '#label' => check_plain($selected_value[0]),
        '#url' => $url
      );

    }

  }

  return $build;

}
