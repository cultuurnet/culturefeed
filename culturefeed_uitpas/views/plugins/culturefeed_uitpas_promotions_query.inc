<?php
/**
 * @file
 * Definition of culturefeed_uitpas_promotions_query.
 */

/**
 * Views query plugin for UiTPAS points promotions.
 */
class culturefeed_uitpas_promotions_query extends views_plugin_query {

  /**
   * {@inheritdoc}
   */
  function execute(&$view) {

    $cf = DrupalCultureFeed::getConsumerInstance();
    $promotion_points_query = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
    $promotions_result = $cf->uitpas()->getPromotionPoints($promotion_points_query);

    $view->result = $promotions_result->objects;
  }
}
