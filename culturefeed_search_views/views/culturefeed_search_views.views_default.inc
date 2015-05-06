<?php

/**
 * Implements hook_views_default_views().
 **/
function culturefeed_search_views_views_default_views() {
  $files = file_scan_directory(drupal_get_path('module', 'culturefeed_search_views'). '/views/views', '/.inc/');
  foreach ($files as $filepath => $file) {
    require $filepath;
    if (isset($view)) {
      $views[$view->name] = $view;
    }
  }
  if ($views) {
    return $views;
  }
}