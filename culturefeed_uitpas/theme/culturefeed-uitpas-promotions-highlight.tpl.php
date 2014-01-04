<?php

/**
 * @file
 * Default theme implementation to display culturefeed uitpas promotions 
 * highlight.
 *
 * Available variables:
 * - $promotions_table: The list of promotions.
 * - $more: Linked url of the full list.
 */
?>
<div class="promotions_highlight">
  <?php print $promotions_table; ?>
  <div class="more"><?php print $more; ?></div>
</div>
