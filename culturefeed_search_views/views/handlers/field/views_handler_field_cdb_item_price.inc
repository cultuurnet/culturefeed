<?php

/**
 * A handler to provide the price of a cdb item.
 */
class views_handler_field_cdb_item_price extends views_handler_field {

  /**
   * @see views_handler_field::render()
   */
  function render($extended_entity) {

    $price = '';

    /* @var \CultuurNet\Search\ActivityStatsExtendedEntity $extended_entity */
    if ($extended_entity->getType() == 'event') {

      /* @var CultureFeed_Cdb_Item_Event $entity */
      $entity = $extended_entity->getEntity();
      try {
        $price_info = $entity->getDetails()->current()->getPrice();
        if ($price_info) {
          $price = $price_info->getValue();
        }
      }
      catch (Exception $e) {
        watchdog_exception('culturefeed_search_views', $e);
      }

    }

    return $price;

  }

}
