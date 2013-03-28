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
    $options[$type['path']] = $type['title'];
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
  
  drupal_goto($form_state['values']['type'], array('query' => $query));
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
function culturefeed_search_ui_search_facets_block() {
  // This currently means the page callback has already been invoked.
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

/**
 * Block callback for the active filters during search.
 */
function culturefeed_search_ui_search_active_filters_block() {
  // This currently means the page callback has already been invoked.
  global $facetingComponent;
  
  if (!$facetingComponent instanceof \CultuurNet\Search\Component\Facet\FacetComponent) {
    return '';
  }
  
  $output = array(
    '#type' => 'container',
    '#attributes' => array(),
  );
  
  $query = drupal_get_query_parameters(NULL, array('q', 'page'));
  $path = current_path();

  foreach ($facetingComponent->getFacets() as $facet) {
    $items = $facet->getResult()->getItems();
    if (!empty($items)) {
      
      $active_options = isset($query['facet'][$facet->getKey()]) ? $query['facet'][$facet->getKey()] : array();

      $facetListItems = array();
      
      foreach ($facet->getResult()->getItems() as $option => $number) {
        $facet_query = $query;
        $selected = in_array($option, $active_options);
        if ($selected) {
          // Remove this facet from URL, URL will function as removal URL.
          $key = array_search($option, $facet_query['facet'][$facet->getKey()]);
          if (FALSE !== $key) {
            unset($facet_query['facet'][$facet->getKey()][$key]);
          }
          
          $url = url($path, array('query' => $facet_query, 'absolute' => TRUE));
          $output['item_' . $number] = array(
            '#type' => 'markup',
            '#markup' => '<span class="facet_item">' . check_plain($option) . '</span> [' . l('x', $url) . ']'
          );
        }
        
      }
    }
  }
  
  return $output;
}
