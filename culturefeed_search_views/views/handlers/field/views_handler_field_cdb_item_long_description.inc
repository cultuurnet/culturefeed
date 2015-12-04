<?php

/**
 * A handler to provide the long description of a cdb item.
 */
class views_handler_field_cdb_item_long_description extends views_handler_field {

  /**
   * @see views_handler_field::render()
   */
  function render($extended_entity) {

    $long_description = '';

    /* @var \CultuurNet\Search\ActivityStatsExtendedEntity $extended_entity */
    if ($extended_entity->getType() == 'event') {

      /* @var CultureFeed_Cdb_Item_Event $entity */
      $entity = $extended_entity->getEntity();
      try {
        $long_description = $entity->getDetails()->getDetailByLanguage('nl')->getLongDescription();
      }
      catch (Exception $e) {
        watchdog_exception('culturefeed_search_views', $e);
      }

    }

    return $long_description;

  }

}
