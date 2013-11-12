<?php

/**
 * @file
 * Default theme implementation to display culturefeed uitpas advantage details.
 *
 * Available variables:
 * - $period: The period.
 * - $location: The location.
 * - $available: The availability.
 */
?>
<div class="advantage_details">
  <?php if ($period): ?>
  <div class="period"><?php print $period; ?></div>
  <?php endif; ?>
  <div class="location"><?php print $location; ?></div>
  <div class="available"><?php print $available; ?></div>
</div>