<?php
/**
 * @file
 * Defines page callbacks which access the Search api.
 */

/**
 * Page callback to show the detail from an agenda item.
 * @param CultureFeed_Cdb_Item_Base $item
 *   Cdb item to view.
 */
function culturefeed_agenda_detail_page($item) {

  return array(
    '#theme' => 'culturefeed_' . $item->getType(),
    '#item' => $item,
  );

}
