<?php

function culturefeed_ui_block_similar_users_cache_id() {
  $account = menu_get_object('user');

  return array($account->uid);
}

function culturefeed_ui_block_similar_users_cache_expires() {
  return $_SERVER['REQUEST_TIME'] + variable_get('culturefeed_ui_similar_users_block_cache_expires', 3600);
}

function culturefeed_ui_block_similar_users_is_visible() {
  return culturefeed_ui_is_culturefeed_profile_page();
}

function culturefeed_ui_block_similar_users_view() {
  $account = menu_get_object('user');

  try {
    $users = DrupalCultureFeed::getSimilarUsers($account->culturefeed_uid);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    return;
  }

  if (empty($users->objects)) {
    return;
  }

  $items = culturefeed_ui_users_list($users);

  if (empty($items)) {
    return;
  }

  $block['subject'] = t('Similar users');
  $block['content'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
    '#attributes' => array('class' => 'user-list'),
  );

  return $block;
}

function culturefeed_ui_block_most_active_user_view() {
  $sort = variable_get('culturefeed_ui_block_most_active_user_sort', CultureFeed_SearchUsersQuery::SORT_NUMBEROFACTIVEACTIVITIES);

  $query = new CultureFeed_SearchUsersQuery();
  $query->max = 1;
  $query->order = CultureFeed_SearchUsersQuery::SORT_ORDER_DESCENDING;
  $query->sort = $sort;

  try {
    $cf_account = reset(DrupalCultureFeed::searchUsers($query)->objects);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    return;
  }

  if (!$cf_account) {
    return;
  }

  $block['subject'] = t('Most active user');
  $block['content'] = array(
    '#theme' => 'culturefeed_ui_most_active_user',
    '#account' => $cf_account,
    '#sort' => $sort,
  );

  return $block;
}

function culturefeed_ui_block_search_users_view() {
  $block['subject'] = t('Search users');
  $block['content'] = drupal_get_form('culturefeed_ui_users_search_form');

  return $block;
}

function culturefeed_ui_block_connect_is_visible() {
  if (user_is_anonymous()) {
    return TRUE;
  }

  return FALSE;
}

function culturefeed_ui_block_connect_view() {
  $block['subject'] = t('UiTiD connect');
  $block['content'] = array(
    '#theme' => 'culturefeed_ui_connect',
  );

  return $block;
}

function culturefeed_ui_block_profile_box_is_visible() {
  if (user_is_anonymous() || DrupalCultureFeed::isCultureFeedUser()) {
    return TRUE;
  }

  return FALSE;
}

function culturefeed_ui_block_profile_box_view() {
  if (DrupalCultureFeed::isCultureFeedUser()) {

    try {

      $cf_account = DrupalCultureFeed::getLoggedInUser();
      $uid = culturefeed_get_uid_for_cf_uid($cf_account->id, $cf_account->nick);
      $culture_feed_profile_name = array(
        'id' => $cf_account->id,
        'uid' => $uid,
        'nick' => $cf_account->nick,
        'link' => l($cf_account->nick, 'user/' . $uid),
      );
      drupal_alter('culturefeed_ui_profile_box_nick', $culture_feed_profile_name);

      // Get the main items. These are always visible.
      $main_items = module_invoke_all('culturefeed_ui_profile_box_main_items', $cf_account);
      drupal_alter('culturefeed_ui_profile_box_main_items', $main_items, $cf_account);

      // Get the items that should appear in the dropdown.
      $dropdown_items = module_invoke_all('culturefeed_ui_profile_box_dropdown_items', $cf_account);
      drupal_alter('culturefeed_ui_profile_box_dropdown_items', $dropdown_items, $cf_account);

      $content = array(
        '#theme' => 'culturefeed_ui_profile_box',
        '#profile_name' => $culture_feed_profile_name,
        '#account' => $cf_account,
        '#main_items' => $main_items,
        '#dropdown_items' => $dropdown_items,
      );

    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_ui', $e);
      return;
    }
  }
  else {
    // Get the main items. These are always visible.
    $main_items = module_invoke_all('culturefeed_ui_anonymous_profile_box_main_items');
    drupal_alter('culturefeed_ui_anonymous_profile_box_main_items', $main_items);

    $content = array(
      '#theme' => 'culturefeed_ui_anonymous_profile_box',
      '#account' => isset($cf_account) ? $cf_account : FALSE,
      '#main_items' => $main_items,
    );
  }

  $block['subject'] = '';
  $block['content'] = $content;

  return $block;
}

function culturefeed_ui_block_profile_menu_is_visible() {

  if (!DrupalCultureFeed::isCultureFeedUser()) {
    return FALSE;
  }

  if (arg(0) == 'user') {
    $account = menu_get_object('user');
    global $user;
    if (!$account || $account->uid != $user->uid) {
      return FALSE;
    }
  }

  return TRUE;

}

function culturefeed_ui_block_profile_menu_view() {

  global $user;

  $menu = module_invoke_all('culturefeed_ui_profile_menu');
  drupal_alter('culturefeed_ui_profile_menu', $menu);
  uasort($menu, 'drupal_sort_weight');

  $items = array();

  foreach ($menu as $menu_item) {
    $vars = array(
      'title' => $menu_item['title'],
      'url' => $menu_item['url'],
      'description' => $menu_item['description'],
    );

    $data = theme('culturefeed_ui_profile_menu_item', $vars);

    $class = $_GET['q'] == $menu_item['url'] ? array('active') : array();

    $items[] = array(
      'data' => $data,
      'class' => $class,
    );
  }

  $block['subject'] = t('Manage profile');
  $block['content'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
  );

  return $block;
}

/**
 * Returns the profile sections block.
 *
 * @return array
 *   The block.
 */
function culturefeed_ui_block_profile_sections_view() {

  if (DrupalCultureFeed::isCultureFeedUser()) {

    $sections = array();
    $hook = 'culturefeed_ui_profile_sections';
    foreach (module_implements($hook) as $module) {
      $function = $module . '_' . $hook;
      if (function_exists($function)) {
        $sections[] = call_user_func($function);
      }
    };

    if (count($sections)) {

      $build = array();

      foreach ($sections as $section) {

        $build[] = array(
          '#classes' => isset($section['classes']) ? $section['classes'] : array(),
          '#content' => $section['content'],
          '#theme' => 'culturefeed_ui_profile_section',
          '#title' => $section['title'],
        );

      }

      $block['subject'] = '';
      $block['content'] = $build;

      return $block;

    }

  }

}

function culturefeed_ui_block_mbox_confirmation_reminder_view() {
  if (!DrupalCultureFeed::isCultureFeedUser()) {
    return;
  }

  try {
    $cf_account = DrupalCultureFeed::getLoggedInUser();
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    return;
  }

  if ($cf_account->mboxVerified) {
    return;
  }

  $block['subject'] = 'Confirm email address';
  $block['content'] = theme('culturefeed_ui_mbox_confirmation_reminder', array('account' => $cf_account));

  return $block;
}


function culturefeed_ui_block_my_uitid_view() {
  try {
    $account = DrupalCultureFeed::getLoggedInUser();
    if (!$account) {
      return;
    }

    // get total likes.
    $query = new CultureFeed_SearchActivitiesQuery();
    $query->userId = $account->id;
    $query->type = CultureFeed_Activity::TYPE_LIKE;
    $query->max = 1;
    $like = DrupalCultureFeed::searchActivities($query)->total;

    // get total gotos.
    $query = new CultureFeed_SearchActivitiesQuery();
    $query->userId = $account->id;
    $query->type = CultureFeed_Activity::TYPE_IK_GA;
    $query->max = 1;
    $goto = DrupalCultureFeed::searchActivities($query)->total;

    $block['subject'] = t('My profile');
    $block['content'] = theme('culturefeed_ui_my_uitid', array(
      'account' => $account,
      'like' => $like,
      'goto' => $goto,
    ));

    return $block;

  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    return;
  }
}

function culturefeed_ui_block_my_uitid_is_visible() {
  return DrupalCultureFeed::isCultureFeedUser();
}


function culturefeed_ui_block_something_for_you_view() {
  $block['subject'] = t('Something for you?');
  $block['content'] = theme('culturefeed_ui_something_for_you');
  return $block;
}

function culturefeed_ui_block_something_for_you_is_visible() {
  return user_is_anonymous();
}
