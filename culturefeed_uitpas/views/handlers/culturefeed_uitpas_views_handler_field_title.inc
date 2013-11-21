<?php
// @codingStandardsIgnoreFile

/**
 * @file
 */

class culturefeed_uitpas_views_handler_field_title extends views_handler_field {

  /**
   * {@inheritdoc}
   */
  function option_definition() {

    $options = parent::option_definition();
    $options['link'] = array('default' => TRUE);
    return $options;

  }

  /**
   * {@inheritdoc}
   */
  function options_form(&$form, &$form_state) {

    $form['link'] = array(
      '#title' => t('Link this field to the original piece of content'),
      '#description' => t("Enable to override this field's links."),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link']),
    );
    parent::options_form($form, $form_state);

  }

  /**
   * {@inheritdoc}
   */
  function render($values) {

    $value = $this->get_value($values);
    if ($this->options['link']) {
      return l($value, $this->definition['base_path'] . '/' . culturefeed_search_slug($value) . '/' . $values->id);
    }
    else {
      return $value;
    }

  }

}
