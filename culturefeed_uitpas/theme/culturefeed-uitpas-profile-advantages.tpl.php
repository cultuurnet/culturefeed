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
<div id="profile_advantages_link">
  <ul>
    <li><a href="/culturefeed/profile/uitpas/promotions"><?php print t('Promotions') ?></li>
    <li><a href="/culturefeed/profile/uitpas/advantages"><?php print t('Welcome Advantages') ?></li>
  </ul>
</div>

<div class="profile_advantages">
  <div class="advantages"><?php print $advantages_table; ?></div>
</div>
