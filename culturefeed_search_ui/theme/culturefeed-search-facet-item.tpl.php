<?php
/**
 * @file
 * Template file for one culturefeed search facet item.
 */

/**
 * @var string $label
 * @var integer $count
 * @var string $url
 * @var boolean $active
 */
?>
<?php if ($active): ?>
  <?php print check_plain($label); ?> (<?php print $count; ?>) [<?php print l('x', $url); ?>]
<?php else: ?>
  <?php print l($label, $url); ?> (<?php print $count; ?>)
<?php endif; ?>
