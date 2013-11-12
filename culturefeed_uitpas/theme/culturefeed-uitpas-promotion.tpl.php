<?php

/**
 * @file
 * Default theme implementation to display culturefeed uitpas promotions 
 * highlight.
 *
 * Available variables:
 * - $points: The number of points.
 * - $period: The period.
 * - $location: The location.
 * - $available: The availability.
 */
?>
<div class="promotion_details">
  <div class="points"><?php print $points; ?></div>
  <?php if ($period): ?>
  <div class="period"><?php print $period; ?></div>
  <?php endif; ?>
  <div class="location"><?php print $location; ?></div>
  <div class="available"><?php print $available; ?></div>
</div>
