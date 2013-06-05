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

  $output = array(
    '#points' => $cf_user->points,
    '#theme' => 'culturefeed_userpoints_ui_my_balance',
  );

  return $output;
  
}
