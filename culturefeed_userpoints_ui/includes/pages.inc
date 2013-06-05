<?php
/**
 * Page callback for the userpoints handling.
 */

/**
 * Page callback to list the gifts and exchange the points for a gift.
 */
function culturefeed_userpoints_ui_exchange_page() {
  
  $cf_account = DrupalCultureFeed::getLoggedInUser();
  
  $output = array(
    '#theme' => 'culturefeed_userpoints_ui_exchange_page',
    '#points' => $cf_account->points,
    '#cf_user' => $cf_account,
  );
  
  return $output;
  
}
