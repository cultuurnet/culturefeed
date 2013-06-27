<?php
/**
 * @class
 * Handler class to handle filtering on normal query string.
 */
class views_handler_filter_culturefeed_search_q extends views_handler_filter {

  function init(&$view, &$options) {
    parent::init($view, $options);
    // set the operator
    $this->operator = 'q';
  }

  function value_form(&$form, &$form_state) {
    $form['value'] = array(
      '#type' => 'textfield',
      '#title' => t('Value'),
      '#size' => 30,
      '#default_value' => $this->value,
    );
  }

    /**
   * Add this filter to the query.
   *
   * Due to the nature of fapi, the value and the operator have an unintended
   * level of indirection. You will find them in $this->operator
   * and $this->value respectively.
   */
  function query() {
    $this->query->add_where($this->options['group'], $this->operator, $this->value);
  }

}
