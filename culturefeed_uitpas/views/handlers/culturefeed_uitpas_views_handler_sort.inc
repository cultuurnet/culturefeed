<?php

// @codingStandardsIgnoreFile

/**
 * @file
 */
class culturefeed_uitpas_views_handler_sort extends views_handler_sort {

  /**
   * {@inheritdoc}
   */
  function query() {

    switch ($this->options['order']) {
      case 'ASC':
      case 'asc':
      default:
      $order = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::ORDER_ASC;
        break;
      case 'DESC';
      case 'desc';
      $order = CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions::ORDER_DESC;
        break;
    }

    $this->query->object->sort = $this->definition['sort_property'];
    $this->query->object->order = $order;

  }

  /**
   * {@inheritdoc}
   */
  function can_expose() {
    return FALSE;
  }

}