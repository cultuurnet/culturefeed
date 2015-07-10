<?php

/**
 * A handler to provide proper displays for the categories of a cdb item.
 */
class views_handler_field_cdb_item_category extends views_handler_field {

	/**
	 * @see views_handler_field::option_definition()
	 */
	function option_definition() {
		$options = parent::option_definition();

    $options['type'] = array('default' => 'eventtype');
		$options['link'] = array('default' => FALSE);

		return $options;
	}

	/**
	 * @see views_handler_field::options_form()
	 */
	function options_form(&$form, &$form_state) {

    $form['type'] = array(
      '#type' => 'radios',
      '#title' => t('Type'),
      '#description' => t('What kind of categories do you want to show?'),
      '#options' => array(
        'theme' => t('Theme'),
        'publicscope' => t('Public scope'),
        'targetaudience' => t('Target audience'),
        'eventtype' => t('Event type'),
        'actortype' => t('Actor type'),
        'educationfield' => t('Education field'),
        'educationlevel' => t('Education Level'),
        'facility' => t('Facility'),
        'flandersregion' => t('Flanders region'),
      ),
      '#default_value' => isset($this->options['format']) ? $this->options['format'] : 'image',
    );

		$form['link'] = array(
			'#type' => 'checkbox',
			'#title' => t('Link the category to the category result page'),
			'#default_value' => isset($this->options['link']) ? $this->options['link'] : FALSE,
		);

		parent::options_form($form, $form_state);

	}

  /**
   * @see views_handler_field::render()
   */
  function render($extended_entity) {

    if (!method_exists($extended_entity->getEntity(), 'getCategories')) {
      return;
    }

		$categories = culturefeed_agenda_get_category_list_of_item($this->options['type'], $extended_entity->getEntity());

    if (!$categories) {
      return;
    }

		$categories_names = array();

		if ($this->options['link']) {
			foreach ($categories['links'] as $category) {
				$categories_names[] = $category;
			}
			$categories_names = implode(', ', $categories_names);
		}
		else {
			foreach ($categories['text'] as $category) {
				$categories_names[] = $category;
			}
			$categories_names = implode(', ', $categories_names);
		}

		return $categories_names;

  }

}
