<?php

/**
 * @file
 * Default theme implementation to display culturefeed uitpas profile 
 * advantages.
 *
 * Available variables:
 * - $promotions: The list of promotions.
 * - $upcoming_promotions: The list of upcoming promotions.
 * - $advantages: The list of advantages.
 */
?>
<div class="profile_advantages">
  <div class="promotions"><?php print $promotions; ?></div>
  <div class="upcoming_promotions"><?php print $upcoming_promotions; ?></div>
  <div class="advantages"><?php print $advantages; ?></div>
</div>
