<?php
/**
 * @file 
 * Template page for a promotions list.
 */

?>

<p><?php print t('Collect extra points? That possible ...'); ?></p>

<div>
<?php foreach ($items as $promotion): ?>
  <?php print $promotion; ?>
<?php endforeach; ?>
</div>