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
      'search_url' => url('agenda/search', array('query' => culturefeed_saved_search_get_search_url_query($search))),
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

  foreach ($searches as $search) {
    $item = array(
      'title' => check_plain($search->name),
      'search_url' => url('agenda/search', array('query' => culturefeed_saved_search_get_search_url_query($search))),
    );
    $items[] = $item;
  }

  $vars['items'] = $items;

}
