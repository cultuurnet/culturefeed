<?php
// @codingStandardsIgnoreFile

/**
 * @file
 */

class culturefeed_uitpas_views_handler_field_markup extends views_handler_field {

  /**
   * {@inheritdoc}
   */
  function option_definition() {

    $options = parent::option_definition();
    $options['format'] = array('default' => filter_default_format());
    return $options;

  }

  /**
   * {@inheritdoc}
   */
  function options_form(&$form, &$form_state) {

    $formats = filter_formats();
    $options = array();
    foreach ($formats as $key => $format) {
      $options[$key] = $format->name;
    }

    $form['format'] = array(
      '#title' => t('Link this field to the original piece of content'),
      '#description' => t("Enable to override this field's links."),
      '#type' => 'select',
      '#default_value' => !empty($this->options['format']),
      '#options' => $options,
    );
    parent::options_form($form, $form_state);

  }

  /**
   * {@inheritdoc}
   */
  function render($values) {

    $value = $this->get_value($values);

    $format = $this->options['format'];

    // If the requested text format does not exist, the text cannot be filtered.
    if (!filter_format_load($format)) {
      watchdog('filter', 'Missing text format: %format.', array('%format' => $format), WATCHDOG_ALERT);
      return '';
    }

    return check_markup($value, $format, '');

  }


}
