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
    $options['maxheight'] = array('default' => '');
    $options['maxwidth'] = array('default' => '');
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
    $form['maxheight'] = array(
      '#title' => t('Maximum height for images'),
      '#type' => 'textfield',
      '#default_value' => !empty($this->options['maxheight']) ? $this->options['maxheight'] : '',
    );
    $form['maxwidth'] = array(
      '#title' => t('Maximum width for images'),
      '#type' => 'textfield',
      '#default_value' => !empty($this->options['maxwidth']) ? $this->options['maxwidth'] : '',
    );
    parent::options_form($form, $form_state);

  }

  /**
   * {@inheritdoc}
   */
  function render($values) {

    $value = $this->get_value($values);

    $vars = array('attributes' => array());
    if ($this->options['maxheight']) {
      $vars['maxheight'] = $this->options['maxheight'];
    }
    if ($this->options['maxwidth']) {
      $vars['maxwidth'] = $this->options['maxwidth'];
    }

    if ($value) {

      if ($this->options['multiple']) {

        $images = array();
        foreach ($value as $image) {
          $images[] = theme('culturefeed_uitpas_promotion_image', $vars + array('path' => $image));
        }
        return implode('', $images);

      }
      else {
        return theme('culturefeed_uitpas_promotion_image', $vars + array('path' => $value[0]));
      }

    }

  }

}
