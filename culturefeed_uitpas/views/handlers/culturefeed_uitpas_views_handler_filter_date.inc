<?php

// @codingStandardsIgnoreFile

/**
 * @file
 */
class culturefeed_uitpas_views_handler_filter_date extends views_handler_filter {

  /**
   * {@inheritdoc}
   */
  function query() {
    $this->query->object->{$this->real_field} = time();
  }

  /**
   * {@inheritdoc}
   */
  function can_expose() {
    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  function admin_summary() {
    return '= ' . t('now');
  }

}
