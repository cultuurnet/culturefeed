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

if ($noitems) :
  ?>
  <b><?php print t('No results found'); ?></b>
  <?php 
else :
  foreach ($items as $item) {
    print $item;
  }
endif;