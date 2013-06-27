<?php
/**
 * @class
 * Handler class to handle filtering ageform.
 */
class views_handler_filter_culturefeed_search_agefrom extends views_handler_filter_in_operator {

  /**
   * @see views_handler_filter_in_operator::value_form()
   */
  function value_form(&$form, &$form_state) {

    $categories = culturefeed_search_get_categories_by_domain($this->domain_id);

    $form['value'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Agefrom'),
      '#default_value' => $this->value,
      '#options' => $this->get_value_options(),
    );
  }

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
   * @see views_handler_filter_in_operator::query()
   */
  function query() {

    $value = 'agefrom' . ':(' . implode(' OR ', $this->value) . ')';
    $this->query->add_where($this->options['group'], 'fq', $value);

  }

  /**
   * @see views_handler_filter_in_operator::get_value_options()
   */
  function get_value_options() {
    $this->value_options = array_slice(range(0, 99), 1, 99, TRUE);
    return $this->value_options;
  }

  /**
   * @see views_handler_filter_in_operator::operators()
   */
  function operators() {

    $operators = array(
      'OR' => array(
        'title' => t('Contains'),
        'values' => 1,
        'short' => t('Contains'),
      ),
    );

    return $operators;
  }

}