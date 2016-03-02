<?php

/**
 * @file
 * Default theme implementation to display culturefeed uitpas promotions
 * highlight.
 *
 * Available variables:
 * - $items: The list of promotions.
 * - $promotions_table: Table of promotions.
 * - $more: Linked url of the full list.
 */
?>
<div class="promotions_highlight">
  <?php print $promotions_table; ?>
  <div class="more"><?php print $more; ?></div>
</div>
