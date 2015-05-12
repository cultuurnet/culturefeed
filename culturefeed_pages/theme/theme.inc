<?php
/**
 * @file
 * Theming / preprocess functions for culturefeed pages.
 */


/**
 * Preprocess the culturefeed pages basic search page.
 * @see culturefeed-pages-basic-search-page.tpl.php
 */
function culturefeed_pages_preprocess_culturefeed_pages_basic_search_page(&$variables) {

  if (!empty($variables['results'])) {

    $variables['items'] = array();
    foreach ($variables['results'] as $item) {
      $variables['items'][] = theme('culturefeed_pages_basic_search_result_item', array('item' => $item));
    }

  }

  if ($variables['total_results'] > 0) {
    $variables['total_results_message'] = t("@total pages found for '@search'", array('@total' => $variables['total_results'], '@search' => $variables['search']));
  }
  else {
    $variables['total_results_message'] = t("No pages found for '@search'", array('@search' => $variables['search']));
  }

  $query = drupal_get_query_parameters();
  $variables['create_message'] = l(t('Create your own page'), 'pages/add', array('query' => $query));

  $variables['site'] = check_plain(variable_get('site_name', ''));
  $variables['search'] = check_plain($variables['search']);

}

/**
 * Preprocess a culturefeed page basic search result item.
 * @see culturefeed-pages-basic-search-result-item.tpl.php
 */
function culturefeed_pages_preprocess_culturefeed_pages_basic_search_result_item(&$variables) {

  $page = $variables['item']->getEntity();

  $variables['title'] = $page->getName();

  culturefeed_pages_preprocess_culturefeed_page_summary($variables);
  $variables['description'] = truncate_utf8($variables['description'], 125, FALSE, TRUE);

  drupal_add_library('system', 'drupal.ajax');

  // Local storage of the followers for the current user.
  $i_follow_pages = array();
  try {

    $cf_user = DrupalCultureFeed::getLoggedInUser();
    if ($cf_user && $cf_user->following) {
      foreach ($cf_user->following as $cfp_follower) {
        $i_follow_pages[] = $cfp_follower->page->getId();
      }
    }

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
  }

  $query = array('destination' => culturefeed_search_detail_path('page', $page->getId(), $page->getName()));

  // Anonymous user.
  $variables['logged_in'] = !empty($cf_user);
  $variables['following'] = FALSE;
  if (empty($cf_user)) {
    $variables['follow_text'] = theme('culturefeed_pages_login_required_message', array('page' => $page, 'action' => CultureFeed_Pages_Default::ACTION_FOLLOW));
    $variables['member_text'] = theme('culturefeed_pages_login_required_message', array('page' => $page, 'action' => CultureFeed_Pages_Default::ACTION_BECOME_MEMBER));
  }
  // Logged in user.
  else {

    // Member link.
    if (!culturefeed_pages_is_user_member_of_page($page->getId())) {
      $variables['become_member_url'] = url('culturefeed/pages/join/nojs/' . $page->getId(), array('query' => $query));
    }

    // Follow link.
    if (in_array($page->getId(), $i_follow_pages)) {
      $variables['following'] = TRUE;
      $variables['follow_url'] = url('culturefeed/pages/defollow/nojs/' . $page->getId(), array('query' => $query));
      $variables['follow_text'] = t('Unfollow');
    }
    else {
      $variables['follow_url'] = url('culturefeed/pages/follow/nojs/' . $page->getId(), array('query' => $query));
      $variables['follow_text'] = t('Follow');
    }
  }
}

/**
 * Preprocess the variables for the page summary.
 * @see culturefeed-page-summary.tpl.php
 */
function culturefeed_pages_preprocess_culturefeed_page_summary(&$variables) {

  _culturefeed_pages_preprocess_culturefeed_page($variables);

  $page = $variables['item']->getEntity();
  $variables['more_text'] = t('More info');
  $variables['url'] = culturefeed_search_detail_url('page', $page->getId(), $page->getName());

  $variables['member_count'] = $variables['item']->getActivityCount(\CultuurNet\Search\ActivityStatsExtendedEntity::ACTIVITY_COUNT_PAGE_MEMBER);
  $variables['follower_count'] = $variables['item']->getActivityCount(\CultuurNet\Search\ActivityStatsExtendedEntity::ACTIVITY_COUNT_PAGE_FOLLOW);

}

/**
 * Preprocess the variables for the page detail.
 * @see culturefeed-page.tpl.php
 */
function culturefeed_pages_preprocess_culturefeed_page(&$variables) {

  _culturefeed_pages_preprocess_culturefeed_page($variables);

  $page = $variables['item'];

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
      $member['name'] = check_plain($membership->user->nick);
      $member['relation'] = check_plain($membership->relation);
      $member['role'] = check_plain($membership->role);
      $member['picture'] = check_plain($membership->user->depiction);
      $member['validated'] = $membership->validated;

      $variables['members'][] = $member;
    }

  }

  _culturefeed_pages_preprocess_page_metatags($variables);

}

/**
 * Preprocess the general variables for a culturefeed page.
 */
function _culturefeed_pages_preprocess_culturefeed_page(&$variables) {

  $item = $variables['item'];
  if ($item instanceof CultureFeed_Cdb_Item_Page) {
    $page = $item;
  }
  else {
    $page = $item->getEntity();
  }

  $variables['title'] = check_plain($page->getName());
  $variables['id'] = $page->getId();
  $variables['description'] = check_markup($page->getDescription(), 'filtered_html');
  $variables['links'] = $page->getLinks();
  $variables['image'] = $page->getImage();
  $variables['thumbnail'] = $page->getImage();
  $variables['baseline'] = $page->getTagline();
  $variables['cover'] = $page->getCover();

  foreach ($variables['links'] as &$link) {
    $link = preg_replace('#^http://|^https://#', "", $link);
  }

  $logged_in = $variables['logged_in'];

  // Add join link if user is logged in and not a member yet.
  if (!culturefeed_pages_is_user_member_of_page($page->getId()) && $page->getPermissions()->allowMembers && $logged_in) {
    $query = array('destination' => culturefeed_search_detail_path('page', $page->getId(), $page->getName()), '/');
    $variables['become_member_link'] = l(t('Become a member'), 'culturefeed/pages/join/nojs/' . $page->getId(), array('query' => $query));
  }
  else {
    if (culturefeed_pages_is_user_member_of_page($page->getId()) && !culturefeed_pages_is_user_validated_admin_of_page($page->getId())) {
      $query = array('destination' => culturefeed_search_detail_path('page', $page->getId(), $page->getName()), '/');
      $variables['become_validated_admin_url'] = url('pages/' . $page->getId() . '/membership/request-validated-admin/nojs', array('query' => $query));
    }
  }

  // Address information
  $address = $page->getAddress();
  if (!empty($address)) {

    $variables['address'] = array();

    $variables['address']['street'] = '';
    if ($address->getStreet()) {
      $variables['address']['street'] = check_plain($address->getStreet() . ' ' . $address->getHouseNumber());
    }

    $variables['address']['city'] = check_plain($address->getCity());
    $variables['address']['zip'] = check_plain($address->getZip());

    $coordinates = $address->getGeoInformation();
    if ($coordinates) {
      $variables['coordinates'] = array(
        'lat' => $coordinates->getYCoordinate(),
        'lng' => $coordinates->getXCoordinate(),
      );
    }

  }

  // Contact information.
  $variables['contact'] = array();
  if ($page->getTelephone()) {
    $variables['contact']['tel'] = check_plain($page->getTelephone());
  }
  if ($page->getEmail()) {
    $variables['contact']['mail'] = check_plain($page->getEmail());
  }

  // Check if page has activities if we are bot on a searchable page.
  $current_search_page = culturefeed_get_searchable_type_by_path();
  $variables['has_activities'] = FALSE;
  if (module_exists('culturefeed_agenda' && !$current_search_page)) {
    $result = culturefeed_pages_get_activities_from_page($page, variable_get('pages_block_agenda_total_items', 5));
    $variables['has_activities'] = ($result && $result->getTotalCount() > 0);
  }

  // Check if current user already follows this page
  $i_follow_pages = array();
  try {

    $cf_user = DrupalCultureFeed::getLoggedInUser();
    if ($cf_user && $cf_user->following) {
      foreach ($cf_user->following as $cfp_follower) {
        $i_follow_pages[] = $cfp_follower->page->getId();
      }
    }

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_pages', $e);
  }

  $variables['following'] = FALSE;
  if (!empty($cf_user) && in_array($page->getId(), $i_follow_pages)) {
    $variables['following'] = TRUE;
  }

}

/**
 * Preprocess the variables for the page detail when invisible.
 * @see culturefeed-page-invisible.tpl.php
 */
function culturefeed_pages_preprocess_culturefeed_page_invisible(&$variables) {

  $page = $variables['page'];

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
    $colleague['name'] = check_plain($membership->user->nick);
    $colleague['picture'] = check_plain($membership->user->depiction);

    $colleagues[] = $colleague;

  }

  $variables['colleagues'] = $colleagues;
  $variables['nick'] = check_plain($variables['account']->nick);
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
 * Theme 1 followers block.
 * @param $variables
 */
function culturefeed_pages_preprocess_culturefeed_pages_followers_block(&$variables) {

  $page = $variables['page'];
  $variables['title'] = $page->getName();

  $uids = culturefeed_get_uids_for_memberships($variables['followers']);

  $followers = array();
  foreach ($variables['followers'] as $cf_follower) {

    // If user was not found in drupal, don't show it.
    if (!isset($uids[$cf_follower->user->id])) {
      continue;
    }

    $follower = array();
    $follower['url'] = url('user/' . $uids[$cf_follower->user->id]);
    $follower['name'] = check_plain($cf_follower->user->nick);
    $follower['picture'] = check_plain($cf_follower->user->depiction);

    $followers[] = $follower;

  }

  $variables['followers'] = $followers;
  $variables['follow_link'] = culturefeed_pages_follow_link($page);

}

/**
 * Theme 1 members block.
 * @param $variables
 */
function culturefeed_pages_preprocess_culturefeed_pages_members_block(&$variables) {

  $page = $variables['page'];
  $variables['title'] = $page->getName();

  $uids = culturefeed_get_uids_for_memberships($variables['members']);

  $members = array();
  foreach ($variables['members'] as $cf_member) {

    // If user was not found in drupal, don't show it.
    if (!isset($uids[$cf_member->user->id])) {
      continue;
    }

    $member = array();
    $member['url'] = url('user/' . $uids[$cf_member->user->id]);
    $member['name'] = check_plain($cf_member->user->nick);
    $member['picture'] = check_plain($cf_member->user->depiction);
    $member['relation'] = check_plain($cf_member->relation);
    $member['role'] = check_plain($cf_member->role);

    $members[] = $member;

  }

  $variables['members'] = $members;
  $variables['member_link'] = culturefeed_pages_member_link($page);
  //$variables['member_url'] =  url('culturefeed/pages/join/nojs/' . $variables['page']->getId(), array('query' => $query));
  $variables['member_url'] =  url('culturefeed/pages/join/nojs/' . $variables['page']->getId());

}


/**
 * Preprocess the variables for the page administration options.
 * @see culturefeed-pages-block-admin-options.tpl.php
 */
function culturefeed_pages_preprocess_culturefeed_pages_block_admin_options(&$variables) {

  $page = $variables['page'];
  $variables['switch_link'] = l(t('Login as') . ' ' . $page->getName(), 'pages/switch/' . $page->getId(), array('attributes' => array('class' => array('button'))));

  $variables['page_id'] = $page->getId();

  if ($variables['logged_in_as_page_admin']) {

    // Default to true, so message doesn't show when we have no agenda enabled.
    $variables['has_activities'] = TRUE;
    if (module_exists('culturefeed_agenda')) {
      // Use same item count as the block, so we hit the same cache.
      $result = culturefeed_pages_get_activities_from_page($page, variable_get('pages_block_agenda_total_items', 5));
      $variables['has_activities'] = ($result && $result->getTotalCount() > 0);
    }

    if ($variables['page_notifications'] && $variables['page_notifications']->total > 0) {
      $notification_variables = array(
        'activities' => $variables['page_notifications'],
        'class' => 'notifications',
        'read_more_url' => url('pages/' . $page->getId() . '/notifications'),
        'read_more_text' => t('View all notifications'),
      );
      $variables['notifications'] = theme('culturefeed_social_user_activity_list', $notification_variables);
    }
    else {
      $variables['notifications'] = t('Your page has no notifications yet');
    }

  }

}

/**
 * Preprocess the variables for the page administration block detail.
 * @see culturefeed-pages-page-menu-item.tpl.php
 */
function culturefeed_pages_preprocess_culturefeed_pages_page_menu_item(&$variables) {
  $variables['link'] = l($variables['title'], $variables['url']);
}

/**
 * Preprocess the search results on a user.
 * @see culturefeed-pages-user-search-result.tpl.php
 */
function culturefeed_pages_preprocess_culturefeed_pages_user_search_result(&$variables) {

  $variables['total'] = $variables['result']->total;
  $accounts = culturefeed_get_uids_for_users($variables['result']->objects);

  $members = array();
  foreach ($variables['user_list']->memberships as $membership) {
    $members[] = $membership->user->id;
  }

  $add_options = array(
    'attributes' => array(
      'role' => 'button',
      'data-toggle' => 'modal',
      'data-target' => '#page_confirm',
    ),
    'query' => drupal_get_query_parameters(),
  );

  $variables['results'] = array();
  foreach ($variables['result']->objects as $object) {

    if (!isset($accounts[$object->id])) {
      continue;
    }

    $result = array();
    $result['nick'] = check_plain($object->nick);
    $result['profile_link'] = l(t('View profile'), 'user/' . $accounts[$object->id]);
    $result['profile_url'] =  url('user/' . $accounts[$object->id]);
    $add_options['attributes']['data-remote'] = url('pages/' . $variables['page']->getId() . '/membership/add/' . $object->id . '/ajax', array('query' => $add_options['query']));
    $result['add_link'] = in_array($object->id, $members) ? t('Member') : l(t('Add as member'), 'pages/' . $variables['page']->getId() . '/membership/add/' . $object->id . '/nojs', $add_options);
    $variables['results'][] = $result;

  }

}

/**
 * Preprocess the culturefeed pages events admin page.
 * @see culturefeed-pages-events-admin-page.tpl.php
 */
function culturefeed_pages_preprocess_culturefeed_pages_events_admin_page(&$variables) {

  $rows = array();

  foreach ($variables['results'] as $result) {

    $row = array();

    $detail = $result->getEntity()->getDetails()->getDetailByLanguage(culturefeed_search_get_preferred_language());
    if ($detail) {

      $path = culturefeed_search_detail_path($result->getType(), $result->getId(), $detail->getTitle());
      $row['title'] = l($detail->getTitle(), $path);
      $row['date'] = check_plain($detail->getCalendarSummary());
      $comment_count = $result->getActivityCount(\CultuurNet\Search\ActivityStatsExtendedEntity::ACTIVITY_COUNT_COMMENT);
      if ($comment_count) {
        $row['comment_count'] = l(format_plural($comment_count, '@count comment', '@count comments'), $path, array('fragment' => t('Read comments')));
      }
      else {
        $row['comment_count'] = t('No comments');
      }
      $row['edit_url'] = l(t('Edit via UiTdatabank.be'), CULTUREFEED_PAGES_UITDATABANK_EVENT_URL . $result->getEntity()->getCdbid(), array('attributes' => array('target' => '_blank')));

      $rows[] = $row;

    }

  }

  if (!empty($rows)) {

    $header = array(
      array('data' => t('Title'), 'field' => 'title', 'sort' => 'asc'),
      array('data' => t('Date'), 'field' => 'startdate', 'sort' => 'asc'),
      array('data' => t('Reactions'), 'field' => 'comment_count', 'sort' => 'desc'),
      '',
    );
    $variables['items'] = theme('table', (array('header' => $header, 'rows' => $rows)));

  }

  $variables['view_page_link'] = culturefeed_search_detail_l('page', $variables['page']->getId(),     $variables['page']->getName(), t('View page'));
  $variables['page_link'] = culturefeed_search_detail_l('page', $variables['page']->getId(),     $variables['page']->getName(), $variables['page']->getName());

}

/**
 * Preprocess the variables for the block to become admin member.
 * @see culturefeed-page.tpl.php
 */
function culturefeed_pages_preprocess_culturefeed_pages_request_admin_membership_block(&$variables) {

  $page = $variables['page'];
  $cf_account_id = DrupalCultureFeed::getLoggedInUserId();

  $path = 'pages/' . $page->getId() . '/membership/request-admin/' . $cf_account_id . '/nojs';

  $variables['url'] = url($path, array('query' => drupal_get_destination()));
  $variables['link'] = '<span id="request_adminship_link_' . $page->getId() . '_wrapper">' . l(t('Send a request to become administrator'), $path, array('query' => drupal_get_destination(), 'attributes' => array('class' => array('use-ajax')))) . '</span>';

}

/**
 * Preprocess the variables for my pages overview.
 * @see culturefeed-pages-page-my-pages.tpl.php
 */
function culturefeed_pages_preprocess_culturefeed_pages_page_my_pages(&$variables) {

  $build = $variables['build'];
  $drupal_uid = culturefeed_get_uid_for_cf_uid($variables['user']->id, $variables['user']->nick);
  $variables['user_url'] = url('user/' . $drupal_uid);

  if (!empty($build['member_pages'])) {
    $variables['my_pages'] = drupal_render($build['member_pages']);
  }

  if (!empty($build['followed-pages'])) {
    $variables['i_follow'] = drupal_render($build['followed-pages']);
  }

  $variables['search_pages'] = drupal_render($build['search-pages']);

}

/**
 * Preprocess the variables for the page activities overview.
 * @see culturefeed-pages-page-agenda.tpl.php
 */
function culturefeed_pages_preprocess_culturefeed_pages_page_agenda(&$variables) {

  if (!empty($variables['read_more_path'])) {
    $query = array('organiser' => $variables['page']->getId());
    $variables['read_more'] = l(t('All activities of this organiser'), $variables['read_more_path'], array('query' => $query));
  }

  $variables['follow_link'] = culturefeed_pages_follow_link($variables['page']);

  $variables['is_following'] = culturefeed_pages_is_user_following_page($variables['page']->getId());

}

/**
 * Preprocess the variables for a suggestion list item.
 * @see
 */
function culturefeed_pages_preprocess_culturefeed_pages_page_suggestion_list_item(&$variables) {
  culturefeed_pages_preprocess_culturefeed_pages_basic_search_result_item($variables);
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

/**
 * Theme the edit membership form.
 */
function theme_culturefeed_pages_edit_membership_form($variables) {

  $cf_user = $variables['form']['#membership']->user;
  $page = $variables['form']['#membership']->page;

  $output = '<td>' . theme('image', array('path' => $cf_user->depiction . '?maxwidth=100')) . $cf_user->nick . '</td>';
  $output .= '<td>' . date('d/m/Y H:i', $variables['form']['#membership']->creationDate) . '</td>';

  $output .= '<td>' . drupal_render_children($variables['form']) . '</td>';

  return $output;

}

/**
 * Theme the message when a follow action was succesfull.
 */
function theme_culturefeed_pages_follow_success_message($variables) {

  $message = '<p><strong>' . t('You are following this page now') . '</strong></p>';

  if ($variables['page']->getPermissions()->allowMembers && !culturefeed_pages_is_user_member_of_page($variables['page']->getId())) {
    $message .= '<p>' . t('Want to join this page?') . ' ';
    $message .= l(t('Become a member'), 'culturefeed/pages/join/nojs/' . $variables['page']->getId(), array(
      'attributes' => array('class' => array('use-ajax'))));
  }

  return $message;

}

/**
 * Theme the message when an unfollow action was succesfull.
 */
function theme_culturefeed_pages_unfollow_success_message($variables) {
  return '<p><strong>' . t('You are no longer following this page anymore') . '</strong></p>';
}

/**
 * Theme the message when a user is anonymous, and the follow link would be displayed.
 */
function theme_culturefeed_pages_login_required_message($variables) {

  if ($variables['action'] == CultureFeed_Pages_Default::ACTION_BECOME_MEMBER) {
    $message = t('You must be logged in to join this page.');
  }
  else {
    $message = t('You must be logged in to follow this page.');
  }

  return '<div class="login-required">' . $message . '</div>';
}

/**
 * Theme the link to request administratorship for a page.
 */
function theme_culturefeed_pages_admin_request_link($variables) {

  $page = $variables['page'];
  $cf_user = $variables['cf_user'];

  $output = '<div id="request_adminship_link_' . $page->getId() . '_wrapper">';
  $output .= l(t('Become an administrator'), 'pages/' . $page->getId() . '/membership/request-admin/' . $cf_user->id . '/nojs', array(
    'query' => drupal_get_destination(),
    'attributes' => array(
      'class' => array('use-ajax button')
    )
  ));
  $output .= '</div>';

  return $output;
}

/**
 * Theme the message shown when a user can not delete his membership.
 */
function theme_culturefeed_pages_membership_delete_not_possible($variables) {

  $output = t('Not possible to remove');

  return $output;
}

/**
 * Helper preprocessor to add meta description tag.
 * @param type $variables
 */
function _culturefeed_pages_preprocess_page_metatags(&$variables) {

  $title_parts = array();
  $description_parts = array();

  if (!empty($variables['title'])) {
    $title_parts[] = $variables['title'];
  }
  if (!empty($variables['address'])) {
    $location = '';
    if (!empty($variables['address']['street'])) {
      $location .= $variables['address']['street'] . ', ';
    }
    if (!empty($variables['address']['zip'])) {
      $location .= $variables['address']['zip'];
    }
    if (!empty($variables['address']['city'])) {
      $location .= ' ' . $variables['address']['city'];
    }
    if (!empty($variables['address']['city'])) {
      $title_parts[] = $variables['address']['city'];
    }
    $description_parts[] = t('Address: @location', array('@location' => $location));
  }
  if (!empty($variables['contact']) && !empty($variables['contact']['tel'])) {
    $description_parts[] = t('Phone number: @number', array('@number' => $variables['contact']['tel']));
  }
  if (!empty($variables['description'])) {
    $description_parts[] = strip_tags($variables['description']);
  }

  // Add meta title tag.
  drupal_set_title(implode(' | ', $title_parts));

  // Add meta description field.
  drupal_add_html_head(array(
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'description',
      'content' => implode(' - ', $description_parts),
    ),
  ), 'meta_description');

}

/**
 * Preprocess a related items block.
 */
function culturefeed_pages_preprocess_culturefeed_pages_nearby_pages(&$variables) {

  global $language;
  global $base_url;
  // Render the results.
  $items = array();
  foreach ($variables['results'] as $key => $search_result) {
    $entity = $search_result->getEntity();

    $image = $entity->getImage();
    $location = $entity->getAddress()->getCity();
    $items[$key]['link'] = culturefeed_search_detail_l($search_result->getType(), $search_result->getId(), $search_result->getTitle($language->language));
    $items[$key]['image'] = $image;
    $items[$key]['location'] = $location;
  }
  $variables['items'] = $items;
}

/**
 * Preprocess the culturefeed pages news admin page.
 * @see culturefeed-pages-news-admin-page.tpl.php
 */
function culturefeed_pages_preprocess_culturefeed_pages_news_admin_page(&$variables) {

  $rows = array();
  $page_id = $variables['page']->getId();

  foreach ($variables['results'] as $activity) {

    // Use node title as default.
    $title = $activity->nodeTitle;

    // Real title is stored in the json.
    $content = json_decode($activity->value);
    if ($content && !empty($content->title)) {
      $title = $content->title;
    }

    $row = array();
    $row['title'] = $title;
    $row['url'] = url(culturefeed_search_detail_path('news', $activity->id, $row['title']));
    $row['date'] = format_date($activity->creationDate, 'custom', 'd M \o\m H:i');
    $row['delete_url'] = url('pages/' . $page_id . '/news/delete/' . $activity->id);

    $rows[] = $row;
  }

  $variables['items'] = $rows;
  $variables['view_page_link'] = culturefeed_search_detail_l('page', $variables['page']->getId(), $variables['page']->getName(), t('View page'));
  $variables['page_link'] = culturefeed_search_detail_l('page', $variables['page']->getId(), $variables['page']->getName(), $variables['page']->getName());
}

/**
 * Preprocess the page timeline block.
 * @see culturefeed-pages-block-page-timeline.tpl.php
 */
function culturefeed_pages_preprocess_culturefeed_pages_block_page_timeline(&$variables) {

  $variables['activities'] = theme('culturefeed_social_user_activity_wall', array('activities' => $variables['time_line']));
  $variables['page_admin'] = culturefeed_pages_is_user_admin_of_page($variables['page']->getId());
  $variables['page_id'] = $variables['page']->getId();
}
