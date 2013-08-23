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
  <span class="facet-label active"><?php print check_plain($label); ?> <small class="muted">(<?php print $count; ?>)</small> <?php print l('<i class="icon-remove text-red pull-right icon-large"></i>', $url, array('attributes' => array('class' => 'facet-remove pull-right', 'title' => 'Verwijder filter'), 'html' => TRUE)); ?></span>
<?php else: ?>
  <?php print '' . l($label, $url, array('html' => TRUE)); ?> <small class="muted facet-count">(<?php print $count; ?>)</small>
<?php endif; ?>
