<?php
// @codingStandardsIgnoreFile

/**
 * @file
 */

class culturefeed_uitpas_views_handler_filter_boolean extends views_handler_filter_boolean_operator {

  function query() {
    if (!empty($this->value)) {
      $this->query->object->{$this->real_field} = $this->value == 1 ? TRUE : FALSE;
    }



    /*
    if (empty($this->value)) {

      if ($this->accept_null) {
        $or = db_or()
          ->condition($field, 0, '=')
          ->condition($field, NULL, 'IS NULL');
        $this->query->add_where($this->options['group'], $or);
      }
      else {
        $this->query->add_where($this->options['group'], $field, 0, '=');
      }
    }
    else {
      if (!empty($this->definition['use equal'])) {
        $this->query->add_where($this->options['group'], $field, 1, '=');
      }
      else {
        $this->query->add_where($this->options['group'], $field, 0, '<>');
      }
    }*/
  }
}
