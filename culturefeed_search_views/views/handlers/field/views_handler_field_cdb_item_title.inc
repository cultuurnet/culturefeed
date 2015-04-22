<?php

/**
 * A handler to provide proper displays for titles of a cdb item.
 */
class views_handler_field_cdb_item_title extends views_handler_field {

  /**
   * @see views_handler_field::option_definition()
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['override'] = array('default' => FALSE);
    $options['link'] = array('default' => FALSE);

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

    $form['link'] = array(
      '#type' => 'checkbox',
      '#title' => t('Link the title to the detail page'),
      '#default_value' => isset($this->options['link']) ? $this->options['link'] : FALSE,
    );

    parent::options_form($form, $form_state);

  }

  /**
   * @see views_handler_field::render()
   */
  function render($extended_entity) {

    if ($this->options['override']) {
      $title = 'This title is overriden';
    }
    else {
      $title = $extended_entity->getTitle(culturefeed_search_get_preferred_language());
    }

    if ($this->options['link']) {
      return culturefeed_search_detail_l($extended_entity->getType(), $extended_entity->getId(), $title, $title);
    }
    else {
      return $title;
    }

  }

}
