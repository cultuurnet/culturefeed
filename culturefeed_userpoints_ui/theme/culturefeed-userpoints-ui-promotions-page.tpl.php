<?php
/**
 * @file 
 * Template page for a promotions list.
 */

?>

<p>Extra punten verzamelen? dat kan ... </p>

<div>
<?php foreach ($items as $promotion): ?>
  <?php print $promotion; ?>
<?php endforeach; ?>
</div>