<?php

/**
 * A handler to provide proper displays for the picture of a cdb item.
 */
class views_handler_field_cdb_item_picture extends views_handler_field {

  /**
   * @see views_handler_field::option_definition()
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['override'] = array('default' => FALSE);
    $options['width'] = array('default' => 100);
    $options['height'] = array('default' => 100);
    $options['format'] = array('default' => 'image');
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

    $form['width'] = array(
      '#type' => 'textfield',
      '#title' => t('Width'),
      '#default_value' => isset($this->options['width']) ? $this->options['width'] : '',
      '#required' => TRUE,
    );

    $form['height'] = array(
      '#type' => 'textfield',
      '#title' => t('Height'),
      '#default_value' => isset($this->options['height']) ? $this->options['height'] : '',
      '#required' => TRUE,
    );

		$form['format'] = array(
			'#type' => 'select',
			'#title' => t('Format'),
			'#options' => array(
				'image' => t('image'),
				'url' => t('url'),
			),
			'#default_value' => isset($this->options['format']) ? $this->options['format'] : 'image',
		);

    $form['link'] = array(
      '#type' => 'checkbox',
      '#title' => t('Link the picture to the detail page'),
      '#default_value' => isset($this->options['link']) ? $this->options['link'] : FALSE,
    );

    parent::options_form($form, $form_state);

  }

  /**
   * @see views_handler_field::render()
   */
  function render($extended_entity) {

    $detail = $extended_entity->getEntity()->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language());
    if (!$detail) {
      return;
    }

    $main_picture = '';

    if ($this->options['override']) {
      $media = $detail->getMedia();
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
        $collaboration_data = json_decode($roadmap->getDescription());
        $collaboration_keyword = strtolower($collaboration_data->keyword);

        if($consumer_id == $subbrand && $collaboration_keyword == $parameter) {
          $picture = $collaboration_data->image;
          if (isset($picture)) {
            $main_picture = $picture;
          }
        }
      }
      if(empty($main_picture)) {
        $pictures = $detail->getMedia()->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_PHOTO);
        foreach ($pictures as $picture) {
          if ($picture->isMain()) {
            $main_picture = $picture->getHLink();
          }
        }
      }
    }
    else {
      $pictures = $detail->getMedia()->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_PHOTO);
      foreach ($pictures as $picture) {
        if ($picture->isMain()) {
          $main_picture = $picture->getHLink();
        }
      }
    }

    if (empty($main_picture) && $pictures && $pictures->count() > 0) {
      $pictures->rewind();
      $picture = $pictures->current();
      $main_picture = $picture->getHLink();
    }

    if (!empty($main_picture)) {
			if ($this->options['format'] == 'image') {
				$image = theme('image', array('path' => $main_picture . '?crop=auto&width=' . $this->options['width'] . '&height=' . $this->options['height']));
			}
			else if ($this->options['format'] == 'url') {
				$image = $main_picture . '?crop=auto&width=' . $this->options['width'] . '&height=' . $this->options['height'];
			}

      if ($this->options['link']) {
        $path = culturefeed_search_detail_path($extended_entity->getType(), $extended_entity->getId(), $extended_entity->getTitle(culturefeed_search_get_preferred_language()));
        return l($image, $path, array('html' => TRUE, ));
      }
      else {
        return $image;
      }
    }

  }

}
