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
    <span class="facet-label active"><?php print check_plain($label); ?> <span class="facet-count">(<?php print $count; ?>)</span> <a href="<?php print $url; ?>" class="facet-remove" title="<?php print t('Remove filter'); ?>">&times;</a></span>
<?php else: ?>
    <span class="facet-label"><a href="<?php print $url; ?>"><?php print check_plain($label); ?></a></span> <span class="facet-count">(<?php print $count; ?>)</span>
<?php endif; ?>
