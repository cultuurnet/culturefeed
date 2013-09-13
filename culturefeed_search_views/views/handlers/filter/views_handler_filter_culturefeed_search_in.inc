<?php

/**
 * @file
 * Definition of views_handler_filter_in_operator.
 */

/**
 * Simple filter to handle matching of multiple options selectable via checkboxes for the culturefeed search.
 */
abstract class views_handler_filter_culturefeed_search_in extends views_handler_filter_in_operator {

  protected $search_field; // Field to search on.

  /**
   * @see views_handler_filter_in_operator::option_definition()
   */
  function option_definition() {

    $options = parent::option_definition();

    $options['operator']['default'] = 'OR';
    $options['value']['default'] = array();

    return $options;

  }

  /**
   * @see views_handler_filter_in_operator::operators()
   */
  function operators() {

    $operators = array(
      'AND' => array(
        'title' => t('Contains all selected values'),
        'values' => 1,
        'short' => t('Has value')
      ),
      'OR' => array(
        'title' => t('Contains at least 1 of the selected values'),
        'values' => 1,
        'short' => t('Contains value'),
      ),
    );

    return $operators;

  }

  /**
   * @see views_handler_filter_in_operator::query()
   */
  function query() {

    $values = $this->value;
    array_walk($values, function (&$item) {
      $item = '"' . str_replace('"', '\"', $item) . '"';
    });

    $value = $this->search_field . ':(' . implode(' ' . $this->operator . ' ', $values) . ')';
    $this->query->add_where($this->options['group'], 'fq', $value);

  }

}
