<?php
/**
 * @file
 */

/**
 * An area handler to provide an 'Add event' link.
 */
class views_handler_area_cdb_item_create extends views_handler_area {

  /**
   * {@inheritdoc}
   */
  function render($empty = FALSE) {

    $links = array();

    if (user_access('create culturefeed events')) {
      $links['create'] = l(t('Add event'), 'agenda/e/add');
    }

    if (!empty($links)) {
      return theme('item_list', array('items' => $links));
    }
    else {
      return '';
    }

  }

}
