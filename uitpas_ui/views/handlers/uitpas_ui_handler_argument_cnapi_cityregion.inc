<?php

/**
 * @file
 * Definition of uitpas_ui_handler_argument_cnapi_cityregion.
 */

class uitpas_ui_handler_argument_cnapi_cityregion extends views_handler_argument {

  function init(&$view, &$options) {
    parent::init($view, $options);
  }

  /**
   * Add this argument to the query.
   */
  function query($group_by = FALSE) {
    $this->ensure_my_table();
    //our condition takes either a lid or a concatenated string of negated lids
    if (is_numeric($this->argument)) {
      $this->query->add_where(0, "$this->table_alias.$this->real_field", $this->argument, 'regio');
    }
    else {
      $args = explode('|', $this->argument);
      foreach ($args as $arg) {
        $this->query->add_where(0, "$this->table_alias.$this->real_field", $arg, 'regio');
      }
    }
  }
}