<?php

/**
 * A handler to provide proper displays for the short description of a cdb item.
 */
class views_handler_field_cdb_item_short_description extends views_handler_field {

  /**
   * @see views_handler_field::render()
   */
  function render($extended_entity) {
    $details = $extended_entity->getEntity()->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language());
    if ($details) {
      return $details->getShortDescription();
    }
  }

}
