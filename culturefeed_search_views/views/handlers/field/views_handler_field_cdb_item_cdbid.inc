<?php

/**
 * A handler to provide proper displays for titles of a cdb item.
 */
class views_handler_field_cdb_item_cdbid extends views_handler_field {

  /**
   * @see views_handler_field::render()
   */
  function render($extended_entity) {

    /* @var \CultuurNet\Search\ActivityStatsExtendedEntity $extended_entity */
    $cdbid = $extended_entity->getId();

    return $cdbid;

  }

}
