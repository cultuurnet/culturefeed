<?php
/**
 * @file 
 * Template file for a search list.
 * @var
 *   $noitems 
 *     Boolean to indicate no items were found.
 *   $items Array 
 *     items ready to display.
 *   $nowrapper (default=false). 
 *     For ajax request, you should hide the wrapper.
 */

$link = l(t('perform a new search.'), str_replace("/ajax", "", $_GET['q']));

if ($noitems) :
  ?>
  <?php print 
  '<div class="alert"><p><strong>' . t('There are no more search results.') . '</strong></p> <p>' . t('Refine your search results or') . ' ' . $link . '</p></div>'  
  ?>
  <?php 
else :
  foreach ($items as $item) {
    print $item;
  }
endif;




