<?php

/**
 * @file
 * Default theme implementation to display culturefeed uitpas advantages
 * promotions.
 *
 * Available variables:
 * - $advantages_table: The list of advantages.
 * - $info: Info text.
 */
?>

<div id="advantages_link">
  <ul>
    <li><a href="/promotions"><?php print t('Promotions') ?></li>
    <li><a href="/advantages"><?php print t('Welcome Advantages') ?></li>
  </ul>
</div>

<div class="advantages">
  <div class="advantages_table"><?php print $advantages_table; ?></div>
  <div class="info"><?php print $info; ?></div>
</div>
