<?php

/**
 * @class
 * Handler class for sorting on recommend count.
 */
class views_handler_sort_culturefeed_search_recommends extends views_handler_sort_culturefeed_search_base {

  /**
   * @see views_handler::init()
   */
  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->sort_field = 'recommend_count';
  }

}