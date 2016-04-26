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
 * @var boolean $active_subitem
 */
?>

<?php if ($active): ?>
<span class="facet-label active"><?php print check_plain($label); ?><span class="facet-count">(<?php print $count; ?>)</span><a href="<?php print $url; ?>" class="facet-remove" title="<?php print t('Remove filter'); ?>"><span class="element-invisible"><?php print t('Remove filter'); ?></span>&times;</a></span>
<?php else: ?>
<span class="facet-label"><a href="<?php print $url; ?>" <?php $active_subitem ? print 'rel="nofollow"' : ''; ?>><?php print check_plain($label); ?></a></span><span class="facet-count">(<?php print $count; ?>)</span>
<?php endif; ?>
