<?php
/**
 * @file
 */

use \CultuurNet\Search\ActivityStatsExtendedEntity;

/**
 * A handler to provide links to operations that can be performed on the item.
 */
class views_handler_field_cdb_item_operations extends views_handler_field {

  /**
   * {@inheritdoc}
   */
  function render(ActivityStatsExtendedEntity $extended_entity) {

    $type = $extended_entity->getType();

    $links = array();

    $links['view'] = culturefeed_search_detail_l($type, $extended_entity->getId(), $extended_entity->getTitle(culturefeed_search_get_preferred_language()), t('View'));

    $tags_path = culturefeed_entry_ui_tags_path($extended_entity);
    if ($tags_path && user_access('manage object tags')) {
      $links['tags'] = l(t('Tags'), $tags_path);
    }

    $edit_path = culturefeed_entry_ui_edit_path($extended_entity);
    if ($edit_path && user_access('edit culturefeed events')) {
      $links['edit'] = l(t('Edit'), $edit_path);
    }

    return theme('item_list', array('items' => $links));
  }

}
