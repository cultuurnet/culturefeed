<?php
/**
 * @file
 * Template file for one culturefeed search facet item.
 */

/**
 * @var string $name
 * @var integer $count
 * @var string $url
 * @var boolean $active
 */
?>
<?php if ($active): ?>
  <?php print check_plain($name); ?> (<?php print $count; ?>) [<?php print l('x', $url); ?>]
<?php else: ?>
  <?php print l($name, $url); ?> (<?php print $count; ?>)
<?php endif; ?>
