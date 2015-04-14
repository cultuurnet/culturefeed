<?php
/**
 * @class
 * Handler class to handle filtering on normal query string.
 */
class views_handler_filter_culturefeed_search_daterange extends views_handler_filter {

  private $option_absolute;
	private $start_date;
  private $end_date;
	private $relative_date;

  /**
   * @see views_handler_filter::init()
   */
  function init(&$view, &$options) {
    parent::init($view, $options);
		$this->option_absolute = $this->options['option_absolute'];
    $this->start_date = $this->options['start_date'];
    $this->end_date = $this->options['end_date'];
		$this->relative_date = $this->options['relative_date'];
  }

  /**
   * @see views_handler_filter::option_definition()
   */
  function option_definition() {

    $options = parent::option_definition();
		$options['option_absolute'] = array('default' => 0);
    $options['start_date'] = array('default' => '');
    $options['end_date'] = array('default' => '');
		$options['relative_date'] = array('default' => '');

    return $options;

  }

  /**
   * @see views_handler_filter::value_form()
   */
  function value_form(&$form, &$form_state) {
		$form['option_absolute'] = array(
			'#type' => 'radios',
			'#title' => t('Absolute or relative date'),
			'#default_value' => $this->option_absolute,
			'#options' => array(
				0 => t('Absolute'),
				1 => t('Relative')
			),
			'#description' => t('Do you want to use an absolute start date or a relative one?'),
		);
    $form['start_date'] = array(
      '#type' => 'textfield',
      '#title' => t('Start date'),
      '#size' => 30,
      '#default_value' => $this->start_date,
      '#description' => t('Format: !date_format', array('!date_format' => date('d/m/Y'))),
      //'#required' => TRUE,
			'#states' => array(
				'visible' => array(
					':input[name="options[option_absolute]"]' => array('value' => 0),
				),
			),
    );
    $form['end_date'] = array(
      '#type' => 'textfield',
      '#title' => t('End date'),
      '#size' => 30,
      '#default_value' => $this->end_date,
      '#description' => t('Format: !date_format', array('!date_format' => date('d/m/Y'))),
			'#states' => array(
				'visible' => array(
					':input[name="options[option_absolute]"]' => array('value' => 0),
				),
			),
    );
		$form['relative_date'] = array(
			'#type' => 'textfield',
			'#title' => t('Relative date'),
			'#size' => 30,
			'#default_value' => $this->relative_date,
			'#description' => t('A relative date starting from now. For eg. +2 weeks, tomorrow.'),
			'#states' => array(
				'visible' => array(
					':input[name="options[option_absolute]"]' => array('value' => 1),
				),
			),
		);
  }

  /**
   * @see views_handler_filter::value_validate()
   */
  function value_validate($form, &$form_state) {

		$optionAbsolute = $form_state['values']['options']['option_absolute'];

		if ($optionAbsolute == 0) {
			if (!empty($form_state['values']['options']['start_date'])) {
				$startDate = DateTime::createFromFormat('d/m/Y', $form_state['values']['options']['start_date']);
				if (!$startDate) {
					form_set_error('start_date', t('The entered start date is not in the correct format'));
				}
			}

			if (!empty($form_state['values']['options']['end_date'])) {
				$endDate = DateTime::createFromFormat('d/m/Y', $form_state['values']['options']['end_date']);
				if (!$endDate) {
					form_set_error('start_date', t('The entered end date is not in the correct format'));
				}
			}
		}
		else if ($optionAbsolute == 1) {
			if (!empty($form_state['values']['options']['relative_date'])) {
				$now = new DateTime();
				$rel_start_date = clone $now;
				$rel_start_date->modify($this->relative_date);

				if (!$rel_start_date) {
					form_set_error('relative_date', t('The entered relative date is not in the correct format'));
				}
			}
		}
  }

  /**
   * @see views_handler_filter::admin_summary()
   */
  function admin_summary() {

		if ($this->option_absolute == 0) {
			if (!empty($this->end_date)) {
				return check_plain(t('Between @start_date and @end_date', array(
					'@start_date' => $this->start_date,
					'@end_date' => $this->end_date
				)));
			}
			else {
				return check_plain($this->start_date);
			}
		}
		else if ($this->option_absolute == 1) {
			if (!empty($this->relative_date)) {
				$now = new DateTime();
				$rel_start_date = clone $now;
				$rel_start_date->modify($this->relative_date);

				return check_plain(t('Starting from @relative_start_date', array(
					'@relative_start_date' => $rel_start_date->format('d/m/Y'),
				)));
			}
		}
  }

  /**
   * @see views_handler_filter::query()
   */
  function query() {

		if ($this->option_absolute == 0) {
			$startDate = DateTime::createFromFormat('d/m/Y', $this->start_date);
			$endDate = $startDate;
			if (!empty($this->end_date)) {
				$endDate = DateTime::createFromFormat('d/m/Y', $this->end_date);
			}

			// Set start date time on beginning of the day.
			if (!empty($startDate)) {
				$startDate->setTime(0, 0, 1);
			}

			// Set end date time to end of the day day, to it searches on full day.
			if (!empty($endDate)) {
				$endDate->setTime(23, 59, 59);
			}

			if (!empty($startDate) && !empty($endDate)) {
				$parameter = new \CultuurNet\Search\Parameter\DateRangeFilterQuery('startdate', $startDate->getTimestamp(), $endDate->getTimestamp());
			}

			if (isset($parameter)) {
				$this->query->add_parameter($parameter);
			}
		}
		else if ($this->option_absolute == 1) {
			$now = new DateTime();
			$rel_start_date = clone $now;
			$rel_start_date->modify($this->relative_date);
			$rel_start_date->format('d/m/Y');

			// Set start date time on beginning of the day.
			$rel_start_date->setTime(0, 0, 1);

			$parameter = new \CultuurNet\Search\Parameter\DateRangeFilterQuery('startdate', $rel_start_date->getTimestamp(), '*');

			$this->query->add_parameter($parameter);
		}

  }

}
