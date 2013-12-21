<?php

/**
 * @file
 * Default theme implementation to display culturefeed uitpas my uitpas summary.
 *
 * Available variables:
 * - $image: user image.
 * - $name: user name.
 * - $points: number of points saved.
 * - $promotions_title: promotions title.
 * - $promotions_links: list of promotions.
 * - $upcoming_promotions_title: upcoming promotions title.
 * - $upcoming_promotions_links: upcoming promotions.
 * - $all_promotions: all promotions link.
 */
?>
<div class="profile_summary">
  <div class="details">
    <?php if ($image): ?><div class="image"><?php print $image; ?></div><?php endif; ?>
    <div class="name"><?php print $name; ?></div>
    <div class="points"><?php print $points; ?></div>
  </div>
  <div class="promotions">
  <h3><?php print $promotions_title; ?></h3>
    <?php print $promotions_links; ?>
  </div>
  <?php if ($upcoming_promotions): ?>
  <div class="upcoming_promotions">
    <h3><?php print $upcoming_promotions_title; ?></h3>
    <?php print $upcoming_promotions_links; ?>
  </div>
  <?php endif; ?>
  <div class="all_promotions">
    <?php print $all_promotions; ?>
  </div>
</div>
