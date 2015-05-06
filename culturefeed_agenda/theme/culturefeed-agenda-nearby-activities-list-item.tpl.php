<?php
/**
 * @file
 * Template for the summary of a page.
 */
?>

<?php if (!empty($thumbnail)): ?>
  <img src="<?php print $thumbnail; ?>">
<?php endif; ?>

<h4><a href="<?php print $url ?>"><?php print $title; ?></a></h4>
<?php if (isset($location['city'])): ?>
  <?php print $location['city']; ?>
<?php endif;?>

<?php if (isset($when_md)): ?>
  <?php print $when_md; ?>
<?php endif;?>
