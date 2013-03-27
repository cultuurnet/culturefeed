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
function culturefeed_agenda_search_block_form($form, & $form_state) {

  $searchable_types = culturefeed_get_searchable_types();

  $options = array();
  foreach ($searchable_types as $key => $type) {
    $options[$type['path']] = $type['title'];
  }
  //$form['#action'] = url($type['path'], array('absolute' => TRUE));
  
  $form['type'] = array(
    '#type' => 'select',
    '#title' => 'Zoeken in',
    '#default_value' => key($options),
    '#options' => $options,
  );
  
  $form['search'] = array(
    '#type' => 'textfield',
    '#title' => 'Zoekopdracht',
    '#default_value' => '',
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
function culturefeed_agenda_search_block_form_submit($form, & $form_state) {
  
  // When changing sort method, we remove the page parameter.
  $query = drupal_get_query_parameters(NULL, array('q', 'page'));
  $query['search'] = $form_state['values']['search'];
  
  drupal_goto($form_state['values']['type'], array('query' => $query));
}

/**
 * Creates a form to change the sortorder of the search results.
 * 
 * @param array $form
 * @param array $form_state
 * @return array
 */
function culturefeed_agenda_search_sortorder_form($form, & $form_state) {
  $query = drupal_get_query_parameters();
  
  if (isset($query['sort'])) {
    $default = $query['sort'];
  }
  else {
    $default = 'relevancy';
  }
  
  // @todo add JavaScript autosubmit to the select box
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
  
  return $form;
}

/**
 * Submits and changes the sortorder of the search results.
 *
 * @param array $form
 * @param array $form_state
 * @return array
 */
function culturefeed_agenda_search_sortorder_form_submit($form, & $form_state) {

  // When changing sort method, we remove the page parameter.
  $query = drupal_get_query_parameters(NULL, array('q', 'page'));
  $query['sort'] = $form_state['values']['sort'];
  
  drupal_goto($_GET['q'], array('query' => $query));
}

/**
 * Renders a facet block to go along with the search form.
 * @return string
 */
function culturefeed_agenda_search_facets_block() {
  global $facetingComponent;
  
  if (!$facetingComponent instanceof \CultuurNet\Search\Component\Facet\FacetComponent) {
    return '';
  }
  
  $output = array(
    '#type' => 'container',
    '#attributes' => array(),
  );
  
  foreach ($facetingComponent->getFacets() as $facet) {
    $items = $facet->getResult()->getItems();
    if (!empty($items)) {
      $output[$facet->getKey()] = array(
        '#type' => 'container',
        '#attributes' => array(),
      );
  
      // @todo maybe we should labeling this as an integral part to the search component?
      switch ($facet->getKey()) {
        case 'city':
          $label = t('Waar');
          break;
        case 'category_theme_name':
          $label = t('Thema');
          break;
        default:
          $label = $facet->getKey();
      }
  
      $output[$facet->getKey()]['name'] = array(
        '#type' => 'html_tag',
        '#tag' => 'h1',
        '#value' => $label,
      );
  
      // @todo check if we can use a render array as well for theme('item_list')
      $output[$facet->getKey()]['options'] = array(
        '#type' => 'markup',
        '#markup' => theme('culturefeed_search_facets_list', array('facet' => $facet)),
      );
    }
  }
  
  return $output;
}

