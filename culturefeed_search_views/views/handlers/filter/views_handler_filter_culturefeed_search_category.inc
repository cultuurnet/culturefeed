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

}