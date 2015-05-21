<?php
/**
 * @class
 * Handler class to handle filtering via the culturefeed search api by using a filter query.
 */
class views_handler_filter_culturefeed_search_hasimage extends views_handler_filter_culturefeed_search_filter_query {

	/**
	 * @see views_handler_filter::init()
	 */
	function init(&$view, &$options) {
		parent::init($view, $options);
		$this->search_field = 'hasimage';
		$this->value = TRUE;
	}

	/**
	 * @see views_handler_filter::value_form()
	 */
	function value_form(&$form, &$form_state) {
		// Empty form.
	}

	/**
	 * @see views_handler_filter::admin_summary()
	 */
	function admin_summary() {
		return '';
	}

	/**
	 * @see views_handler_filter::can_expose()
	 */
	function can_expose() {
		return FALSE;
	}

}