<?php
/**
 * @file 
 * Template file for a search list.
 * @var
 *   $noitems Boolean to indicate items were found.
 *   $items Array of items ready to display.
 */

if ($noitems) :
  ?>
  <b>Geen resultaten gevonden</b>
  <?php 
else :
  foreach ($items as $item) {
    print $item;
  }
endif;