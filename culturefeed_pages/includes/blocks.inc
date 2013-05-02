<?php
/**
 * @file
 * Block callbacks for culturefeed pages.
 */

/**
 * Show the fellow members from a user inside a block.
 */
function culturefeed_pages_block_fellow_members($account) {

  if (empty($account->culturefeed_uid)) {
    return;
  }

  $cf_pages = DrupalCultureFeed::getConsumerInstance()->pages();
  $cf_account = DrupalCultureFeed::getUser($account->culturefeed_uid);

  $build = array();
  // Render colleagues from first 3 memberships.
  if (empty($cf_account->pageMemberships)) {
    return;
  }

  foreach ($cf_account->pageMemberships as $i => $membership) {

    if ($i == 3) {
      break;
    }

    $memberships = array();
    $user_list = $cf_pages->getUserList($membership->page->getId(), array(CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN, CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_MEMBER), FALSE);

    if (!empty($user_list->memberships)) {
      foreach ($user_list->memberships as $user_list_membership) {
        // Don't show current user.
        if ($user_list_membership->user->id == $account->culturefeed_uid) {
          continue;
        }
        $memberships[] = $user_list_membership;
      }
    }

    $build[] = array(
      '#theme' => 'culturefeed_pages_fellow_members_block',
      '#page' => $membership->page,
      '#members' => $memberships,
      '#account' => $cf_account,
    );

  }

  $block = array();
  $block['subject'] = "Mijn collega's";
  $block['content'] = $build;

  return $block;

}

/**
 * Show the pages that a given user follows in a block.
 */
function culturefeed_pages_block_pages_user_follows($account) {

  if (empty($account->culturefeed_uid)) {
    return;
  }

  $cf_account = DrupalCultureFeed::getUser($account->culturefeed_uid);

  if (empty($cf_account->following)) {
    return;
  }

  $block = array();
  $block['subject'] = "Pagina's die ik volg";
  $block['content'] = array(
    '#theme' => 'culturefeed_pages_following_pages_block',
    '#following' => $cf_account->following,
  );

  return $block;

}

/**
 * Show the administrative options for the current page.
 */
function culturefeed_pages_block_pages_admin_options() {

  $page = menu_get_object('culturefeed_pages_page', 3);

  if (!$page) {
    return;
  }

  // This refers very much to the access callback, but we offer all
  // components to the templates.
  // @see culturefeed_pages_edit_page_access()
  $cultureFeedUser = DrupalCultureFeed::getLoggedInUser();

  $isVisible = $page->isVisible();
  $isPageAdmin = FALSE;
  $loggedInAsPageAdmin = FALSE;

  // Calculate the number of pages where the current user is ADMIN.
  if ($cultureFeedUser->adminPagesCount > 0) {
    $adminMemberships = $cultureFeedUser->getMembershipsByRole(CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN);

    // Check the current page against the current active admin page.
    $isPageAdmin = in_array($page->getId(), array_keys($adminMemberships));
    $loggedInAsPageAdmin = $page->getId() === culturefeed_pages_get_active_page_id();
  }

  // Page admins are allowed to administer the page.
  if ($isPageAdmin && $loggedInAsPageAdmin) {
    $admin_menu = _culturefeed_pages_block_pages_admin_menu($page);
  }
  // If the current user is admin of the current page, we ask the
  // user if he wants to login as the page administrator.
  elseif (!$isPageAdmin) {
    return;
  }

  $block = array();
  $block['subject'] = "Pagina beheren";
  $block['content'] = array(
    '#theme' => 'culturefeed_pages_block_admin_options',
    '#admin_menu' => isset($admin_menu) ? $admin_menu : "",
    '#account' => $cultureFeedUser,
    '#page' => $page,
    '#is_page_admin' => $isPageAdmin,
    '#logged_in_as_page_admin' => $loggedInAsPageAdmin,
  );

  return $block;

}

/**
 * Show the administration menu for the current page.
 */
function culturefeed_pages_block_pages_admin_menu() {

  $page = menu_get_object('culturefeed_pages_page', 1);

  if (!$page) {
    return;
  }

  if (!culturefeed_pages_edit_page_access($page)) {
    return;
  }

  $block = array();
  $block['subject'] = "Pagina beheren";
  $block['content'] = _culturefeed_pages_block_pages_admin_menu($page);

  return $block;

}

/**
 * Helper function to output the admin menu for a page.
 * @param CultureFeed_Cdb_Item_Page $page
 */
function _culturefeed_pages_block_pages_admin_menu(CultureFeed_Cdb_Item_Page $page) {

  $menu = array(
    array(
      'title' => 'Basisinformatie',
      'url' => 'pages/' . $page->getId() . '/edit',
      'description' => '(bewerk contactinfo, foto, beschrijving, website, ...)',
    ),
    array(
      'title' => 'Instellingen',
      'url' => 'pages/' . $page->getId() . '/configuration',
      'description' => '(Mogen gebruikers jouw pagina volgen, beoordelingen schrijven, activiteiten aanraden, ...)',
    ),
    array(
      'title' => 'Berichten',
      'url' => 'pages/' . $page->getId() . '/messages',
      'description' => '(Lees en beantwoord berichten en boekingsaanvragen)',
    ),
    array(
      'title' => 'Leden & rollen',
      'url' => 'pages/' . $page->getId() . '/members/nojs',
      'description' => '(Beheer leden en toegangsrechten)',
    ),
    array(
      'title' => 'Activiteiten',
      'url' => 'pages/' . $page->getId() . '/activiteiten',
      'description' => '(Beheer je activiteiten en beoordelingen)',
    ),
    array(
      'title' => 'Statistieken',
      'url' => 'pages/' . $page->getId() . '/stats',
      'description' => '',
    ),
  );

  drupal_alter('culturefeed_pages_page_admin_menu', $menu);

  $items = array();

  foreach ($menu as $menu_item) {
    $vars = array(
      'title' => $menu_item['title'],
      'url' => $menu_item['url'],
      'description' => $menu_item['description'],
    );

    $data = theme('culturefeed_pages_page_menu_item', $vars);

    $class = $_GET['q'] == $menu_item['url'] ? array('active') : array();

    $items[] = array(
      'data' => $data,
      'class' => $class,
    );
  }

  $admin_menu = theme('item_list', array('items' => $items));

  return $admin_menu;

}

/**
 * Show the timeline of a page in a block.
 */
function culturefeed_pages_block_page_timeline($page) {

  if (!$page || !$page instanceof CultureFeed_Cdb_Item_Page) {
    return;
  }

  $content = array();
  try {

    $activities = DrupalCultureFeed::getLoggedInUserInstance()->pages()->getTimeline($page->getId());

    if (empty($activities->objects)) {
      $content['activities'] = array(
        '#markup' => 'Deze pagina heeft nog geen acties.',
      );
    }
    else {

      $content['activities'] = array(
        '#theme' => 'culturefeed_social_user_activity_list',
        '#activities' => $activities,
        '#class' => 'page-time-line',
      );

    }

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
  }

  $block['subject'] = 'Tijdslijn';
  $block['content'] = $content;

  return $block;

}

/**
 * Returns a block with followers of a certain type of content.
 * @param misc $object
 */
function culturefeed_pages_block_followers($object) {

  $account = DrupalCultureFeed::getLoggedInAccount();
  
  if (empty($account->culturefeed_uid)) {
    return;
  }
  
  $cf_pages = DrupalCultureFeed::getConsumerInstance()->pages();
  $cf_account = DrupalCultureFeed::getUser($account->culturefeed_uid);

  $build = array();

  $followers = array();
  $you_follow = FALSE;
  
  // Render all followers.
  if (!empty($cf_account->following)) {

    foreach ($cf_account->following as $follower) {
      if ($follower->user->id != $account->culturefeed_uid) {
        $followers[] = $follower;
      }
      else {
        $you_follow = TRUE;
      }
    }
    
    $followers = array_slice($followers, 0, 6);

  }

  $build['items_list'] = array(
    '#theme' => 'culturefeed_pages_followers_block',
    '#page' => $object,
    '#followers' => $followers,
    '#num_followers' => count($cf_account->following),
    '#you_follow' => $you_follow,
  );

  $block['subject'] = 'Volgen deze pagina';
  $block['content'] = $build;

  return $block;
}
