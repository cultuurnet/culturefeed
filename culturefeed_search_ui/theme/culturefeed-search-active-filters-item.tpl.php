<?php
/**
 * @file
 * Template file for one item in the 'current filter' block.
 */
?>

<span class="filter">
    <span class="filter-label"><?php print check_plain($label); ?></span>
    <a href="<?php print $url; ?>" title="Verwijder filter '<?php print check_plain($label); ?>'">
      <i class="icon-remove icon-large text-red"></i><span class="hidden">verwijder
    </a>
</span>