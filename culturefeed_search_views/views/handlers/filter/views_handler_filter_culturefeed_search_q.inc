<?php
/**
 * @class
 * Handler class to handle filtering on normal query string.
 */
class views_handler_filter_culturefeed_search_q extends views_handler_filter {

  private $parameter_type = 'q';

  /**
   * @see views_handler_filter::value_form()
   */
  function value_form(&$form, &$form_state) {
    $form['value'] = array(
      '#type' => 'textfield',
      '#title' => t('Value'),
      '#size' => 30,
      '#default_value' => $this->value,
    );
  }

  /**
   * @see views_handler_filter::query()
   */
  function query() {
    $this->query->add_where($this->options['group'], $this->parameter_type, $this->value);
  }

}
