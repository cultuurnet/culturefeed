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
 * @var string $teaser (shows the details of the event)
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

<?php if(isset($teaser)): ?>
  <?php print $teaser; ?>
<?php endif; ?>
