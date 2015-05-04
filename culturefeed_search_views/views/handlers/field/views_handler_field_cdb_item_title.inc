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
      $media = $extended_entity->getEntity()->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language())->getMedia();
      $roadmaps = $media->byMediaType('roadmap');

      $parameters = $this->query->getParameters();
      $consumer_id = variable_get('culturefeed_api_application_key');

      foreach ($parameters as $i => $parameter) {
        $key = $parameter->getKey();
        $value = $parameter->getValue();
        if ($key != 'fq' || empty($value)) {
          unset($parameters[$i]);
        }
      }

      // If more filterquery's are defined, take first one.
      $parameter = substr(substr($parameters[0]->getValue(), 10), 0, -1);

      foreach ($roadmaps as $roadmap) {
        $subbrand = $roadmap->getSubbrand();
        $collaboration_keyword = json_decode($roadmap->getDescription());
        $collaboration_keyword = strtolower($collaboration_keyword->keyword);

        if($consumer_id == $subbrand && $collaboration_keyword == $parameter) {
          $title = $roadmap->getTitle();
          $original_title = $extended_entity->getTitle(culturefeed_search_get_preferred_language());
        }
      }
      if(!isset($title)) {
        $title = $extended_entity->getTitle(culturefeed_search_get_preferred_language());
      }
    }
    else {
      $title = $extended_entity->getTitle(culturefeed_search_get_preferred_language());
    }

    if ($this->options['link']) {
      if (isset($original_title)) {
        return culturefeed_search_detail_l($extended_entity->getType(), $extended_entity->getId(), $original_title, $title);
      }
      else {
        return culturefeed_search_detail_l($extended_entity->getType(), $extended_entity->getId(), $title, $title);
      }
    }
    else {
      return $title;
    }

  }

}
