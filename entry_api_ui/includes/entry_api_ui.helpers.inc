<?php
/**
 * @file
 * Helper functions for entry api UI.
 */

function entry_api_ui_get_category_options($section) {

  $options = array();
  $categories = _cnapi_get_section_categories($section, CNAPI_DIMENSION_EVENTTYPE);
  foreach ($categories as $category) {
    $options[$category['cid']] = $category['name'];
  }

  return $options;

}