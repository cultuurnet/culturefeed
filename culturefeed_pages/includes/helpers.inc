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
 * Is the current user admin of a given page.
 * @return bool
 */
function culturefeed_pages_is_user_admin_of_page($page_id) {

  $cf_account = DrupalCultureFeed::getLoggedInUser();

  if (!empty($cf_account->pageMemberships)) {
    foreach ($cf_account->pageMemberships as $membership) {
      if ($membership->role == CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN && $membership->page->getId() == $page_id) {
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

/**
 * Set the breadcrumb to the given page.
 * @param CultureFeed_Cdb_Item_Page $page
 *   Page to set the breadcrumb to.
 */
function culturefeed_pages_set_page_breadcrumb(CultureFeed_Cdb_Item_Page $page) {

  // Set the breadcrumb.
  $active_trail = array();

  $active_trail[] = array(
    'title' => t('Home'),
    'href' => '<front>',
    'link_path' => '',
    'localized_options' => array(),
    'type' => 0,
  );

  $active_trail[] = array(
    'title' => "Pagina's",
    'href' => 'agenda/pages',
    'link_path' => '',
    'localized_options' => array(),
    'type' => 0,
  );

  $page_path = culturefeed_search_detail_path('page', $page->getId(), $page->getName());

  // When we are currently not on the page detail, also add breadcrumb to the page.
  if ($page_path != $_GET['q']) {
    $active_trail[] = array(
      'title' => $page->getName(),
      'href' => $page_path,
      'link_path' => '',
      'localized_options' => array(),
      'type' => 0,
    );
  }

  $active_trail[] = array(
    'title' => drupal_get_title(),
    'href' => $_GET['q'],
    'link_path' => '',
    'localized_options' => array(),
    'type' => 0,
  );

  menu_set_active_trail($active_trail);

}

/**
 * Search activities that are connected to the given page.
 * @return mixed \CultuurNet\Search\SearchResult or NULL on exception
 */
function culturefeed_pages_get_activities_from_page(CultureFeed_Cdb_Item_Page $page, $max_results) {

  $parameters[] = new \CultuurNet\Search\Parameter\Rows($max_results);
  $parameters[] = new \CultuurNet\Search\Parameter\Group();
  $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery('type:event OR type:production');
  $parameters[] = new \CultuurNet\Search\Parameter\Query('"' . $page->getId() . '"');

  $service = culturefeed_get_search_service();

  try {

    $searchService = culturefeed_get_search_service();
    $result = $searchService->search($parameters);

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    return NULL;
  }

  return $result;

}

/**
 * Helper function to create a piece of content to request adminship of a page.
 * 
 * @param CultureFeed_Cdb_Item_Page $page
 * @param integer $rendered
 *   Indicates whether the output needs to be rendered or not.
 */
function culturefeed_pages_block_request_admin_membership(CultureFeed_Cdb_Item_Page $page, $rendered = TRUE) {

  // Only for pages if the current user is already member of the page AND
  // if the page does not have a default admin yet.
  $is_member = FALSE;
  $is_admin = FALSE;
  $number_of_admins = 0;

  $account = DrupalCultureFeed::getLoggedInAccount();
  $cf_pages = DrupalCultureFeed::getConsumerInstance()->pages();
  $user_list = $cf_pages->getUserList($page->getId(), array(CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN, CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_MEMBER), FALSE);
  
  if (!empty($user_list->memberships)) {
    foreach ($user_list->memberships as $user_list_membership) {
      if ($user_list_membership->user->id == $account->culturefeed_uid) {
        $is_member = TRUE;
        if ($user_list_membership->role == CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN) {
          $is_admin = TRUE;
        }
      }
      if ($user_list_membership->role == CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN) {
        $number_of_admins++;
      }
    }
  }
  
  // Skip if the user is not even member.
  if (!$is_member) {
    return;
  }
  
  // Skip if the user is already admin.
  if ($is_admin) {
    return;
  }
  
  $build = array(
    '#theme' => 'culturefeed_pages_request_admin_membership_block',
    '#page' => $page,
  );
  
  if ($rendered) {
    return drupal_render($build);
  }
  else {
    return $build;
  }
}
