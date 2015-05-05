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
      $media = $extended_entity->getEntity()->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language())->getMedia();
      $roadmaps = $media->byMediaType('roadmap');

      $parameters = $this->query->getParameters();
      $consumer_id = variable_get('culturefeed_api_application_key');

      foreach ($parameters as $i => $parameter) {
        $key = $parameter->getKey();
        $value = $parameter->getValue();
        if ($key != 'fq' || empty($value) || 0 !== strpos($value, 'keyword')) {
          unset($parameters[$i]);
        }
      }

      // If more filterquery's are defined, take first one.
      if (count($parameters) > 0) {
        $parameter = substr(substr($parameters[0]->getValue(), 10), 0, -1);
      }

      foreach ($roadmaps as $roadmap) {
        $subbrand = $roadmap->getSubbrand();
        $collaboration_keyword = json_decode($roadmap->getDescription());
        $collaboration_keyword = strtolower($collaboration_keyword->keyword);

        if($consumer_id == $subbrand && $collaboration_keyword == $parameter) {
          $short_description = $roadmap->getPlainText();
        }
      }
      if(!isset($short_description)) {
        $short_description = $extended_entity->getEntity()->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language())->getShortDescription();
      }
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
