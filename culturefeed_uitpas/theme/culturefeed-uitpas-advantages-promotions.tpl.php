<?php

/**
 * @file
 * Default theme implementation to display culturefeed uitpas advantages
 * promotions.
 *
 * Available variables:
 * - $promotions_table: The list of promotions.
 * - $advantages_table: The list of advantages.
 * - $info: Info text.
 */
?>
<div class="advantages_promotions">
  <div class="promotions_table"><?php print $promotions_table; ?></div>
  <div class="advantages_table"><?php print $advantages_table; ?></div>
  <div class="info"><?php print $info; ?></div>
</div>
