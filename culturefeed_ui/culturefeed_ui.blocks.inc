<?php

function culturefeed_ui_block_view_user_liked_locations() {
  global $user;

  $account = menu_get_object('user');

  if (!$account || !DrupalCultureFeed::isCultureFeedUser($account->uid)) {
    return;
  }

  $query = new CultureFeed_SearchActivitiesQuery();
  $query->contentType = CultureFeed_Activity::CONTENT_TYPE_ACTOR;
  $query->type = array(
    CultureFeed_Activity::TYPE_LIKE,
  );
  $query->userId = $account->culturefeed_uid;
  if ($user->uid == $account->uid) {
    $query->private = ($account->uid == $user->uid);
  }

  try {
    $activities = DrupalCultureFeed::searchActivities($query);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    return;
  }
  
  if (empty($activities->objects)) {
    return;
  }

  $actors = culturefeed_get_nodes_for_activities($activities->objects);
  
  $items = array();
  
  foreach ($actors as $actor) {
    $items[] = theme('cnapi_ui_actor_mini_summary', array('actor' => $actor));
  }
  
  if (empty($items)) {
    return;
  }

  $block['subject'] = 'Favoriete locaties';
  $block['content'] = array(
    array(
      '#theme' => 'item_list',
      '#items' => $items,
    ),
  );

  return $block;
}

function culturefeed_ui_block_view_actions() {
  $node = menu_get_object();
  $request = cnapi_ui_get_active_request();

  // We only act upon cnapi requests.
  if (!$request && !$node) {
    return;
  }

  if ($request) {
    $type = reset(array_intersect(array('actor', 'event', 'production'), array_keys($request)));
    if (!$type) {
      return;
    }
  }
  else {
    $type = 'page';
  }
  
  $id = $request ? $request[$type] : $node->nid;
  
  $user_likes_this = FALSE;
  $user_goes_to = FALSE;
  
  if (DrupalCultureFeed::isCultureFeedUser()) {
    $query = new CultureFeed_SearchActivitiesQuery();
    $query->type = array(
      CultureFeed_Activity::TYPE_LIKE,
      CultureFeed_Activity::TYPE_IK_GA,
    );
    $query->nodeId = $id;
    $query->contentType = culturefeed_activity_content_type_from_cnapi_type($type);
    $query->userId = DrupalCultureFeed::getLoggedInAccount()->culturefeed_uid;
    $query->private = TRUE;
    
    try {
      $activities = DrupalCultureFeed::searchActivities($query);
      if (!empty($activities->objects)) {
        foreach ($activities->objects as $activity) {
          if ($activity->type == CultureFeed_Activity::TYPE_LIKE) {
            $user_likes_this = TRUE;
          }
          if ($activity->type == CultureFeed_Activity::TYPE_IK_GA) {
            $user_goes_to = TRUE;
          }
        }
      }
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_ui', $e);
      return;
    }
  }
  
  $items = array();
    
  if (!$user_likes_this) {
    $items[] = l('Vind ik leuk', 'culturefeed/do/' . CultureFeed_Activity::TYPE_LIKE . '/' . $type . '/' . $id);
  }
  else {
    $items[] = l('Vind ik niet meer leuk', 'culturefeed/undo/' . CultureFeed_Activity::TYPE_LIKE . '/' . $type . '/' . $id);
  }
    
  if (!$user_goes_to) {
    $items[] = l('Ik ga hier naartoe', 'culturefeed/do/' . CultureFeed_Activity::TYPE_IK_GA . '/' . $type . '/' . $id);
  }
  else {
    $items[] = l('Ik ga hier niet meer naartoe', 'culturefeed/undo/' . CultureFeed_Activity::TYPE_IK_GA . '/' . $type . '/' . $id);
  }
  
  $items[] = l('Mail', 'forward', array('query' => array('path' => 'node/' . $node->nid)));
  
  $block['subject'] = t('Actions');
  $block['content'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
  );

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
  $block['subject'] = 'Zoek andere gebruikers';
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
  $block['subject'] = 'Top events';
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

  $block['subject'] = 'Gelijkaardige gebruikers';
  $block['content'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
  );

  return $block;
}

function culturefeed_ui_block_view_profile_menu() {
	global $user;
	
  if (!DrupalCultureFeed::isCultureFeedUser()) {
    return;
  }
  
  $menu = array(
    array(
      'title' => 'Mijn UiTid',
      'url' => 'user/' . $user->uid,
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

  $block['subject'] = 'Beheer je profiel';
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

  $block['subject'] = 'Recente activiteiten';
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
  $node = menu_get_object();
  $request = cnapi_ui_get_active_request();

  // We only act upon cnapi requests.
  if (!$request && !$node) {
    return;
  }

  if ($request) {
    $type = reset(array_intersect(array('actor', 'event', 'production'), array_keys($request)));
    if (!$type) {
      return;
    }
  }
  else {
    $type = 'page';
  }
  
  $id = $request ? $request[$type] : $node->nid;

  $activity_content_type = culturefeed_activity_content_type_from_cnapi_type($type);

  try {
    $users = DrupalCultureFeed::searchActivityUsers($id, CultureFeed_Activity::TYPE_LIKE, $activity_content_type);
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
      '#markup' => l('Bekijk iedereen', 'culturefeed/users/likes/event/' . $id),
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

  $activity_content_type = culturefeed_activity_content_type_from_cnapi_type($type);

  try {
    $users = DrupalCultureFeed::searchActivityUsers($request[$type], CultureFeed_Activity::TYPE_IK_GA, $activity_content_type);
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
  
  $cityid = $event['location']['address']['cityid'];
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
    '#cityid' => $cityid,
    '#headings' => $valid_headings, 
  );

  return $block;
}