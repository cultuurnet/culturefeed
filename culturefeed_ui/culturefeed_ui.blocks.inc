<?php

function culturefeed_ui_block_view_actions() {
  $request = cnapi_ui_get_active_request();

  // We only act upon cnapi requests.
  if (!$request) {
    return;
  }

  $type = reset(array_intersect(array('actor', 'event', 'production'), array_keys($request)));

  if (!$type) {
    return;
  }
  
  $query = new CultureFeed_SearchActivitiesQuery();
  $query->type = array(
    CultureFeed_Activity::TYPE_LIKE,
  );
  $query->nodeId = $request[$type];
  $query->consumer = variable_get('culturefeed_api_application_key', '');
  $query->userId = DrupalCultureFeed::getLoggedInAccount()->culturefeed_uid;
  $query->private = TRUE;

  try {
    $activities = DrupalCultureFeed::searchActivities($query);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    return;
  }

  if (empty($activities->objects)) {
    $like = l('Like', 'culturefeed/like/' . $type . '/' . $request[$type]);
  }
  else {
    $like = 'you like this ' . l('Unlike', 'culturefeed/unlike/' . $type . '/' . $request[$type]);
  }
  
  $block['subject'] = t('Actions');
  $block['content'] = $like;

  return $block;
}

function culturefeed_ui_block_view_mbox_confirmation_reminder() {
  if (user_is_anonymous() || !DrupalCultureFeed::isCultureFeedUser()) {
    return;
  }
  
  try {
    $cf_account = DrupalCultureFeed::getLoggedInUser();
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
  }
  
  if ($cf_account->mboxVerified) {
    return;
  }
  
  $block['subject'] = t('Confirm your mail address');
  $block['content'] = theme('culturefeed_ui_mbox_confirmation_reminder', array('account' => $cf_account));

  return $block;
}

function culturefeed_ui_block_view_search_users() {
  $block['subject'] = t('Search users');
  $block['content'] = drupal_get_form('culturefeed_ui_search_users_form');

  return $block;
}

function culturefeed_ui_block_view_profile_box() {
  if (!user_is_anonymous() && !DrupalCultureFeed::isCultureFeedUser()) {
    return;
  }

  $nick = '';
  $picture = '';
  $url_profile = '';

  if (!user_is_anonymous()) {
    try {
      $cf_account = DrupalCultureFeed::getLoggedInUser();
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_ui', $e);
    }

    if (isset($cf_account)) {
      $nick = $cf_account->nick;
      $picture = $cf_account->depiction . '?maxwidth=100&maxheight=100&crop=auto';
      $url_profile = 'user/' . culturefeed_get_uid_for_cf_uid($cf_account->id, $cf_account->nick);
    }
  }

  $block['subject'] = '';
  $block['content'] = array(
    '#theme' => 'culturefeed_ui_profile_box',
    '#nick' => $nick,
    '#picture' => $picture,
    '#url_profile' => $url_profile,
  );

  return $block;
}

function culturefeed_ui_block_view_connect() {
  if (!user_is_anonymous()) {
    return;
  }

  $block['subject'] = 'Maak zelf een UiTid';
  $block['content'] = array(
    '#theme' => 'culturefeed_ui_connect',
  );

  return $block;
}

function culturefeed_ui_block_view_most_active_user() {
  $sort = variable_get('culturefeed_ui_block_most_active_user_sort', CultureFeed_SearchUsersQuery::SORT_NUMBEROFACTIVEACTIVITIES);

  $query = new CultureFeed_SearchUsersQuery();
  $query->max = 1;
  $query->order = CultureFeed_SearchUsersQuery::SORT_ORDER_DESCENDING;
  $query->sort = $sort;

  try {
    $account = reset(DrupalCultureFeed::searchUsers($query)->objects);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    return;
  }

  $block['subject'] = 'UiTslover van de week';
  $block['content'] = array(
    '#theme' => 'culturefeed_ui_most_active_user',
    '#account' => $account,
    '#sort' => $sort,
    '#sort_value' => $account->sortValue,
    '#url_view_all' => 'culturefeed/users/mostactive',
  );

  return $block;
}

function culturefeed_ui_block_view_top_events() {
  $sort = variable_get('culturefeed_ui_block_top_events_sort', CultureFeed::TOP_EVENTS_SORT_ACTIVE);

  try {
    $events = DrupalCultureFeed::getTopEvents($sort, 5); // @todo make max configurable
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    return;
  }

  $items = array();

  $events_info = culturefeed_get_nodes_for_cdbids('event', $events);

  foreach ($events as $cdbid) {
    if (!isset($events_info[$cdbid])) {
      continue;
    }

    $event = $events_info[$cdbid];

    $request = array('event' => $cdbid, 'title' => $event['title']);

    $items[] = cnapi_url_dp2dul($event['title'], $request);
  }

  // @todo make this themeable
  $block['subject'] = t('Top events');
  $block['content'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
    '#type' => 'ol',
  );

  return $block;
}

function culturefeed_ui_block_cache_id_similar_users() {
  $account = menu_get_object('user');
  
  return array($account->uid);
}

function culturefeed_ui_block_is_visible_similar_users() {
  $account = menu_get_object('user');

  if (!$account || !DrupalCultureFeed::isCultureFeedUser($account->uid)) {
    return FALSE;
  }
  
  return TRUE;
}

function culturefeed_ui_block_view_similar_users() {
  $account = menu_get_object('user');

  try {
    $cf_account = DrupalCultureFeed::getConsumerInstance()->getUser($account->culturefeed_uid, FALSE, FALSE);

    $users = DrupalCultureFeed::getSimilarUsers($cf_account->id);
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

  $block['subject'] = t('Users similar to !name', array('!name' => $cf_account->nick));
  $block['content'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
  );

  return $block;
}

function culturefeed_ui_block_view_user_picture() {
  global $user;

  $account = menu_get_object('user');

  if (!$account || !DrupalCultureFeed::isCultureFeedUser($account->uid)) {
    return;
  }

  try {
    if ($user->uid == $account->uid) {
      $cf_account = DrupalCultureFeed::getLoggedInUser();
    }
    else {
      $cf_account = DrupalCultureFeed::getConsumerInstance()->getUser($account->culturefeed_uid, FALSE, FALSE);
    }
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    return;
  }

  $path = $cf_account->depiction . '?maxwidth=200';

  $block['subject'] = '';
  $block['content'] = array(
    array(
      '#theme' => 'image',
      '#path' => $path,
    ),
  );

  if ($user->uid == $account->uid) {
    $block['content'][] = array('#markup' => l(t('Change picture'), 'culturefeed/profile/edit'));
  }

  return $block;
}

function culturefeed_ui_block_view_profile_menu() {
  if (!DrupalCultureFeed::isCultureFeedUser()) {
    return;
  }
  
  $menu = array(
    array(
      'title' => 'Mijn UiTid',
      'url' => 'user/' . culturefeed_get_or_create_user(DrupalCultureFeed::getLoggedInUserId())->uid,
      'description' => 'Zo zien anderen jouw profiel.',
    ),
    array(
      'title' => 'Profiel bewerken',
      'url' => 'culturefeed/profile/edit',
      'description' => 'Wijzig naam, adres, favoriet categoriëen, ...',
    ),
    array(
      'title' => 'Account bewerken',
      'url' => 'culturefeed/account/edit',
      'description' => 'Wijzig paswoord, e-mailadres, toepassingen verbonden met jouw account, ...',
    ),
    array(
      'title' => 'Aanbevelingen',
      'url' => 'culturefeed/recommendations',
      'description' => 'Ontdek activiteiten op jouw maat.',
    ),
  );

  $items = array();
  
  foreach ($menu as $menu_item) {
    $items[] = array(
      'data' => theme('culturefeed_ui_profile_menu_item', array('title' => $menu_item['title'], 'url' => $menu_item['url'], 'description' => $menu_item['description'])),
      'class' => $_GET['q'] == $menu_item['url'] ? 'active' : '',
    );    
  }

  $block['subject'] = t('Profile');
  $block['content'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
  );

  return $block;
}

function culturefeed_ui_block_view_user_activities() {
  global $user;

  $account = menu_get_object('user');

  if (!$account || !DrupalCultureFeed::isCultureFeedUser($account->uid)) {
    return;
  }

  $query = new CultureFeed_SearchActivitiesQuery();
  $query->max = 50;
  $query->type = array(
    CultureFeed_Activity::TYPE_LIKE,
    CultureFeed_Activity::TYPE_FACEBOOK,
    CultureFeed_Activity::TYPE_TWITTER,
    CultureFeed_Activity::TYPE_IK_GA
  );
  $query->userId = $account->culturefeed_uid;
  if ($user->uid == $account->uid) {
    $query->private = TRUE;
  }

  try {
    $activities = DrupalCultureFeed::searchActivities($query);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    return;
  }

  $items = culturefeed_ui_activities_list($activities);
  
  if (empty($items)) {
    return;
  }

  $block['subject'] = t('User activities');
  $block['content'] = array(
    array(
      '#theme' => 'item_list',
      '#items' => $items,
    ),
  );

  return $block;
}

function culturefeed_ui_block_view_recent_activities() {
  $query = new CultureFeed_SearchActivitiesQuery();
  $query->max = 5; // @todo make this configurable
  $query->contentType = CultureFeed_Activity::CONTENT_TYPE_EVENT;
  $query->type = array(
    CultureFeed_Activity::TYPE_LIKE,
    CultureFeed_Activity::TYPE_FACEBOOK,
    CultureFeed_Activity::TYPE_TWITTER,
    CultureFeed_Activity::TYPE_IK_GA
  );

  try {
    $activities = DrupalCultureFeed::searchActivities($query);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    return;
  }

  $items = culturefeed_ui_activities_list($activities);
  
  if (empty($items)) {
    return;
  }

  $block['subject'] = t('Recent activities');
  $block['content'] = array(
    array(
      '#theme' => 'item_list',
      '#items' => $items,
    ),
    array(
      '#markup' => l(t('Older activities'), 'culturefeed/activities/recent'),
    ),
  );

  return $block;
}

function culturefeed_ui_block_view_users_likes() {
  $request = cnapi_ui_get_active_request();

  // We only act upon cnapi requests.
  if (!$request) {
    return;
  }

  $type = reset(array_intersect(array('actor', 'event', 'production'), array_keys($request)));

  if (!$type) {
    return;
  }

  $query = new CultureFeed_SearchActivitiesQuery();
  $query->type = CultureFeed_Activity::TYPE_LIKE;
  $query->nodeId = $request[$type];

  try {
    $users = DrupalCultureFeed::searchActivityUsers($request[$type], CultureFeed_Activity::TYPE_LIKE);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    return;
  }

  $items = culturefeed_ui_users_list($users);
  
  if (empty($items)) {
    return;
  }

  $block['subject'] = t('Likes');
  $block['content'] = array(
    array(
      '#theme' => 'item_list',
      '#items' => $items,
    ),
    array(
      '#markup' => l('Bekijk iedereen', 'culturefeed/users/likes/event/' . $request[$type]),
    )
  );

  return $block;
}

function culturefeed_ui_block_view_users_attends() {
  $request = cnapi_ui_get_active_request();

  // We only act upon cnapi requests.
  if (!$request) {
    return;
  }

  $type = reset(array_intersect(array('actor', 'event', 'production'), array_keys($request)));

  if (!$type) {
    return;
  }

  $query = new CultureFeed_SearchActivitiesQuery();
  $query->type = CultureFeed_Activity::TYPE_IK_GA;
  $query->nodeId = $request[$type];

  try {
    $users = DrupalCultureFeed::searchActivityUsers($request[$type], CultureFeed_Activity::TYPE_IK_GA);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    return;
  }

  $items = culturefeed_ui_users_list($users);
  
  if (empty($items)) {
    return;
  }

  $block['subject'] = t('Attends');
  $block['content'] = array(
    array(
      '#theme' => 'item_list',
      '#items' => $items,
    ),
    array(
      '#markup' => l('Bekijk iedereen', 'culturefeed/users/likes/event/' . $request[$type]),
    )
  );

  return $block;
}

function culturefeed_ui_block_cache_id_related_events() {
  $request = cnapi_ui_get_active_request();
  
  return array($request['event']);
}

function culturefeed_ui_block_is_visible_related_events() {
  $request = cnapi_ui_get_active_request();

  // We only act upon cnapi requests.
  if (!$request) {
    return FALSE;
  }

  $is_event = array_search('event', array_keys($request)) !== FALSE;

  if (!$is_event) {
    return FALSE;
  }
  
  return TRUE;
}

function culturefeed_ui_block_view_related_events() {
  $request = cnapi_ui_get_active_request();
  
  $event = cnapi_get_event($request['event']);
  
  $headings = cnapi_get_headings();
  
  $zip = $event['location']['address']['zipcode'];
  $event_headings = isset($event['headings']) ? $event['headings'] : array();
  
  $valid_headings = array();
  
  if (!empty($event_headings)) {
    foreach ($event_headings as $heading) {
      $heading = $headings[$heading];
      if (!cnapi_output_type_has_heading_groups() || (cnapi_output_type_has_heading_groups() && $heading['pid'] != 0)) {
        $valid_headings[] = $heading;
      }
    }
  }
  
  $query = new CultureFeed_RecommendationsQuery();
  $query->max = 4;
  
  try {
    $recommendations = DrupalCultureFeed::getRecommendationsForEvent($request['event'], $query);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    return;
  }
  
  $cdbid = array();
  
  foreach ($recommendations as $recommendation) {
    $cdbids[] = $recommendation->itemid;
  }
  
  $events = culturefeed_get_nodes_for_cdbids('event', $cdbids);

  $block['subject'] = t('Related events');
  $block['content'] = array(
    '#theme' => 'culturefeed_ui_related_events', 
    '#events' => $events,
    '#zip' => $zip,
    '#headings' => $valid_headings, 
  );

  return $block;
}