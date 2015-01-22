<?php
/**
 * @file
 * Template for the summary of a page.
 */
?>

<?php if (!empty($thumbnail)): ?>
  <img src="<?php print $thumbnail; ?>?width=80&height=80&crop=auto" />
<?php endif; ?>

<h4 class="media-heading"><a href="<?php print $url ?>"><?php print $title; ?></a></h4>
<?php if (isset($location['city'])): ?>
  <?php print $location['city']; ?>
<?php endif;?>

<?php if (isset($when)): ?>
  <?php print $when; ?>
<?php endif;?>
