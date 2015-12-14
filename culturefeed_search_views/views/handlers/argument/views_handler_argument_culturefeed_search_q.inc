<?php
/**
 * @class
 * Handler class to handle filtering on normal query string.
 */
class views_handler_argument_culturefeed_search_q extends views_handler_argument {

  private $parameter_type = 'q';

  /**
   * @see views_handler_argument::default_actions()
   */
  function default_actions($which = NULL) {
    $defaults = parent::default_actions();

    // Remove summary option
    unset($defaults['summary']);

    if ($which) {
      if (!empty($defaults[$which])) {
        return $defaults[$which];
      }
    }
    else {
      return $defaults;
    }
  }

  /**
   * @see views_handler_argument::query()
   */
  function query($group_by = FALSE) {
    $search_string = _culturefeed_search_ui_sanitize_query_term($this->argument);
    $this->query->add_where(0, $this->parameter_type, $search_string);
  }

}
