<?php

/**
 * A handler to provide proper displays for the calendar summary of a cdb item.
 */
class views_handler_field_cdb_item_calendar_summary extends views_handler_field {

  /**
   * @see views_handler_field::render()
   */
  function render($extended_entity) {
    $details = $extended_entity->getEntity()->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language());
    if ($details) {
      return $details->getCalendarSummary();
    }
  }

}
