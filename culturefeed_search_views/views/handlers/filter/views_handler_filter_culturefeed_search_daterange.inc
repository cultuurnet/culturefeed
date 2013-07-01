<?php
/**
 * @class
 * Handler class to handle filtering on normal query string.
 */
class views_handler_filter_culturefeed_search_daterange extends views_handler_filter {

  private $start_date;
  private $end_date;

  /**
   * @see views_handler_filter::init()
   */
  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->start_date = $this->options['start_date'];
    $this->end_date = $this->options['end_date'];
  }

  /**
   * @see views_handler_filter::option_definition()
   */
  function option_definition() {

    $options = parent::option_definition();
    $options['start_date'] = array('default' => '');
    $options['end_date'] = array('default' => '');

    return $options;

  }

  /**
   * @see views_handler_filter::value_form()
   */
  function value_form(&$form, &$form_state) {
    $form['start_date'] = array(
      '#type' => 'textfield',
      '#title' => t('Start date'),
      '#size' => 30,
      '#default_value' => $this->start_date,
      '#description' => t('Format: !date_format', array('!date_format' => date('d/m/Y'))),
      '#required' => TRUE,
    );
    $form['end_date'] = array(
      '#type' => 'textfield',
      '#title' => t('End date'),
      '#size' => 30,
      '#default_value' => $this->end_date,
      '#description' => t('Format: !date_format', array('!date_format' => date('d/m/Y'))),
    );
  }

  /**
   * @see views_handler_filter::value_validate()
   */
  function value_validate($form, &$form_state) {

    $startDate = DateTime::createFromFormat('d/m/Y', $form_state['values']['options']['start_date']);
    if (!$startDate) {
      form_set_error('start_date', t('The entered start date is not in the correct format'));
    }

    if (!empty($form_state['values']['options']['start_date'])) {
      $endDate = DateTime::createFromFormat('d/m/Y', $form_state['values']['options']['start_date']);
      if (!$endDate) {
        form_set_error('start_date', t('The entered end date is not in the correct format'));
      }
    }

  }

  /**
   * @see views_handler_filter::admin_summary()
   */
  function admin_summary() {

    if (!empty($this->end_date)) {
      return check_plain(t('Between @start_date and @end_date', array('@start_date' => $this->start_date, '@end_date' => $this->end_date)));
    }
    else {
      return check_plain($this->start_date);
    }

  }

  /**
   * @see views_handler_filter::query()
   */
  function query() {

    $startDate = DateTime::createFromFormat('d/m/Y', $this->start_date);
    $endDate = $startDate;
    if (!empty($this->end_date)) {
      $endDate = DateTime::createFromFormat('d/m/Y', $this->end_date);
    }

    // Set start date time on beginning of the day.
    $startDate->setTime(0, 0, 1);

    // Set end date time to end of the day day, to it searches on full day.
    $endDate->setTime(23, 59, 59);

    $parameter = new \CultuurNet\Search\Parameter\DateRangeFilterQuery('startdate', $startDate->getTimestamp(), $endDate->getTimestamp());

    $this->query->add_parameter($parameter);

  }

}
