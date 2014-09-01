<?php

/**
 * @file
 * Template for detailpage of a news item activity.
 */
?>

<?php if (!empty($page_admin)): ?>
  <a href="<?php print url('pages/' . $page_id . '/news/delete/' . $activity_id); ?>"><?php print t('Delete news item') ?></a>
<?php endif; ?>

<?php print $picture ?>
<?php print $nick ?>

<?php print $date ?><br/>

<?php print $body; ?>

<?php if (!empty($image)) :?>
  <?php print $image; ?>
<?php endif; ?>

