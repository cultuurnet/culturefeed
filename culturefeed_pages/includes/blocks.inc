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
  
  $cultureFeedUser = DrupalCultureFeed::getLoggedInUser();
  
  // Calculate the number of pages where the current user is ADMIN.
  if ($cultureFeedUser->adminPagesCount > 0) {
    $adminMemberships = $cultureFeedUser->getMembershipsByRole(CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN);
  }
  dsm($adminMemberships, 'user');

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
      'url' => 'pages/' . $page->getId() . '/members-and-roles',
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

  $block = array();
  $block['subject'] = "Pagina beheren";
  $block['content'] = array(
    '#theme' => 'culturefeed_pages_block_admin_options',
    '#admin_menu' => $admin_menu,
    '#account' => $cultureFeedUser,
    '#page' => $page,
  );
  
  return $block;
}

