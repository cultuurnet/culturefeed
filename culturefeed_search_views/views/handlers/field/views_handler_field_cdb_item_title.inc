<?php

/**
 * A handler to provide proper displays for titles of a cdb item.
 */
class views_handler_field_cdb_item_title extends views_handler_field {

  /**
   * @see views_handler_field::render()
   */
  function render($extended_entity) {
    return $extended_entity->getTitle(culturefeed_search_get_preferred_language());
  }

}
