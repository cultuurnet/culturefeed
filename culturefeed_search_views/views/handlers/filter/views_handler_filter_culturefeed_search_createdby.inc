<?php
/**
 * @class
 * Handler class to handle filtering createdby.
 */
class views_handler_filter_culturefeed_search_createdby extends views_handler_filter_culturefeed_search_filter_query {

  /**
   * @see views_handler_filter::init()
   */
  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->search_field = 'createdby';

  }

  /**
   * @see views_handler_filter_in_operator::value_form()
   */
  function value_form(&$form, &$form_state) {
    $form['value'] = array(
      '#type' => 'textfield',
      '#title' => t('Created by'),
      '#default_value' => $this->value,
    );
  }

}