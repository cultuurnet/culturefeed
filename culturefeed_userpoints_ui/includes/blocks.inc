<?php
/**
 * @file
 * Blocks for the Culturefeed Userpoints module.
 */

/**
 * Returns a block with your userpoints balance.
 */
function culturefeed_userpoints_ui_block_balance() {
  
  $cf_user = DrupalCultureFeed::getLoggedInUser();
  if (!$cf_user) {
    return;
  }
  
  $output = array(
    '#points' => CultureFeedUserpointsWishlist::getRealPoints($cf_user->points),
    '#points_in_wishlist' => CultureFeedUserpointsWishlist::getPointsInWishlist(),
    '#points_left' => CultureFeedUserpointsWishlist::getPointsLeft($cf_user->points),
    '#theme' => 'culturefeed_userpoints_ui_my_balance',
  );

  return $output;
  
}
