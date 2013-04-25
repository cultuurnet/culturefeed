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

/**
 * Sets all the pages where the loggedIn user is ADMIN.
 */
function culturefeed_pages_set_pages($pages) {
  $_SESSION['culturefeed']['pages'] = $pages;
}

/**
 * Checks if the loggedIn user has an active page (yet).
 * 
 * @return Boolean hasActivePage.
 */
function culturefeed_pages_has_active_page() {
  return isset($_SESSION['culturefeed']['page']);
}

/**
 * Get the current page of the loggedIn user.
 * 
 * @return CultureFeed_Cdb_Item_Page $page
 */
function culturefeed_pages_get_active_page() {
  return isset($_SESSION['culturefeed']['page']) ? $_SESSION['culturefeed']['page'] : NULL;
}

/**
 * Get the current page ID of the loggedIn user.
 * 
 * @return String $id
 */
function culturefeed_pages_get_active_page_id() {
  return isset($_SESSION['culturefeed']['page']) ? $_SESSION['culturefeed']['page']->getId() : "";
}

/**
 * Sets the current page of the loggedIn user.
 */
function culturefeed_pages_set_active_page(CultureFeed_Cdb_Item_Page $page) {
  $_SESSION['culturefeed']['page'] = $page;
  
  $account = DrupalCultureFeed::getLoggedInAccount();
  $account->data = array(
    'culturefeed_pages_id' => $page->getId(),
  );
  user_save($account);
  
}
