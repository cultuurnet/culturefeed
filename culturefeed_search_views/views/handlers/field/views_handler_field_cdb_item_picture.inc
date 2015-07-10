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
    $options['scale'] = array('default' => 'none');
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

    $form['scale'] = array(
      '#type' => 'radios',
      '#title' => t('Scale'),
      '#options' => array(
        'none' => t('None'),
        'downscaleonly' => t('Downscale only'),
        'upscaleonly' => t('Upscale only'),
        'both' => t('Both'),
        'upscalecanvas' => t('Upscale canvas'),
      ),
      '#default_value' => isset($this->options['scale']) ? $this->options['scale'] : 'none',
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

    $image = '';
    $main_picture = '';
    $no_main_picture = '';
    $override_picture = FALSE;

    if ($this->options['override']) {

      module_load_include('inc', 'culturefeed_search_views', 'views/helpers/culturefeed_search_views_helper_collaboration');
      $data = culturefeed_search_views_helper_collaboration_get_data($extended_entity, $this->view);
      if ($data && isset($data['description']->image) && $data['description']->image) {
        $main_picture = $data['description']->image;
        $override_picture = TRUE;
      }

    }

    if (!$main_picture) {

      $pictures = $detail->getMedia()->byMediaType(CultureFeed_Cdb_Data_File::MEDIA_TYPE_PHOTO);
      foreach ($pictures as $picture) {

        if (!$no_main_picture) {
          $no_main_picture = $picture->getHLink();
        }
        if ($picture->isMain()) {
          $main_picture = $picture->getHLink();
        }

      }

      if (!$main_picture && $no_main_picture) {
        $main_picture = $no_main_picture;
      }

    }

    // Render the main picture according the settings.
    if ($main_picture) {

      if ($this->options['format'] == 'image') {

        // In case of an override.
        if ($override_picture) {

          if ($this->options['width'] >= $this->options['height'])  {
            $image = theme('image', array(
              'path' => $main_picture,
              'width' => $this->options['width'],
            ));
          }
          else {
            $image = theme('image', array(
              'path' => $main_picture,
              'height' => $this->options['height'],
            ));
          }

        }

        else {
          if ($this->options['scale'] != 'none') {
            $image = theme('image', array('path' => $main_picture . '?crop=auto&width=' . $this->options['width'] . '&height=' . $this->options['height'] . '&scale=' . $this->options['scale']));
          }
          else {
            $image = theme('image', array('path' => $main_picture . '?crop=auto&width=' . $this->options['width'] . '&height=' . $this->options['height']));
          }
        }

      }
      else {

        if ($this->options['format'] == 'url') {

          if ($override_picture) {
            $image = $main_picture;
          }
          else {
            if ($this->options['scale'] != 'none') {
              $image = $main_picture . '?crop=auto&width=' . $this->options['width'] . '&height=' . $this->options['height'] . '&scale=' . $this->options['scale'];
            }
            else {
              $image = $main_picture . '?crop=auto&width=' . $this->options['width'] . '&height=' . $this->options['height'];
            }
          }

        }

      }

      // Return the image if set.
      if ($image) {

        if ($this->options['link']) {
          $path = culturefeed_search_detail_path($extended_entity->getType(), $extended_entity->getId(), $extended_entity->getTitle(culturefeed_search_get_preferred_language()));
          return l($image, $path, array('html' => TRUE,));
        }
        else {
          return $image;
        }

      }

    }

  }

}
