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
  <span class="facet-label active"><?php print check_plain($label); ?> <span class="facet-count">(<?php print $count; ?>)</span> <?php print l('&times;', $url, array('attributes' => array('class' => 'facet-remove'), 'html' => TRUE)); ?></span>
<?php else: ?>
  <span class="facet-label"><?php print '' . l($label, $url, array('html' => TRUE)); ?></span> <span class="facet-count">(<?php print $count; ?>)</span>
<?php endif; ?>
