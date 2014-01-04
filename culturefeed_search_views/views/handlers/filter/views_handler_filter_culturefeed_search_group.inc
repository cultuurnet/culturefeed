<?php

/**
 * @class
 * Handler class to handle the grouping.
 */
class views_handler_filter_culturefeed_search_group extends views_handler_filter {

  /**
   * @see views_handler_filter::value_form()
   */
  function value_form(&$form, &$form_state) {
    $form['value'] = array(
      '#type' => 'textfield',
      '#title' => t('Group'),
      '#size' => 30,
      '#default_value' => $this->value,
    );
  }

  /**
   * @see views_handler_filter::query()
   */
  function query() {

    $value = $this->getValue();
    $value = '"' . str_replace('"', '\"', $value) . '"';

    $this->query->add_parameter(new \CultuurNet\Search\Parameter\Group($value));
  }

  /**
   * Get the value to query.
   */
  function getValue() {
    return $this->value;
  }

}
