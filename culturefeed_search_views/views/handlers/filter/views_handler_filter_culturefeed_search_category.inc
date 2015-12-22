<?php
/**
 * @class
 * Handler class to handle filtering on a cdb category.
 */
class views_handler_filter_culturefeed_search_category extends views_handler_filter_culturefeed_search_in {

  private $domain_id;

  /**
   * @see views_handler_filter::init()
   */
  function init(&$view, &$options) {

    parent::init($view, $options);

    $id_parts = explode('_', $options['id']);
    $this->domain_id = $id_parts[1];
    $this->search_field = 'category_id';

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
      'NOT IN' => array(
        'title' => t('Is not one of'),
        'short' => t('not in'),
        'values' => 1,
      ),
    );

    return $operators;

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
   * @see views_handler_filter_in_operator::get_value_options()
   */
  function get_value_options() {
    $this->value_options = culturefeed_search_get_categories_by_domain($this->domain_id);
    return $this->value_options;
  }

  /**
   * @see views_handler_filter_in_operator::query()
   */
  function query() {

    $values = $this->value;
    array_walk($values, function (&$item) {
      $item = '"' . str_replace('"', '\"', $item) . '"';
    });

    if ($this->operator == 'NOT IN') {
      $value = '!' . $this->search_field . ':(' . implode(' OR ', $values) . ')';
    }else {
      $value = $this->search_field . ':(' . implode(' ' . $this->operator . ' ', $values) . ')';
    }

    $this->query->add_where($this->options['group'], 'fq', $value);

  }

}