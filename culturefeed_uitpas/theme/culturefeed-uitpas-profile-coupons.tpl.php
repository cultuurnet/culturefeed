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
    <div class="coupon-detail">
        <h3><?php print $coupon['name']; ?></h3>
        <?php if (!empty($coupon['description'])): ?>
            <?php print $coupon['description']; ?><br />
        <?php endif; ?>
        <?php if(isset($coupon['validTo'])): ?>
            Geldig tot: <?php print $coupon['validTo']; ?><br />
        <?php endif; ?>
        <?php if(!empty($coupon['remaining'])): ?>
            Nog beschikbaar: <?php print $coupon['remaining']; ?>
        <?php endif; ?>
    </div>
    <hr />
  <?php endforeach; ?>
</div>
