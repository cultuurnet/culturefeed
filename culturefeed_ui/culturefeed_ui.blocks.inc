<?php

function culturefeed_ui_block_view_recommendations() {
  if (!DrupalCultureFeed::isCultureFeedUser()) {
    return;
  }

  $block['subject'] = 'Aanbevolen voor jou';
  $block['content'] = array(
    'recommendations' => culturefeed_ui_recommendations_list(5),
    'more' => array('#markup' => l('Meer aanbevelingen', 'culturefeed/recommendations')),
  );

  return $block;
}

function culturefeed_ui_block_is_visible_user_liked_locations() {
  $account = menu_get_object('user');

  if (arg(0) == 'user' && is_numeric(arg(1)) && !arg(2) && DrupalCultureFeed::isCultureFeedUser($account->uid)) {
    return TRUE;
  }

  return FALSE;
}

function culturefeed_ui_block_view_user_liked_locations() {
  global $user;

  $account = menu_get_object('user');

  $query = new CultureFeed_SearchActivitiesQuery();
  $query->contentType = CultureFeed_Activity::CONTENT_TYPE_ACTOR;
  $query->type = CultureFeed_Activity::TYPE_LIKE;
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

function culturefeed_ui_block_view_mbox_confirmation_reminder() {
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

  $block['subject'] = 'Bevestig jouw e-mailadres';
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

  if (DrupalCultureFeed::isCultureFeedUser()) {
    try {
      $cf_account = DrupalCultureFeed::getLoggedInUser();
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_ui', $e);
      return;
    }
  }

  $block['subject'] = '';
  $block['content'] = array(
    '#theme' => 'culturefeed_ui_profile_box',
    '#account' => isset($cf_account) ? $cf_account : FALSE,
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

  if (arg(0) == 'user' && is_numeric(arg(1)) && !arg(2) && DrupalCultureFeed::isCultureFeedUser($account->uid)) {
    return TRUE;
  }

  return FALSE;
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
    '#attributes' => array('class' => 'user-list'),
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
    array(
      'title' => 'Acties',
      'url' => 'culturefeed/activities',
      'description' => 'Stel in welke acties (Leuk, Deel, Tweet, ...) voor anderen zichtbaar zijn in je UiTid-profiel.',
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

function culturefeed_ui_block_is_visible_user_activities() {
  $account = menu_get_object('user');

  if (arg(0) == 'user' && is_numeric(arg(1)) && !arg(2) && DrupalCultureFeed::isCultureFeedUser($account->uid)) {
    return TRUE;
  }

  return FALSE;
}

function culturefeed_ui_block_view_user_activities() {
  global $user;

  $account = menu_get_object('user');

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

  $block['subject'] = 'Recente acties';
  $block['content'] = array(
    array(
      '#theme' => 'item_list',
      '#items' => $items,
      '#attributes' => array('class' => 'user-activity-list'),
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

  $block['subject'] = 'Recente acties';
  $block['content'] = array(
    array(
      '#theme' => 'item_list',
      '#items' => $items,
      '#attributes' => array('class' => 'user-activity-list'),
    ),
    array(
      '#markup' => l('Oudere acties', 'culturefeed/activities/recent'),
    ),
  );

  return $block;
}

function culturefeed_ui_block_view_users_likes() {
  $config['node_types'] = array_keys(node_type_get_types());
  $config['cnapi_types'] = array('actor', 'event', 'production');
  $config['activity_type'] = CultureFeed_Activity::TYPE_LIKE;
  $config['block_subject'] = 'Vinden dit leuk';
  $config['undo_link_title'] = 'Vind ik niet meer leuk';
  $config['do_link_title'] = 'Vind ik leuk';
  $config['do_first_link_title'] = 'Vind dit als eerste leuk';
  $config['all_link_title'] = 'Toon iedereen die dit leuk vindt.';

  return _culturefeed_ui_block_view_users_activity($config);
}

function culturefeed_ui_block_view_users_attends() {
  $config['node_types'] = array();
  $config['cnapi_types'] = array('event', 'production');
  $config['activity_type'] = CultureFeed_Activity::TYPE_IK_GA;
  $config['block_subject'] = 'Gaan hier naartoe';
  $config['undo_link_title'] = 'Ik ga hier niet meer naartoe';
  $config['do_link_title'] = 'Ga ik naartoe';
  $config['do_first_link_title'] = 'Ga hier als eerste naartoe';
  $config['all_link_title'] = 'Toon iedereen die hier naartoe gaat.';

  return _culturefeed_ui_block_view_users_activity($config);
}

function _culturefeed_ui_block_view_users_activity($config) {
  $max = 12;

  $node = menu_get_object();
  $request = cnapi_ui_get_active_request();

  if (!$request && !$node) {
    return;
  }

  if ($request) {
    $type = reset(array_intersect($config['cnapi_types'], array_keys($request)));
    if (!$type) {
      return;
    }
  }
  else {
    if (!in_array($node->type, $config['node_types'])) {
      return;
    }

    $type = 'page';
  }

  $id = $request ? $request[$type] : $node->nid;

  $activity_content_type = culturefeed_activity_content_type_from_cnapi_type($type);

  try {
    $users = DrupalCultureFeed::searchActivityUsers($id, $config['activity_type'], $activity_content_type, 0, $max);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    return;
  }

  $items = culturefeed_ui_users_list($users);

  $content['users'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
    '#attributes' => array('class' => 'user-list'),
  );

  if ($users->total > $max) {
    $content['more'] = array(
      '#markup' => l($config['all_link_title'], 'culturefeed/users/activity/' . $config['activity_type'] . '/event/' . $id, array('attributes' => array('class' => 'all-link'))),
    );
  }

  $user_likes_this = FALSE;

  if (DrupalCultureFeed::isCultureFeedUser()) {
    try {
      $user_likes_this = DrupalCultureFeed::userDidActivity($config['activity_type'], $id, $type);
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_ui', $e);
    }
  }

  $items = array();

  if (!$user_likes_this) {
    $url = 'culturefeed/do/' . $config['activity_type'] . '/' . $type . '/' . $id;
    if ($users->total == 0) {
      $title = $config['do_first_link_title'];
    }
    else {
      $title = $config['do_link_title'];
    }
    $content['link'] = array(
      '#markup' => l($title, $url, array('attributes' => array('class' => 'do-link')))
    );
  }
  else {
    $url = 'culturefeed/undo/' . $config['activity_type'] . '/' . $type . '/' . $id;
    $title = $config['undo_link_title'];
    $content['link'] = array(
      '#markup' => l($title, $url, array('attributes' => array('class' => 'undo-link')))
    );
  }

  $block['subject'] = $config['block_subject'];
  $block['content'] = $content;

  return $block;
}

function culturefeed_ui_block_cache_expires_related_events() {
  return $_SERVER['REQUEST_TIME'] + variable_get('culturefeed_ui_related_events_block_cache_expires', 86400);
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

  $block['subject'] = 'Ontdek ook';
  $block['content'] = array(
    '#theme' => 'culturefeed_ui_related_events',
    '#events' => $recommendations,
    '#cityid' => $cityid,
    '#headings' => $valid_headings,
  );

  return $block;
}