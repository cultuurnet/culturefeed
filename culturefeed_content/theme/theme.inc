<?php

/**
 * @file
 * Theme / preprocess functions for the culturefeed content module.
 */

/**
 * Preprocess the result list of a culturefeed content field.
 *
 * @param array $variables
 */
function template_preprocess_culturefeed_content_result_list(array &$variables) {

  $variables['wrapper_id'] = drupal_html_id('culturefeed-content-result-' . $variables['instance_id']);
  $variables['items'] = [];

  $search_results = $variables['search_result']->getItems();
  foreach ($search_results as $search_result) {
    $variables['items'][] = theme('culturefeed_' . $search_result->getType() . '_summary', ['item' => $search_result]);
  }

  $variables['show_more_url'] = url('agenda/search', ['query' => $variables['query']]);


  if (empty($variables['title'])) {
    $variables['title'] = t('Our selection');
  }

}

/**
 * Returns HTML for the empty text.
 *
 * @param array $variables
 *   The variables.
 *
 * @return string
 *   The HTML.
 */
function theme_culturefeed_content_empty_text(array $variables) {

  $build = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => $variables['classes'],
    ),
    'empty_text' => array(
      '#markup' => $variables['empty_text'],
    ),
  );

  return drupal_render($build);

}
