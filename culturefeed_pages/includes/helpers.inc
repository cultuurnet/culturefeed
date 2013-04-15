<?php
/**
 * @file
 * Helper functions for culturefeed pages.
 */

/**
 * Is the current user member of a given page.
 * @return bool
 */
function culturefeed_pages_is_user_member_of_page($page_id) {

  $cf_account = DrupalCultureFeed::getLoggedInUser();

  if (!empty($cf_account->pageMemberships)) {
    foreach ($cf_account->pageMemberships as $membership) {
      if ($membership->page->getId() == $page_id) {
        return TRUE;
      }
    }
  }

  return FALSE;

}