<?php
/**
 * @class
 * Handler class to handle filtering on a type of date
 */
class views_handler_filter_culturefeed_search_datetype extends views_handler_filter_culturefeed_search_in {

  /**
   * @see views_handler_filter::init()
   */
  function init(&$view, &$options) {

    parent::init($view, $options);

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
    $this->value_options = culturefeed_search_ui_get_date_facet_options();
    return $this->value_options;
  }

  /**
   * @see views_handler_filter_in_operator::query()
   */
  function query() {

    $value = '(' . implode(' ' . $this->operator . ' ', $this->value) . ')';
    $parameter = new  \CultuurNet\Search\Parameter\DateTypeQuery($value);
    $this->query->add_parameter($parameter);

  }

}