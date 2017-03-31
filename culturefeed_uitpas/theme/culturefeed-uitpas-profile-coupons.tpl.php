<?php

/**
 * @file
 * Default theme implementation to display culturefeed uitpas profile actions.
 *
 * Available variables:
 * - $actions_form: Form to set publishing of actions.
 * - $intro: Intro text.
 * - $actions_table: The list of actions.
 */
?>
<div class="profile_coupons">
  <?php foreach ($coupons as $coupon): ?>
    <?php print $coupon['name']; ?><br />
    <?php print $coupon['description']; ?><br />
    Geldig tot: <?php print $coupon['validTo']; ?><br />
    Nog beschikbaar: <?php print $coupon['remaining']; ?>
  <?php endforeach; ?>
</div>
