<?php

/**
 * @class
 * Handler class to handle the grouping.
 */
class views_handler_filter_culturefeed_search_group extends views_handler_filter {

  /**
   * @see views_handler_filter::value_form()
   */
  function value_form(&$form, &$form_state) {
    $form['value'] = array(
      '#type' => 'radios',
      '#title' => t('Group'),
      '#options' => array(
        'event' => 'event',
        'event_production' => 'event_production',
      ),
      '#description' => t('If one or more events are related to a production, they can be grouped by production by choosing "event_production".'),
      '#default_value' => isset($this->value) ? $this->value : 'event',
    );
  }

  /**
   * @see views_handler_filter::query()
   */
  function query() {

    $value = $this->getValue();
    // Removed this line for UIV-1225.
    //$value = '"' . str_replace('"', '\"', $value) . '"';

    $this->query->add_parameter(new \CultuurNet\Search\Parameter\Group($value));
  }

  /**
   * Get the value to query.
   */
  function getValue() {
    return $this->value;
  }

}
