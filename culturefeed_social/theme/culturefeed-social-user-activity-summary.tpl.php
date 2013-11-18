<?php
/**
 * @file
 * Template file for the culturefeed social user activity summary item.
 */

/**
 * @var string $picture
 * @var string $nick
 * @var string $prefix
 * @var string $link
 * @var string $suffix
 * @var string $date
 */
?>

<?php print $picture ?>

<?php print $nick ?>

<?php if($prefix): ?>
  <?php print $prefix . ' '; ?>
<?php endif; ?>
<?php print $link; ?>
<?php if($suffix): ?>
  <?php print ' ' . $suffix; ?>
<?php endif; ?>.<br />

<?php print $date ?>
