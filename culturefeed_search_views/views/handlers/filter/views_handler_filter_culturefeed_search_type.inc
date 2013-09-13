<?php
/**
 * @class
 * Handler class to handle filtering on type.
 */
class views_handler_filter_culturefeed_search_type extends views_handler_filter_culturefeed_search_in {

  /**
   * @see views_handler_filter::init()
   */
  function init(&$view, &$options) {

    parent::init($view, $options);
    $this->search_field = 'type';

  }

  /**
   * @see views_handler_filter_in_operator::value_form()
   */
  function value_form(&$form, &$form_state) {
    $form['value'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Type'),
      '#default_value' => $this->value,
      '#options' => $this->get_value_options(),
    );
  }

  /**
   * @see views_handler_filter_in_operator::get_value_options()
   */
  function get_value_options() {
    $this->value_options = array(
      'event' => 'Event',
      'production' => 'Production',
      'actor' => 'Actor',
    );
    return $this->value_options;
  }

}