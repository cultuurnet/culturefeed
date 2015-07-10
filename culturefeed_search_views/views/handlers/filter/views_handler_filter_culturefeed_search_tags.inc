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
		$this->operator = $this->options['operator'];
		for ($i = 0; $i < 10; $i++) {
			$this->keyword[$i]['inverse'] = $this->options['keyword'][$i]['not'];
			$this->keyword[$i]['value'] = $this->options['keyword'][$i]['value'];
		}
  }

	/**
	 * @see views_handler_filter::option_definition()
	 */
	function option_definition() {

		$options = parent::option_definition();
		$options['operator'] = array('default' => 'or');
		for ($i = 0; $i < 10; $i++) {
			$options['keyword'][$i]['not'] = array('default' => FALSE);
			$options['keyword'][$i]['value'] = array('default' => '');
		}

		return $options;

	}

  /**
   * @see views_handler_filter::value_form()
   */
  function value_form(&$form, &$form_state) {

		$form['operator'] = array(
			'#title' => t('How to apply filters on the query'),
			'#description' => t('Select the operator for the search filters'),
			'#type' => 'radios',
			'#options' => array(
				'and' => t('Use AND-operator'),
				'or' => t('Use OR-operator'),
			),
			'#default_value' => $this->operator,
		);

		for ($i = 0; $i < 10; $i++) {

				$form['keyword'][$i]['value'] = array(
					'#title' => t('Keyword'),
					'#type' => 'textfield',
					'#default_value' => $this->keyword[$i]['value'],
					'#maxlength' => 255,
					'#attributes' => array(
						'style' => 'float:left;'
					),
				);
				$form['keyword'][$i]['not'] = array(
					'#title' => t('Inverse'),
					'#type' => 'checkbox',
					'#default_value' => $this->keyword[$i]['inverse'],
					'#attributes' => array(
						'style' => 'float:left;'
					),
				);
		}
  }

  /**
   * @see views_handler_filter::query()
   */
  function query() {

		$operator = $this->operator;
		$keywords = array();
		$keywords_inverse = array();
		for ($i = 0; $i < 10; $i++) {
			if (!empty($this->keyword[$i]['value']) && $this->keyword[$i]['inverse'] == 1) {
				$keywords_inverse[$i] = $this->keyword[$i]['value'];
			}
			else if (!empty($this->keyword[$i]['value'])) {
				$keywords[$i] = $this->keyword[$i]['value'];
			}
		}

		if ($operator == 'and') {
			if (!empty($keywords)) {
				$value_keywords = '(keywords:"' . implode('" AND "', $keywords) . '")';
				$value = $value_keywords;
			}
			if (!empty($keywords_inverse)) {
				$value_inverse_keywords = '(!keywords:"' . implode('" AND "', $keywords_inverse) . '")';
				if (!empty($keywords)) {
					$value .= '" AND "' . $value_inverse_keywords;
				}
				else {
					$value = $value_inverse_keywords;
				}
			}
		}
		else if ($operator == 'or') {
			if (!empty($keywords)) {
				$value_keywords = '(keywords:"' . implode('" OR "', $keywords) . '")';
				$value = $value_keywords;
			}
			if (!empty($keywords_inverse)) {
				$value_inverse_keywords = '(!keywords:"' . implode('" OR "', $keywords_inverse) . '")';
				if (!empty($keywords)) {
					$value .= '" OR "' . $value_inverse_keywords;
				}
				else {
					$value = $value_inverse_keywords;
				}
			}
		}

    if (isset($value)) {
      $parameter = new \CultuurNet\Search\Parameter\FilterQuery($value);
      $this->query->add_parameter($parameter);
    }

  }

	/**
	 * @see views_handler_filter::admin_summary()
	 */
	function admin_summary() {
		$keywords = array();
		foreach ($this->keyword as $keyword) {
			if ($keyword['value'] != '') {
				$keywords[] = $keyword['value'];
			}
		}
		return implode(', ', $keywords);
	}

}