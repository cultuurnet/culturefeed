<?php
/**
 * @file 
 * Template for the main section of a search page. 
 * 
 * @var $search_result
 *   CultuurNet\Search\SearchResult
 */

if (empty($variables['searchresult'])) {
  print '<b>Geen resultaten gevonden</b>';
}
else {
  
  // Print the sortorder widget if you didn't use the block.
  if (!empty($variables['sortorder_widget'])) {
    print $variables['sortorder_widget'];
  }
  
  foreach ($variables['searchresult']->getItems() as $item) {
    $entity = $item->getEntity();
    if ($entity instanceof CultureFeed_Cdb_Item_Event) {
      print theme('culturefeed_event_summary', array('item' => $item));
    }
  }
}