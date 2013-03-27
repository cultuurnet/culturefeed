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
  foreach ($variables['searchresult']->getItems() as $item) {
    print theme('culturefeed_searchresult_item', array('item' => $item));
  }
}