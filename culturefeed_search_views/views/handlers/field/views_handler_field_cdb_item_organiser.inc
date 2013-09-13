<?php

/**
 * A handler to provide proper displays for the city of a cdb item.
 */
class views_handler_field_cdb_item_organiser extends views_handler_field {

  /**
   * @see views_handler_field::render()
   */
  function render($extended_entity) {

    if (!method_exists($extended_entity->getEntity(), 'getOrganiser')) {
      return;
    }

    $organiser = $extended_entity->getEntity()->getOrganiser();
    if (!$organiser) {
      return;
    }

    if ($organiser->getActor() && $organiser->getActor()->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language())) {
      $actor = $organiser->getActor();
      $organiser_actor_details = $organiser->getActor()->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language());
      return $organiser_actor_details->getTitle();
    }

    return $organiser->getLabel();

  }

}
