<?php

/**
 * A handler to provide the age range of a cdb item.
 */
class views_handler_field_cdb_item_age extends views_handler_field {

  /**
   * @see views_handler_field::render()
   */
  function render($extended_entity) {

    $age = '';

    /* @var \CultuurNet\Search\ActivityStatsExtendedEntity $extended_entity */
    if ($extended_entity->getType() == 'event') {

      /* @var CultureFeed_Cdb_Item_Event $entity */
      $entity = $extended_entity->getEntity();
      $age = $entity->getAgeFrom();

    }

    return $age;

  }

}
