<?php
/**
 * @file
 * Template file for the culturefeed social user activity summary item (with a teaser).
 */

/**
 * @var string $picture
 * @var string $nick
 * @var string $prefix
 * @var string $link
 * @var string $suffix
 * @var string $date
 *
 * Some have a teaser:
 * @var Boolean $has_teaser
 * @var string $teaser_title
 * @var string $teaser_summary
 * @var string $teaser_body
 * @var string $teaser_image
 */
?>

<?php print $picture ?>

<?php print $nick ?>

<?php if($prefix): ?>
  <?php print $prefix . ' '; ?>:
<?php endif; ?>

<?php print $link; ?>

<?php if($suffix): ?>
  <?php print ' ' . $suffix; ?>
<?php endif; ?>
<br />

<?php print $date ?>

<?php if (!empty($teaser)): ?>
<?php print $teaser; ?>
<?php endif; ?>
