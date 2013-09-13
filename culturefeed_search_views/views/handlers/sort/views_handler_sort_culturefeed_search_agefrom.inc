<?php

/**
 * @class
 * Handler class for sorting on agefrom.
 */
class views_handler_sort_culturefeed_search_agefrom extends views_handler_sort_culturefeed_search_base {

  /**
   * @see views_handler::init()
   */
  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->sort_field = 'agefrom';
  }

  /**
   * @see views_handler_sort::option_definition()
   */
  function option_definition() {

    $options = parent::option_definition();
    $options['direction'] = array('default' => 'asc');

    return $options;

  }

}