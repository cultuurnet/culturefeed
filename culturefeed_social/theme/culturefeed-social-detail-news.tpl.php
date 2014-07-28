<?php

/**
 * @file
 * Template for detailpage of a news item activity.
 */
?>

<?php print $picture ?>
<?php print $nick ?>

<?php print $date ?><br/>

<?php print $body; ?>

<?php if (!empty($image)) :?>
  <?php print $image; ?>
<?php endif; ?>

