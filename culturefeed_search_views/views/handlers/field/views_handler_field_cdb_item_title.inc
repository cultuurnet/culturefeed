<?php

use \CultuurNet\Search\Parameter;

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

    /* @var \CultuurNet\Search\ActivityStatsExtendedEntity $extended_entity */
    $original_title = $extended_entity->getTitle(culturefeed_search_get_preferred_language());
    $title = $original_title;

    if ($this->options['override']) {

      module_load_include('inc', 'culturefeed_search_views', 'views/helpers/culturefeed_search_views_helper_collaboration');
      $data = culturefeed_search_views_helper_collaboration_get_data($extended_entity, $this->view);
      if ($data && isset($data['title']) && $data['title']) {
        $title = $data['title'];
      }

    }

    if ($this->options['link']) {
      return culturefeed_search_detail_l($extended_entity->getType(), $extended_entity->getId(), $original_title, $title);
    }
    else {
      return $title;
    }

  }

  /**
   * Called to determine what to tell the clicksorter.
   */
  function click_sort($order) {

    $parameter = new Parameter\Sort('title_sort', strtolower($order));
    $this->query->add_parameter($parameter);

  }

}
