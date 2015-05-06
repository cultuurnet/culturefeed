<?php

/**
 * @file
 * Theming / preprocess functions for culturefeed_saved_searches.
 */

/**
 * Preprocess the saved searches manage list.
 * @see culturefeed-saved-searches-manage-list.tpl.php
 */
function culturefeed_saved_searches_preprocess_culturefeed_saved_searches_manage_list(&$vars) {

  $searches = $vars['searches'];
  $items = array();

  foreach ($searches as $search) {

    $item = array(
      'title' => check_plain($search->name),
      'form' => drupal_get_form('culturefeed_saved_searches_edit_frequency_form', $search),
      'id' => (int)$search->id,
      'delete_url' => url('culturefeed/searches/' . $search->id . '/delete'),
      'search_url' => url('agenda/search', array('query' => culturefeed_search_get_search_url_query($search->query))),
    );
    $items[] = $item;
  }

  $vars['items'] = $items;

}

/**
 * Preprocess the saved searches frontend list.
 * @see culturefeed-saved-searches-list.tpl.php
 */
function culturefeed_saved_searches_preprocess_culturefeed_saved_searches_list(&$vars) {

  $searches = $vars['searches'];
  $items = array();

  $vars['total_searches'] = count($searches);

  foreach ($searches as $search) {
    $item = array(
      'title' => check_plain($search->name),
      'search_url' => url('agenda/search', array('query' => culturefeed_search_get_search_url_query($search->query))),
    );
    $items[] = $item;
  }

  $vars['items'] = $items;

}

/**
 * Preprocess the saved searches CTA.
 */
function culturefeed_saved_searches_preprocess_culturefeed_saved_searches_cta(&$vars) {

  $vars['path'] = 'culturefeed/savesearch';

  if (!culturefeed_is_culturefeed_user()) {

    // If not logged in, go to authenticated page.
    $vars['query'] = array('destination' => $vars['path'] . '?' . drupal_http_build_query($vars['query']));
    $vars['path'] = 'authenticated';

  }

}

/**
 * Theme the saved searches CTA.
 */
function theme_culturefeed_saved_searches_cta($vars) {

  $text = '<h2>' . t('Save this search') . '</h2>' . check_plain($vars['text']);
  return l($text, $vars['path'], array('query' => $vars['query'], 'html' => TRUE));

}