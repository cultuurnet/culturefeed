<?php
/**
 * @class
 * Handler class to handle filtering on a cdb category.
 */
class views_handler_filter_culturefeed_search_category extends views_handler_filter_in_operator {

  private $domain_id;

  /**
   * @see views_handler_filter::init()
   */
  function init(&$view, &$options) {

    parent::init($view, $options);

    $id_parts = explode('_', $options['id']);
    $this->domain_id = $id_parts[1];

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
   * @see views_handler_filter_in_operator::value_form()
   */
  function value_form(&$form, &$form_state) {

    $categories = culturefeed_search_get_categories_by_domain($this->domain_id);

    $form['value'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Category to filter on'),
      '#default_value' => $this->value,
      '#options' => $this->get_value_options(),
    );
  }

  /**
   * @see views_handler_filter_in_operator::query()
   */
  function query() {

    $value = 'category_' . $this->domain_id . '_id' . ':(' . implode(' ' . $this->operator . ' ', $this->value) . ')';
    $this->query->add_where($this->options['group'], 'fq', $value);

  }

  /**
   * @see views_handler_filter_in_operator::get_value_options()
   */
  function get_value_options() {
    $this->value_options = culturefeed_search_get_categories_by_domain($this->domain_id);
    return $this->value_options;
  }

  /**
   * @see views_handler_filter_in_operator::operators()
   */
  function operators() {

    $operators = array(
      'AND' => array(
        'title' => t('Contains all selected categories'),
        'values' => 1,
        'short' => t('Has category')
      ),
      'OR' => array(
        'title' => t('Contains at least 1 of the selected categories'),
        'values' => 1,
        'short' => t('Contains category'),
      ),
    );

    return $operators;

  }

}