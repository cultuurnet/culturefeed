<?php
/**
 * @file
 * Template file for a single culturefeed search facet item.
 */

/**
 * @var string $label
 * @var integer $count
 * @var string $url
 * @var boolean $active
 */
?>
<?php if ($active): ?>
  <span class="facet-label active"><?php print check_plain($label); ?> <small class="muted">(<?php print $count; ?>)</small> <a href="<?php print $url; ?>" class="facet-remove pull-right" title="<?php print t('Remove filter'); ?>"><i class="icon-remove text-red pull-right icon-large"></i></a></span>
<?php else: ?>
  <a href="<?php print $url; ?>"><?php print check_plain($label); ?></a> <small class="muted facet-count">(<?php print $count; ?>)</small>
<?php endif; ?>
