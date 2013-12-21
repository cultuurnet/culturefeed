<?php

/**
 * @file
 * Default theme implementation to display culturefeed uitpas profile 
 * advantages.
 *
 * Available variables:
 * - $promotions_table: The list of promotions.
 * - $upcoming_promotions_table: The list of upcoming promotions.
 * - $advantages_table: The list of advantages.
 */
?>
<div class="profile_advantages">
  <div class="promotions"><?php print $promotions_table; ?></div>
  <div class="upcoming_promotions"><?php print $upcoming_promotions_table; ?></div>
  <div class="advantages"><?php print $advantages_table; ?></div>
</div>
