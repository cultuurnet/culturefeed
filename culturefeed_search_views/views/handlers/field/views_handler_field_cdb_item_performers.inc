<?php

/**
 * A handler to provide the performers of a cdb item.
 */
class views_handler_field_cdb_item_performers extends views_handler_field {

  /**
   * @see views_handler_field::render()
   */
  function render($extended_entity) {

    $performers = array();

    /* @var \CultuurNet\Search\ActivityStatsExtendedEntity $extended_entity */
    if ($extended_entity->getType() == 'event') {

      /* @var CultureFeed_Cdb_Item_Event $entity */
      $entity = $extended_entity->getEntity();
      try {
        $performers = array();
        $performer_details = $entity->getDetails()->current()->getPerformers();
        if ($performer_details) {

          foreach ($performer_details as $performer) {
            /* @var CultureFeed_Cdb_Data_Performer $performer */
            $performers[] = $performer->getLabel() . ' (' . $performer->getRole() . ')';
          }

        }

      }
      catch (Exception $e) {
        watchdog_exception('culturefeed_search_views', $e);
      }

    }

    return implode(', ', $performers);

  }

}
