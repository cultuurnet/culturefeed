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

<?php if($suffix): ?>
  <?php print ' ' . $suffix; ?>
<?php endif; ?>
<br />

<?php print $date ?>

<?php if($has_teaser): ?>

  <?php if($teaser_image): ?>
    <img src="<?php print $teaser_image ?>" alt="<?php print $teaser_title ?>" />
  <?php endif; ?>

  <?php print $teaser_title ?>

  <?php if($teaser_body): ?>
    <?php if($teaser_summary): ?>
    <?php print $teaser_summary ?>
    <div style="display: none"><?php print $teaser_body ?></div>
    <?php else: ?>
    <?php print $teaser_body ?>
    <?php endif; ?>

  <?php endif; ?>

<?php endif; ?>
