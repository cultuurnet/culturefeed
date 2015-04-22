<?php

/**
 * A handler to provide proper displays for the short description of a cdb item.
 */
class views_handler_field_cdb_item_short_description extends views_handler_field {

  /**
   * @see views_handler_field::option_definition()
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['override'] = array('default' => FALSE);

    return $options;
	}

  /**
   * @see views_handler_field::options_form()
   */
  function options_form(&$form, &$form_state) {

    $form['override'] = array(
      '#type' => 'checkbox',
      '#title' => t('Override with collaboration data'),
      '#description' => t('If collaboration data is provided content of this field will be overriden when this is checked.'),
      '#default_value' => isset($this->options['override']) ? $this->options['override'] : FALSE,
    );

    parent::options_form($form, $form_state);
	}

  /**
   * @see views_handler_field::render()
   */
  function render($extended_entity) {

    if ($this->options['override']) {
      $short_description = 'This short description is overriden';
    }
    else {
      $details = $extended_entity->getEntity()->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language());
      if ($details) {
        $short_description = $details->getShortDescription();
      }
    }
    return $short_description;
  }

}
