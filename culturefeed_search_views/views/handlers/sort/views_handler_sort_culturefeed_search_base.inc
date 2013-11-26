<?php

/**
 * @class
 * Base handler class for sorting on search api.
 */
class views_handler_sort_culturefeed_search_base extends views_handler_sort {

  protected $sort_field = '';

  /**
   * @see views_handler_sort::can_expose()
   */
  function can_expose() {
    return FALSE;
  }

  /**
   * @see views_handler_sort::query()
   */
  function query() {
    $parameter = new \CultuurNet\Search\Parameter\Sort($this->sort_field, $this->options['order']);
    $this->query->add_parameter($parameter);
  }

  /**
   * @see views_handler_sort::option_definition()
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['order'] = array('default' => 'desc');

    return $options;
  }

  /**
   * @see views_handler_sort::admin_summary()
   */
  function admin_summary() {
    return $this->options['order'];
  }

}