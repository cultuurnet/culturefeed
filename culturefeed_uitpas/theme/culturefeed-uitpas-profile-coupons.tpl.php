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
<div class="profile-coupons">
  <?php foreach ($coupons as $coupon): ?>
    <div class="coupon-detail">
        <h3><?php print $coupon['name']; ?></h3>
        <?php if (!empty($coupon['description'])): ?>
            <span class="coupon-detail-description"><?php print $coupon['description']; ?></span>
        <?php endif; ?>
        <?php if(isset($coupon['validTo'])): ?>
            <span class="coupon-detail-validto"><strong>Geldig tot:</strong> <?php print $coupon['validTo']; ?></span>
        <?php endif; ?>
        <?php if(isset($coupon['remaining'])): ?>
            <span class="coupon-detail-remaining"><strong>Beschikbaar:</strong> <?php print $coupon['remaining']; ?></span>
        <?php endif; ?>
    </div>
    <hr />
  <?php endforeach; ?>
</div>
