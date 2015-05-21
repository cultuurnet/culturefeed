<?php
/**
 * @file
 * Template file for one culturefeed search filter link as facet.
 */
?>

<?php if ($active): ?>
  <span class="facet-label active"><?php print check_plain($label); ?><a href="<?php print $url; ?>" class="facet-remove" title="<?php print t('Remove filter'); ?>">&times;</a></span>
<?php else: ?>
  <span class="facet-label"><a href="<?php print $url; ?>"><?php print check_plain($label); ?></a></span>
<?php endif; ?>
