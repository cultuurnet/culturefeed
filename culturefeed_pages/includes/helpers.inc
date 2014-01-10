<?php
/**
 * @file
 * Helper functions for culturefeed pages.
 */

/**
 * Get the correct follow url for current user.
 * If he already follows, it will return a unfollow link, otherwise a follow.
 * @param Culturefeed_Cdb_Item_Page $page
 *   Page to follow / unfollow.
 * @param $use_ajax
 *   Should the follow link use ajax or not.
 */
function culturefeed_pages_follow_link(CultureFeed_Cdb_Item_Page $page, $use_ajax = FALSE) {

  if (!DrupalCultureFeed::isCultureFeedUser()) {
    return theme('culturefeed_pages_login_required_message', array('page' => $page, 'action' => CultureFeed_Pages_Default::ACTION_FOLLOW));
  }

  $is_following = culturefeed_pages_is_user_following_page($page->getId());

  $options = array(
    'query' => array(
      'destination' => culturefeed_search_detail_path('page', $page->getId(), $page->getName())
    ),
  );

  if ($use_ajax) {
    drupal_add_library('system', 'drupal.ajax');
    $options['attributes'] = array('class' => array('use-ajax'));
  }

  if ($is_following) {
    return l(t('Unfollow page'), 'culturefeed/pages/defollow/nojs/' . $page->getId(), $options);
  }
  elseif ($page->getPermissions()->allowFollowers) {
    return l(t('Follow page'), 'culturefeed/pages/follow/nojs/' . $page->getId(), $options);
  }

}

/**
 * Is the current user member of a given page.
 * @return bool
 */
function culturefeed_pages_is_user_member_of_page($page_id) {

  try {
    $cf_account = DrupalCultureFeed::getLoggedInUser();

    if (!empty($cf_account->pageMemberships)) {
      foreach ($cf_account->pageMemberships as $membership) {
        if ($membership->page->getId() == $page_id) {
          return TRUE;
        }
      }
    }
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
  }

  return FALSE;

}

/**
 * Is the current user admin of a given page.
 * @return bool
 */
function culturefeed_pages_is_user_admin_of_page($page_id, $cf_account = NULL) {

  try {

    if (empty($cf_account)) {
      $cf_account = DrupalCultureFeed::getLoggedInUser();
    }

    if (!empty($cf_account->pageMemberships)) {
      foreach ($cf_account->pageMemberships as $membership) {
        if ($membership->role == CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN && $membership->page->getId() == $page_id) {
          return TRUE;
        }
      }
    }
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
  }

  return FALSE;

}

/**
 * Is the current user following a given page id.
 * @param string $page_id
 */
function culturefeed_pages_is_user_following_page($page_id) {

  try {

    $cf_account = DrupalCultureFeed::getLoggedInUser();

    if (!empty($cf_account->following)) {
      foreach ($cf_account->following as $follower) {
        if ($follower->page->getId() == $page_id) {
          return TRUE;
        }
      }
    }

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    return FALSE;
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
  return isset($_SESSION['culturefeed']['page']) ? $_SESSION['culturefeed']['page']->getId() : '';
}

/**
 * Sets the current page of the loggedIn user.
 */
function culturefeed_pages_set_active_page(CultureFeed_Cdb_Item_Page $page) {

  $_SESSION['culturefeed']['page'] = $page;

  $account = DrupalCultureFeed::getLoggedInAccount();
  $data = isset($account->data) ? $account->data : array();
  $data['culturefeed_pages_id'] = $page->getId();
  $account->data = $data;

  user_save($account);

}

/**
 * Remove the current page of the loggedIn user.
 */
function culturefeed_pages_remove_active_page() {

  unset($_SESSION['culturefeed']['page']);;

  $account = DrupalCultureFeed::getLoggedInAccount();
  $data = isset($account->data) ? $account->data : array();
  unset($data['culturefeed_pages_id']);
  $account->data = $data;

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

  $categories = $page->getCategories();
  $actor_types = culturefeed_search_get_actortype_categories(array('tid' => $categories));
  $facet = array();
  if ($actor_types) {

    $facet['category_actortype_id'] = array(key($actor_types));

    $active_trail[] = array(
      'title' => current($actor_types),
      'href' => 'agenda/pages',
      'link_path' => '',
      'localized_options' => array(
        'query' => array(
          'facet' => $facet,
        ),
      ),
      'type' => 0,
    );

  }

  $address = $page->getAddress();
  if ($address) {
    $city = $address->getCity();
    $postal = $address->getZip();
    if ($city && $postal) {
      $active_trail[] = array(
        'title' => $postal . ' ' . $city,
        'href' => 'agenda/pages',
        'link_path' => '',
        'localized_options' => array(
          'query' => array(
            'facet' => $facet,
            'location' => $postal . ' ' . $city,
          ),
        ),
        'type' => 0,
      );
    }
  }

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
    'title' => $page->getName(),
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
function culturefeed_pages_get_activities_from_page(CultureFeed_Cdb_Item_Page $page, $max_results, $page_nr = 0) {

  $parameters[] = new \CultuurNet\Search\Parameter\Rows($max_results);
  $parameters[] = new \CultuurNet\Search\Parameter\Group();
  $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery('type:event OR type:production');
  $parameters[] = new \CultuurNet\Search\Parameter\Query('"' . $page->getId() . '"');
  $parameters[] = new \CultuurNet\Search\Parameter\Start($page_nr);

  try {
    return culturefeed_get_search_service()->search($parameters);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    return NULL;
  }

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
  if (!$account) {
    return;
  }

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

/**
 * Get all the members of a given page. If a user is given, exclude him from the list.
 * @param CultureFeed_Cdb_Item_Page $page
 * @param mixed $exclude_user NULL or CultureFeed_User
 */
function culturefeed_pages_get_members_of_page(CultureFeed_Cdb_Item_Page $page, $exclude_user = NULL) {

  $user_list = DrupalCultureFeed::getConsumerInstance()->pages()->getUserList(
    $page->getId(),
    array(CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN, CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_MEMBER),
    FALSE
  );

  $memberships = array();
  if (!empty($user_list->memberships)) {
    foreach ($user_list->memberships as $user_list_membership) {
      // Exclude requested.
      if ($exclude_user && $user_list_membership->user->id == $exclude_user->id) {
        continue;
      }
      $memberships[] = $user_list_membership;
    }
  }

  return $memberships;

}

