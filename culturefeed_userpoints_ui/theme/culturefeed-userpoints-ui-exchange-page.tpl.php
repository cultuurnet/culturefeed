<?php
/**
 * Template for the userpoints exchange page.
 *
 * @vars
 *   $points
 *     raw points from API
 *   $real_points
 *     points calculated to points for this site (E.g vliegpunten).
 *   $real_points_in_wishlist
 *     Site points in wishlist
 *   $real_points_left
 *     Calculated points currently left.
 *
 *   $cf_user
 *     the culturefeed user object
 *   $promotion_list
 *     String version of the list,
 *     see culturefeed-userpoints-ui-promotions-list.tpl.php
 *   $promotion_form
 *     The form to exchange points.
 *
 *   @Note
 *   - that the class <span class="userpoints-points"></span> will trigger
 *     updates on the amount of the points. Example cases:
 *       1) <span class="userpoints-points"><?php print $real_points_left ?></span>
 *       2) <span><?php print $real_points ?></span>
 *
 *   - that the id "culturefeed-userpoints-notifications" is used to anchor the external pages
 *     to this page bringing the points left and notifications in the picture.
 */

?>
<div id="culturefeed-userpoints-notifications">
   <?php print t('Remaining number of points'); ?>: <span class="userpoints-points"><?php print $real_points_left ?></span>
</div>

<?php if ($real_points < $minimum_points): ?>
  <div class="alert alert-block alert-success"><?php print t('You do not have enough points.'); ?> <strong><?php print t('From'); ?> <?php print $variables['minimum_points']; ?> <?php print t('points'); ?></strong> <?php print t('you can redeem your points for tickets, gadgets, coupons, discounts, ... .'); ?><br />
  <?php print t('Not enough points? Do not worry, learn how and how many points you can save each time.'); ?></div>
<?php endif; ?>

<?php print $promotions_list ?>

<?php print $wishlist ?>

<?php print $promotions_form ?>