<?php

/**
 * @class
 * Handler class for sorting on availableto.
 */
class views_handler_sort_culturefeed_search_availableto extends views_handler_sort_culturefeed_search_base {

  /**
   * @see views_handler::init()
   */
  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->sort_field = 'availableto';
  }

}