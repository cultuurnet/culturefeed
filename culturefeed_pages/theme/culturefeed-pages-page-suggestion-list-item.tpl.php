<?php
/**
 * @file
 * Template for the summary of a page.
 */
?>

<hr />

<h3><a href="<?php print $url ?>"><?php print $title; ?> <?php if (!empty($address)): ?>- <?php print $address['city']; ?><?php endif; ?></a></h3>

<?php if ($follower_count > 0): ?>
  <?php print format_plural($follower_count, '@count' . ' ' . t('follower'), '@count' . ' ' . t('followers')); ?>
<?php endif; ?>

<?php if ($member_count > 0): ?>
  - <?php print format_plural($member_count, '@count' . ' ' . t('member'), '@count' . ' ' . t('members')); ?>
<?php endif; ?>


<?php if ($logged_in): ?>

  <?php if (!$following): ?>
    <a href="<?php print $follow_url; ?>"><small><?php print $follow_text; ?></small></a>
  <?php else: ?>
    <small><?php print t('You follow this page'); ?></small>
  <?php endif; ?>
  </p>
<?php else: ?>
  <p><small><?php print $follow_text; ?></small></p>
<?php endif; ?>

<p><?php print $description ?></p>

<p><a href="<?php print $url; ?>"><?php print $more_text; ?></a></p>





















