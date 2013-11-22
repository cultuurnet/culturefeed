<?php

/**
 * @class
 * Handler class for sorting on date.
 */
class views_handler_sort_culturefeed_search_date extends views_handler_sort_culturefeed_search_base {

  /**
   * @see views_handler::init()
   */
  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->sort_field = 'startdate';
  }

  /**
   * @see views_handler_sort::option_definition()
   */
  function option_definition() {

    $options = parent::option_definition();
    $options['order'] = array('default' => 'asc');

    return $options;

  }

}