<?php

// @codingStandardsIgnoreFile

/**
 * @file
 */ 
class culturefeed_uitpas_views_handler_filter_card_system_id extends views_handler_filter {

  function query() {
    $card_system_id = variable_get('culturefeed_uitpas_cardsystem_id');
    if ($card_system_id) {
      $this->query->object->{$this->definition['filter_property']} = $card_system_id;
    }
  }

  /**
   * {@inheritdoc}
   */
  function admin_summary()
  {
    return '= ' . t('Card system ID of the Drupal installation.');
  }

  /**
   * {@inheritdoc}
   */
  function can_expose() {
    return FALSE;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['static_filter_value'] = array(
      '#weight' => -100,
      '#type' => 'markup',
      '#markup' => '= ' . t('Card system ID of the Drupal installation.'),
    );
  }
}
