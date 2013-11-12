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
   * @var CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions
   */
  public $object;

  /**
   * Constructor.
   */
  public function __construct() {
    $this->object = new CultureFeed_Uitpas_Passholder_Query_SearchPromotionPointsOptions();
  }

  /**
   * {@inheritdoc}
   */
  function execute(&$view) {

    $cf = DrupalCultureFeed::getConsumerInstance();
    $promotions_result = $cf->uitpas()->getPromotionPoints($this->object);

    $view->result = array();
    foreach ($promotions_result->objects as $promotion) {
      $promotion_properties = get_object_vars($promotion);
      foreach (array_keys($promotion_properties) as $promotion_property_name) {
        $lowercase_promotion_property_name = strtolower($promotion_property_name);
        if ($promotion_property_name !== $lowercase_promotion_property_name) {
          $promotion_properties[$lowercase_promotion_property_name] = $promotion_properties[$promotion_property_name];
          unset($promotion_properties[$promotion_property_name]);
        }
      }
      // Views does not handle mixed case field names well, so lowercase everything.
      $view->result[] = (object) $promotion_properties;
    }
  }

  function add_field($table, $field, $alias = '', $params = array()) {
    if ($alias) {
      return $alias;
    }
    else {
      return $field;
    }
  }
}
