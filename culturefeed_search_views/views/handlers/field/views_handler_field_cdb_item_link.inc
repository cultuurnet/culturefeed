<?php

/**
 * A handler to provide proper displays for the picture of a cdb item.
 */
class views_handler_field_cdb_item_link extends views_handler_field {

  /**
   * @see views_handler_field::option_definition()
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['title'] = array('default' => t('Read more'));

    return $options;
  }

  /**
   * @see views_handler_field::options_form()
   */
  function options_form(&$form, &$form_state) {

    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#default_value' => isset($this->options['title']) ? $this->options['title'] : '',
      '#required' => TRUE,
    );

    parent::options_form($form, $form_state);

  }

  /**
   * @see views_handler_field::render()
   */
  function render($extended_entity) {

    $title = $extended_entity->getTitle(culturefeed_search_get_preferred_language());

    return culturefeed_search_detail_l($extended_entity->getType(), $extended_entity->getId(), $title, $this->options['title']);

  }

}
