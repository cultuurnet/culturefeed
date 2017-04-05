<?php
/**
 * @file
 * Template file for one item in the 'current filter' block.
 */
?>

<span class="filter">
  <span class="filter-label"><?php print check_plain($label); ?></span> 
  <a href="<?php print $url; ?>" class="filter-remove"><span class="element-invisible"><?php print t('Remove filter'); ?></span><strong>&times;</strong></a>
</span>
