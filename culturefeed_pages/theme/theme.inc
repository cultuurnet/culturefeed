<?php
/**
 * @file
 * Theming / preprocess functions for culturefeed pages.
 */


/**
 * Preprocess the culturefeed pages join page.
 * @see culturefeed-pages-join-page.tpl.php
 */
function culturefeed_pages_preprocess_culturefeed_pages_join_page(&$variables) {

  $variables['message'] = '';

  if (!empty($variables['result'])) {

    $items = $variables['result']->getItems();
    $variables['items'] = array();
    foreach ($items as $item) {
      $variables['items'][] = theme('culturfeed_pages_join_search_result_item', array('item' => $item));
    }
    $variables['create_message'] = 'Heb je nog geen pagina? ' . l('Maak een nieuwe pagina', 'pages/add');

  }

  $variables['site'] = check_plain(variable_get('site_name', ''));
  $variables['search'] = check_plain($variables['search']);

}

/**
 * Preprocess a culturefeed page join result item.
 * @see culturefeed-pages-join-search-result-item.tpl.php
 */
function culturefeed_pages_preprocess_culturfeed_pages_join_search_result_item(&$variables) {

  $page = $variables['item']->getEntity();

  $variables['title'] = $page->getName();

  $address = $page->getAddress();
  if ($address) {
    $variables['location']['street'] = $address->getStreet();
    $variables['location']['city'] = $address->getCity();
    $variables['location']['country'] = $address->getCountry();
  }

  drupal_add_library('system', 'drupal.ajax');

  if (!culturefeed_pages_is_user_member_of_page($page->getId())) {
    $query = array('destination' => culturefeed_search_detail_path('page', $page->getId(), $page->getName()));
    $variables['become_member_link'] = l('Word lid', 'culturefeed/pages/join/nojs/' . $page->getId(), array('query' => $query));
  }

  $variables['view_page_link'] = culturefeed_search_detail_l('page', $page->getId(), $page->getName(), 'Bekijk pagina');

}

/**
 * Preprocess the variables for the page detail.
 * @see culturefeed-page.tpl.php
 */
function culturefeed_pages_preprocess_culturefeed_page(&$variables) {

  $page = $variables['page'];

  $variables['title'] = $page->getName();
  $variables['description'] = $page->getDescription();
  $variables['links'] = $page->getLinks();
  $variables['image'] = $page->getImage();
  
  // Add join link if not a member yet.
  if (!culturefeed_pages_is_user_member_of_page($page->getId())) {
    $query = array('destination' => culturefeed_search_detail_path('page', $page->getId(), $page->getName()), '/');
    $variables['become_member_link'] = l('Word zelf lid', 'culturefeed/pages/join/nojs/' . $page->getId(), array('query' => $query));
  }

  // Address information
  $address = $page->getAddress();
  if (!empty($address)) {

    $variables['address'] = array();
    $variables['address']['street'] = $address->getStreet() . ' ' . $address->getHouseNumber();
    $variables['address']['city'] = $address->getCity();
    $variables['address']['zip'] = $address->getZip();

    $coordinates = $address->getGeoInformation();
    if ($coordinates) {
      $variables['coordinates'] = array(
        'lat' => $coordinates->getXCoordinate(),
        'lng' => $coordinates->getYCoordinate(),
      );
    }

  }

  // Contact information.
  $variables['contact'] = array();
  if ($page->getTelephone()) {
    $variables['contact'][] = $page->getTelephone();
  }
  if ($page->getEmail()) {
    $variables['contact'][] = $page->getEmail();
  }

  $cf_pages = DrupalCultureFeed::getConsumerInstance()->pages();
  $user_list = $cf_pages->getUserList($page->getId(), array(CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN, CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_MEMBER), FALSE);
  $variables['user_list'] = $user_list;

  // First put all memberships in an array, so we can get the drupal uids for it.
  $variables['members'] = array();
  if (!empty($user_list->memberships)) {

    foreach ($user_list->memberships as $user_list_membership) {
      $memberships[] = $user_list_membership;
    }

    $uids = culturefeed_get_uids_for_memberships($memberships);

    // Add every membership to the variables.
    foreach ($memberships as $membership) {

      // If user was not found in drupal, don't show it.
      if (!isset($uids[$membership->user->id])) {
        continue;
      }

      $member = array();
      $member['url'] = url('user/' . $uids[$membership->user->id]);
      $member['name'] = $membership->user->nick;
      $member['role'] = $membership->role;
      $member['picture'] = $membership->user->depiction;

      $variables['members'][] = $member;
    }

  }

}

/**
 * Preprocess the variables for the page detail when invisible.
 * @see culturefeed-page-invisible.tpl.php
 */
function culturefeed_pages_preprocess_culturefeed_page_invisible(&$variables) {
  
  $page = $variables['page'];

  $variables['title'] = $page->getName();
  
}

/**
 * Theme 1 fellow members block.
 * @param $variables
 */
function culturefeed_pages_preprocess_culturefeed_pages_fellow_members_block(&$variables) {

  $uids = culturefeed_get_uids_for_memberships($variables['members']);

  $colleagues = array();
  foreach ($variables['members'] as $membership) {

    // If user was not found in drupal, don't show it.
    if (!isset($uids[$membership->user->id])) {
      continue;
    }

    $colleague = array();
    $colleague['url'] = url('user/' . $uids[$membership->user->id]);
    $colleague['name'] = $membership->user->nick;
    $colleague['picture'] = $membership->user->depiction;

    $colleagues[] = $colleague;

  }

  $variables['colleagues'] = $colleagues;
  $variables['nick'] = $variables['account']->nick;
  $variables['title'] = $variables['page']->getName();

  if (culturefeed_pages_is_user_member_of_page($variables['page']->getId())) {
    $variables['is_member'] = TRUE;
  }
  else {
    $variables['is_member'] = FALSE;
    $query = array('destination' => culturefeed_search_detail_url('page', $variables['page']->getId(), $variables['page']->getName()));
    $variables['become_member_url'] = url('culturefeed/pages/join/nojs/' . $variables['page']->getId(), array('query' => $query));
  }

}

/**
 * Preprocess the variables for the page administration options.
 * @see culturefeed-pages-block-admin-options.tpl.php
 */
function culturefeed_pages_preprocess_culturefeed_pages_block_admin_options(&$variables) {
  $variables['switch_link'] = l($variables['page']->getName(), 'pages/switch/' . $variables['page']->getId(), array('attributes' => array('class' => array('button'))));
}

/**
 * Preprocess the variables for the page administration block detail.
 * @see culturefeed-pages-page-menu-item.tpl.php
 */
function culturefeed_pages_preprocess_culturefeed_pages_page_menu_item(&$variables) {
  $variables['link'] = l($variables['title'], $variables['url']);
}

/**
 * Theme the overview of pages that a user follows in a block.
 */
function theme_culturefeed_pages_following_pages_block($variables) {

  $items = array();
  foreach ($variables['following'] as $following) {
    $items[] = culturefeed_search_detail_l('page', $following->page->getId(), $following->page->getName());
  }

  return theme('item_list', array('items' => $items, 'attributes' => array('class' => array('pages-following'))));

}
