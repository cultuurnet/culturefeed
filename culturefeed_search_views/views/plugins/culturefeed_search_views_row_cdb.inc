<?php

/**
 * Row plugin for Culturefeed CDB items.
 */
class culturefeed_search_views_row_cdb extends views_plugin_row {

  function option_definition() {
    $options = parent::option_definition();

    $options['view_mode'] = array('default' => 'summary');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $options = $this->options_form_summary_options();
    $form['view_mode'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#title' => t('View mode'),
      '#default_value' => $this->options['view_mode'],
    );
  }

  /**
   * Return the main options, which are shown in the summary title.
   */
  function options_form_summary_options() {

    $options = array(
      'summary' => t('Summary'),
      'short_summary' => t('Short summary')
    );

    return $options;

  }

  function summary_title() {
    $options = $this->options_form_summary_options();
    return check_plain($options[$this->options['view_mode']]);
  }

  /**
   * Render each $row.
   */
  function render($row) {
    return  theme('culturefeed_' . $row->getType() . '_' . $this->options['view_mode'], array('item' => $row));
  }

}
