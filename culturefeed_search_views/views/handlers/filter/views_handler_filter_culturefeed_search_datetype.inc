<?php
/**
 * @class
 * Handler class to handle filtering on a type of date
 */
class views_handler_filter_culturefeed_search_datetype extends views_handler_filter {

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

    $form['value'] = array(
      '#type' => 'select',
      '#title' => t('Date type'),
      '#default_value' => $this->value,
      '#options' => $this->get_value_options(),
    );
  }

  /**
   * @see views_handler_filter_in_operator::get_value_options()
   */
  function get_value_options() {
    $this->value_options = culturefeed_search_ui_get_date_facet_options();
    $this->value_options['permanent'] = t('Permanent');
    return $this->value_options;
  }

  /**
   * @see views_handler_filter_in_operator::query()
   */
  function query() {

    $parameter = new \CultuurNet\Search\Parameter\DateTypeQuery($this->value);
    $this->query->add_parameter($parameter);

  }

  /**
   * Display the filter on the administrative summary
   */
  function admin_summary() {
    return check_plain((string) $this->value);
  }

}