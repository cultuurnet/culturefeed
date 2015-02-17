<?php
/**
 * @class
 * Handler class to handle filtering on a city.
 */
class views_handler_filter_culturefeed_search_tags extends views_handler_filter_culturefeed_search_filter_query {

  /**
   * @see views_handler_filter::init()
   */
  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->search_field = 'keywords';
  }

  /**
   * @see views_handler_filter::value_form()
   */
  function value_form(&$form, &$form_state) {
    $form['value'] = array(
      '#type' => 'textfield',
      '#title' => t('Keywords'),
      '#size' => 30,
      '#default_value' => $this->value,
      '#description' => t('For OR use this syntax: Mijnkindopkamp OR UiTPAS Regio Aalst. AND not supplied yet.'),
    );
  }
  
  /**
   * @see views_handler_filter::query()
   */
  function query() {

    $values = explode(' OR ', $this->getValue());
    $keywords = array();
    foreach ($values as $value) {
      $keywords[] = '"' . str_replace('"', '\"', $value) . '"';
    }
    $this->query->add_where($this->options['group'], 'fq', $this->search_field . ':' . implode(' OR ', $keywords));
  }

}