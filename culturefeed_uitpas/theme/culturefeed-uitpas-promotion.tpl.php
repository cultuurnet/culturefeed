<?php
/**
 * @file
 * Default theme implementation to display culturefeed uitpas promotion.
 *
 * Available variables:
 * - $points: The number of points.
 * - $image
 * - $period: The period.
 * - $counters: The providing organisations.
 * - $provider: The providing cardsystem.
 * - $available: The availability.
 * - $description1.
 * - $description2.
 * - $out_of_stock: True or False
 */
?>
<div class="promotion_details">
  <?php if ($provider_raw): ?>
  <div class="provider-label">
    <span class="<?php print drupal_html_class($provider_raw); ?>"><?php print $provider_raw; ?></span>
  </div>
  <?php endif; ?>
  <div class="points"><?php print $points; ?></div>
  <dl class="clearfix">

    <?php if ($counters): ?>
    <dt><?php print t('Offered by'); ?></dt>
    <dd class="counters"><?php print $counters; ?></dd>
    <?php endif; ?>

    <?php if ($period || $out_of_stock): ?>
    <dt><?php print t('Availability'); ?></dt>
      <?php if ($out_of_stock): ?>
      <dd class="out-of-stock"><?php print t('Out of stock'); ?></dd>
      <?php else: ?>
      <dd class="period"><?php print $period; ?></dd>
      <?php endif; ?>
    <?php endif; ?>

    <?php if ($available): ?>
    <dt><?php print t('Only available for'); ?></dt>
    <dd class="available"><?php print $available; ?></dd>
    <?php endif; ?>

    <?php if ($description1): ?>
    <dt><?php print t('Conditions'); ?></dt>
    <dd class="description1"><?php print $description1; ?></dd>
    <?php endif; ?>

  </dl>

  <?php if ($description2): ?>
  <div class="how-to-exchange">
    <button class="show-exchange-info" onclick="Drupal.CultureFeed.UiTPASToggleExchangeInfo()"><?php print t('How to exchange'); ?></button>
    <div class="exchange-info">
      <div class="description2"><?php print $description2; ?></div>
    </div>
  </div>
  <?php endif; ?>
</div>
<div class="promotion_media">
  <?php if ($images_list): ?>
    <?php print $images_list; ?>
  <?php endif; ?>
</div>
