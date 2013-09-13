<?php
/**
 * @class
 * Handler class to handle filtering on a city.
 */
class views_handler_filter_culturefeed_search_city extends views_handler_filter_culturefeed_search_filter_query {

  /**
   * @see views_handler_filter::init()
   */
  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->search_field = 'zipcode';
  }

  /**
   * @see views_handler_filter::value_form()
   */
  function value_form(&$form, &$form_state) {
    $form['value'] = array(
      '#type' => 'textfield',
      '#title' => t('City'),
      '#size' => 30,
      '#default_value' => $this->value,
      '#autocomplete_path' => 'autocomplete/culturefeed/city-suggestion',
    );
  }

  /**
   * @see views_handler_filter_culturefeed_search_filter_query::getValue()
   */
  function getValue() {
    $city_parts = explode(' ', $this->value);
    return $city_parts[0];
  }

}