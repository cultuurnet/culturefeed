<?php

/**
 * A handler to provide the keywords of a cdb item.
 */
class views_handler_field_cdb_item_keywords extends views_handler_field {

  /**
   * @see views_handler_field::render()
   */
  function render($extended_entity) {

    $keywords = array();

    /* @var \CultuurNet\Search\ActivityStatsExtendedEntity $extended_entity */
    if ($extended_entity->getType() == 'event') {

      /* @var CultureFeed_Cdb_Item_Event $entity */
      $entity = $extended_entity->getEntity();
      try {
        $keywords = array();
        foreach ($entity->getKeywords(TRUE) as $keyword) {
          /* @var CultureFeed_Cdb_Data_Keyword $keyword */
          $keywords[] = $keyword->getValue();
        }

      }
      catch (Exception $e) {
        watchdog_exception('culturefeed_search_views', $e);
      }

    }

    return implode(', ', $keywords);

  }

}
