<?php
// @codingStandardsIgnoreFile

/**
 * @file
 */

class culturefeed_uitpas_views_handler_field_image extends views_handler_field {

  /**
   * {@inheritdoc}
   */
  function option_definition() {

    $options = parent::option_definition();
    $options['multiple'] = array('default' => FALSE);
    return $options;

  }

  /**
   * {@inheritdoc}
   */
  function options_form(&$form, &$form_state) {

    $form['multiple'] = array(
      '#title' => t('Show all available images'),
      '#description' => t('Enable to show all images.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['multiple']),
    );
    parent::options_form($form, $form_state);

  }

  /**
   * {@inheritdoc}
   */
  function render($values) {

    $value = $this->get_value($values);
    if ($value) {

      if ($this->options['multiple']) {

        $images = array();
        foreach ($value as $image) {
          $images[] = theme_image(array('path' => $image, 'attributes' => array()));
        }
        return implode('', $images);

      }
      else {
        return theme_image(array('path' => $value[0], 'attributes' => array()));
      }

    }

  }

}
