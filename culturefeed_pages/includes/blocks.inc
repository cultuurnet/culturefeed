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

  try {
    $cf_account = DrupalCultureFeed::getUser($account->culturefeed_uid);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    return;
  }

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
    $memberships = culturefeed_pages_get_members_of_page($membership->page, $cf_account);

    $build[] = array(
      '#theme' => 'culturefeed_pages_fellow_members_block',
      '#page' => $membership->page,
      '#members' => $memberships,
      '#account' => $cf_account,
    );

  }

  $block = array();
  if ($account->uid != $GLOBALS['user']->uid) {
    $block['subject'] = t('Colleague\'s of @username', array('@username' => $account->name));
  }
  else {
    $block['subject'] = t('My colleague\'s');
  }
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

  try {
    $cf_account = DrupalCultureFeed::getUser($account->culturefeed_uid);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    return;
  }

  if (empty($cf_account->following)) {
    return;
  }

  $block = array();
  if ($GLOBALS['user']->uid == $account->uid) {
    $block['subject'] = t('Pages I follow');
  }
  else {
    $block['subject'] = t('@username follows', array('@username' => $account->name));
  }

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

  $page = culturefeed_pages_get_viewed_page();

  if (!$page || !culturefeed_is_culturefeed_user()) {
    return;
  }

  $is_page_admin = culturefeed_pages_is_user_admin_of_page($page->getId());

  if (!$is_page_admin) {
    return;
  }

  try {
    $cf_user = DrupalCultureFeed::getLoggedInUser();
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    return;
  }

  $notifications = NULL;
  $admin_menu = '';
  $loggedin_as_page_admin = $page->getId() === culturefeed_pages_get_active_page_id();

  // If the users active page is the current page, show the admin menu.
  if ($loggedin_as_page_admin) {

    $admin_menu = _culturefeed_pages_block_pages_admin_menu($page);

    if (module_exists('culturefeed_social')) {

      $params = array(
        'max' => CULTUREFEED_SOCIAL_BLOCK_NOTIFICATIONS_COUNT,
        'dateFrom' => date('Y-m-d', strtotime('-1 year')),
      );

      try {
        $notifications = DrupalCultureFeed::getLoggedInUserInstance()->pages()->getNotifications($page->getId(), $params);
      }
      catch (Exception $e) {
        watchdog_exception('culturefeed_pages', $e);
      }

    }


  }

  $block = array();
  $block['subject'] = t('Manage pages');
  $block['content'] = array(
    '#theme' => 'culturefeed_pages_block_admin_options',
    '#admin_menu' => $admin_menu,
    '#account' => $cf_user,
    '#page' => $page,
    '#logged_in_as_page_admin' => $loggedin_as_page_admin,
    '#page_notifications' => $notifications,
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
  $block['subject'] = t('Manage pages');
  $block['content'] = _culturefeed_pages_block_pages_admin_menu($page);

  return $block;

}

/**
 * Helper function to output the admin menu for a page.
 * @param CultureFeed_Cdb_Item_Page $page
 */
function _culturefeed_pages_block_pages_admin_menu(CultureFeed_Cdb_Item_Page $page) {

  $menu = module_invoke_all('culturefeed_pages_page_admin_menu', $page);
  drupal_alter('culturefeed_pages_page_admin_menu', $menu, $page);

  uasort($menu, 'drupal_sort_weight');

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

  $admin_menu = theme('item_list', array('items' => $items, 'attributes' => array('class' => 'dropdown-menu', 'role' => 'menu')));

  return $admin_menu;

}

/**
 * Block with a request-link to become admin member of a page.
 *
 * @param CultureFeed_Cdb_Item_Page $page
 */
function culturefeed_pages_block_pages_request_admin_membership(CultureFeed_Cdb_Item_Page $page) {

  $block = array();
  $block['subject'] = '';
  $block['content'] = culturefeed_pages_block_request_admin_membership($page, FALSE);

  return $block;
}

/**
 * Show the timeline of a page in a block.
 */
function culturefeed_pages_block_page_timeline($page) {

  if (!$page || !$page instanceof CultureFeed_Cdb_Item_Page || !module_exists('culturefeed_social')) {
    return;
  }

  try {
    $active_types = array_keys(culturefeed_social_get_activity_filter_options(TRUE));
    $time_line = DrupalCultureFeed::getConsumerInstance()->pages()->getTimeline($page->getId(), NULL, $active_types);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    return;
  }

  if ($time_line->total == 0) {
    return;
  }

  $filter_form = drupal_get_form('culturefeed_pages_timeline_filter_form', 'all', $page->getId());

  $block = array();
  $block['subject'] = t('Timeline');
  $block['content'] = array(
    '#theme' => 'culturefeed_pages_block_page_timeline',
    '#time_line' => $time_line,
    '#page' => $page,
    '#filter_form' => $filter_form,
  );

  return $block;

}

/**
 * Show the agenda of a page in a block.
 */
function culturefeed_pages_block_page_agenda($page) {

  if (!$page || !$page instanceof CultureFeed_Cdb_Item_Page || !module_exists('culturefeed_agenda')) {
    return;
  }

  // Only an organisation has an agenda.
  $categories = $page->getCategories();
  if (!in_array(CULTUREFEED_ACTORTYPE_ORGANISATION, $categories)) {
    return;
  }

  $is_admin = culturefeed_pages_get_active_page_id() == $page->getId();
  $max_results = variable_get('pages_block_agenda_total_items', 20);

  // Execute search and show results.
  $result = culturefeed_pages_get_activities_from_page($page, $max_results);
  if (!$result || $result->getTotalCount() == 0) {
    return;
  }

  // Render the results.
  $search_results = $result->getItems();
  $items = array();
  foreach ($search_results as $search_result) {
    $items[] = theme('culturefeed_' . $search_result->getType() . '_mini_summary', array('item' => $search_result));
  }

  $block = array();
  $block['subject'] = t('Activities');

  $block['content'] = array(
    '#theme' => 'culturefeed_pages_page_agenda',
    '#items' => $items,
    '#is_admin' => $is_admin,
    '#page' => $page,
    '#class' => 'page-activities',
  );

  if ($result->getTotalCount() > $max_results) {
    $block['content']['#read_more_path'] = 'agenda/search';
  }

  return $block;

}

/**
 * Returns a block with followers of a certain type of content.
 * @param CultureFeed_Cdb_Item_Page $page
 */
function culturefeed_pages_block_followers($page) {

  if (empty($page) || !($page instanceof CultureFeed_Cdb_Item_Page)) {
    return;
  }

  $user_list = NULL;
  try {
    $cf_pages = DrupalCultureFeed::getConsumerInstance()->pages();
    $user_list = $cf_pages->getUserList($page->getId(), array(CultureFeed_Pages_Follower::ROLE), FALSE);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    return;
  }

  $build = array();

  $followers = array();

  $build['items_list'] = array(
    '#theme' => 'culturefeed_pages_followers_block',
    '#page' => $page,
    '#followers' => $user_list->followers,
    '#num_followers' => count($user_list->followers),
  );

  $block['subject'] = t('Page followers');
  $block['content'] = $build;

  return $block;
}

/**
 * Returns a block with members of a certain type of content.
 * @param CultureFeed_Cdb_Item_Page $page
 */
function culturefeed_pages_block_members($page) {

  if (empty($page) || !($page instanceof CultureFeed_Cdb_Item_Page)) {
    return;
  }

  $member_list = NULL;
  try {
    $cf_pages = DrupalCultureFeed::getConsumerInstance()->pages();
    $member_list = $cf_pages->getUserList($page->getId(), array(CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_MEMBER, CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN), FALSE);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
    return;
  }

  $build = array();

  $build['items_list'] = array(
    '#theme' => 'culturefeed_pages_members_block',
    '#page' => $page,
    '#members' => $member_list->memberships,
    '#num_members' => count($member_list->memberships),
  );

  $block['subject'] = t('Members');
  $block['content'] = $build;

  return $block;
}

/**
 * Block callback to show the related activities of a page.
 */
function culturefeed_pages_block_related_activities($page) {

  if (!$page) {
    return;
  }

  $parameters = array();
  $parameters[] = new \CultuurNet\Search\Parameter\Query('*:*');
  $parameters[] = new \CultuurNet\Search\Parameter\Rows(variable_get('page_related_actitivities_total_items', 5));
  $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery('type:event OR type:production');
  $parameters[] = new \CultuurNet\Search\Parameter\Group();

  // Add the context filter.
  $context = variable_get('page_related_actitivities_context');
  if ($context) {

    $categories = $item->getEntity()->getCategories()->getCategoriesByType($context);
    if ($categories) {
      $context_filter = array();
      foreach ($categories as $category) {
        $context_filter[] = $category->getId();
      }
      $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery('category_id' . ':(' . implode(' OR ', $context_filter) . ')');
    }

  }

  // Add the sort.
  if ($sort_field = variable_get('agenda_related_activities_sort', '')) {
    $parameters[] = new \CultuurNet\Search\Parameter\Sort($sort_field, variable_get('agenda_related_activities_sort_direction', \CultuurNet\Search\Parameter\Sort::DIRECTION_ASC));
  }

  // Add extra query.
  if ($filter_query = variable_get('agenda_related_activities_extra_query', '')) {
    $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery($filter_query);
  }

  // Add spatial search.
  if ($item->getType() == 'event' && variable_get('agenda_related_activities_range', 0)) {

    $address = $item->getEntity()->getLocation()->getAddress();
    if ($address) {
      $coordinates = $address->getPhysicalAddress()->getGeoInformation();
      if ($coordinates) {
        $distance = new \CultuurNet\Search\Parameter\Spatial\Distance(variable_get('agenda_related_activities_range', 0));
        $point = new \CultuurNet\Search\Parameter\Spatial\Point($coordinates->getYCoordinate(), $coordinates->getXCoordinate());
        $field = new \CultuurNet\Search\Parameter\Spatial\SpatialField('physical_gis');
        $parameters[] = new \CultuurNet\Search\Parameter\Spatial\GeoFilterQuery($point, $distance, $field);
      }
    }

  }

  // Execute search.
  try {

    $result = culturefeed_get_search_service()->search($parameters);

    if ($result->getTotalCount() == 0) {
      return;
    }

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_agenda', $e);
  }

  global $language;
  $search_results = $result->getItems();
  // Render the results.
  $items = array();
  foreach ($search_results as $search_result) {

    // Don't add current activity to the list.
    if ($search_result->getEntity()->getCdbId() == $item->getEntity()->getCdbid()) {
      continue;
    }

    $items[] = theme('culturefeed_' . $search_result->getType() . '_short_summary', array('item' => $search_result));
  }

  $block = array();
  $block['subject'] = t('Related activities');
  $block['content'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
    '#class' => 'related-activities'
  );

  return $block;

}

/**
 * Show block with a list of pages nearby given page.
 */
function culturefeed_pages_block_nearby_pages($page) {

  if (empty($page)) {
    return;
  }

  $actortypes = variable_get('culturefeed_pages_actor_types', array());

  $parameters = array();
  $parameters[] = new \CultuurNet\Search\Parameter\Query('*:*');
  $parameters[] = new \CultuurNet\Search\Parameter\Rows(CULTUREFEED_PAGE_NEARBY_PAGES_COUNT);
  $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery('!cdbid' . ':' . $page->getId());
  $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery('category_id' . ':(' . implode(' OR ', $actortypes) . ')');

  // If no address information, don't show nearby activities.
  $address = $page->getAddress();
  if (!$address) {
    return;
  }

  // Add proximity search.
  $coordinates = $address->getGeoInformation();
  $zip = $address->getZip();
  if ($coordinates) {
    $distance = new \CultuurNet\Search\Parameter\Spatial\Distance(CULTUREFEED_PAGE_NEARBY_PAGES_RANGE);
    $point = new \CultuurNet\Search\Parameter\Spatial\Point($coordinates->getYCoordinate(), $coordinates->getXCoordinate());
    $field = new \CultuurNet\Search\Parameter\Spatial\SpatialField('physical_gis');
    $parameters[] = new \CultuurNet\Search\Parameter\Spatial\GeoFilterQuery($point, $distance, $field);

    $more_query['coordinates'] = $coordinates->getYCoordinate() . ',' . $coordinates->getXCoordinate();
  }

  // Use zip as fallback for missing coordinates
  elseif ($zip) {
    $parameters[] = new \CultuurNet\Search\Parameter\FilterQuery('zipcode' . ':' . $zip);
    $more_query['location'] = $zip;
  }

  // Execute search.
  $show_more_link = FALSE;
  try {

    $searchService = culturefeed_get_search_service();
    $result = $searchService->searchPages($parameters);

    if ($result->getTotalCount() == 0) {
      return;
    }

    $show_more_link = $result->getTotalCount()  > CULTUREFEED_PAGE_NEARBY_PAGES_COUNT;

  }
  catch (Exception $e) {
    watchdog_exception('cultuurkuur', $e);
    return;
  }
  $more_query['facet'] = array();
  //$more_query['facet']['category_actortype_id'][0] = $context_id;

  $block = array();
  $block['subject'] = t('Nearby Pages');
  $block['content'] = array(
    '#class' => 'nearby-pages',
    '#theme' => 'culturefeed_pages_nearby_pages',
    '#results' => $result->getItems(),
    '#show_more' => $show_more_link,
    '#more_text' => t('More nearby pages'),
    '#more_url' => url('agenda/pages', array('query' => $more_query)),
  );

  return $block;

}

/**
 * Show a block with suggestions of pages for current user.
 */
function culturefeed_pages_block_page_suggestions() {

  // Start with an empty list. On page load, the suggestions will be loaded through ajax.
  return array(
    'subject' => t('Discover these pages'),
    'content' => array(
      '#theme' => 'culturefeed_pages_block_page_suggestions',
      '#filter_form' => drupal_get_form('culturefeed_pages_page_suggestions_filter_form'),
      '#attached' => array(
        'js' => array(
          drupal_get_path('module', 'culturefeed_pages') . '/js/page_suggestions.js',
        )
      ),
    ),
  );
}
