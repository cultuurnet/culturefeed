<?php

/**
 * @file
 * Default theme implementation to display culturefeed uitpas promotions 
 * highlight.
 *
 * Available variables:
 * - $points: The number of points.
 * - $image
 * - $period: The period.
 * - $location: The location.
 * - $available: The availability.
 * - $description1.
 * - $description2.
 */
?>
<div class="promotion_details">
  <?php if ($image): ?>
  <div class="image"><?php print $image; ?></div>
  <?php endif; ?>
  <div class="points"><?php print $points; ?></div>
  <?php if ($period): ?>
  <div class="period"><?php print $period; ?></div>
  <?php endif; ?>
  <div class="location"><?php print $location; ?></div>
  <div class="available"><?php print $available; ?></div>
  <?php if ($description1): ?>
  <div class="description1"><?php print $description1; ?></div>
  <?php endif; ?>
  <?php if ($description2): ?>
  <div class="description2"><?php print $description2; ?></div>
  <?php endif; ?>
</div>
