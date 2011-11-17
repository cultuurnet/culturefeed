<?php

function culturefeed_ui_block_recommendations_view() {
  if (!DrupalCultureFeed::isCultureFeedUser()) {
    return;
  }
  
  $content = array();
  
  $recommendations = culturefeed_ui_recommendations_list(CULTUREFEED_UI_BLOCK_RECOMMENDATIONS_COUNT);
  
  if ($recommendations === FALSE) {
    return;
  }
  
  if (empty($recommendations)) {
    $content['recommendations'] = array(
      '#markup' => 'Er zijn geen aanbevelingen beschikbaar.'
    );
  }
  else {
    $content['recommendations'] = array(
      '#theme' => 'item_list',
      '#items' => $recommendations,
      '#attributes' => array('class' => 'event-recommendation-list')
    );
    $content['more'] = array(
      '#markup' => l('Meer aanbevelingen', 'culturefeed/recommendations'),
    );
  }

  $block['subject'] = 'Aanbevolen voor jou';
  $block['content'] = $content;

  return $block;
}

function culturefeed_ui_block_user_liked_locations_is_visible() {
  return culturefeed_ui_is_culturefeed_profile_page();
}

function culturefeed_ui_block_user_liked_locations_view() {
  $account = menu_get_object('user');

  $query = new CultureFeed_SearchActivitiesQuery();
  $query->contentType = CultureFeed_Activity::CONTENT_TYPE_ACTOR;
  $query->type        = CultureFeed_Activity::TYPE_LIKE;
  $query->userId      = $account->culturefeed_uid;

  try {
    $activities = DrupalCultureFeed::searchActivities($query);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    return;
  }

  if (empty($activities->objects)) {
    $content['locations'] = array(
      '#markup' => 'Deze gebruiker vond nog geen locaties leuk.',
    );
  }
  else {
    $actors = culturefeed_get_nodes_for_activities($activities->objects);
    
    $items = array();
    
    foreach ($actors as $actor) {
      // @todo fix this : actor can be unavailable : show title then.
      $items[] = theme('cnapi_ui_actor_mini_summary', array('actor' => $actor));
    }
    
    $content['locations'] = array(
      '#theme' => 'item_list',
      '#items' => $items,
    );
  }

  $block['subject'] = 'Favoriete locaties';
  $block['content'] = $content;

  return $block;
}

function culturefeed_ui_block_top_events_cache_id() {
  return array();
}

function culturefeed_ui_block_top_events_cache_expires() {
  return $_SERVER['REQUEST_TIME'] + variable_get('culturefeed_ui_top_events_block_cache_expires', 3600);
}

function culturefeed_ui_block_top_events_view() {
  $sort = variable_get('culturefeed_ui_block_top_events_sort', CultureFeed::TOP_EVENTS_SORT_ACTIVE);

  try {
    $events = DrupalCultureFeed::getTopEvents($sort, CULTUREFEED_UI_BLOCK_TOP_EVENTS_COUNT);
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
  
  if (empty($items)) {
    return;
  }

  $block['subject'] = 'Top events';
  $block['content'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
    '#type' => 'ol',
  );

  return $block;
}

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

  $block['subject'] = 'Gelijkaardige gebruikers';
  $block['content'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
    '#attributes' => array('class' => 'user-list'),
  );

  return $block;
}

function culturefeed_ui_block_user_activities_is_visible() {
  return culturefeed_ui_is_culturefeed_profile_page();
}

function culturefeed_ui_block_user_activities_view() {
  $account = menu_get_object('user');

  $query = new CultureFeed_SearchActivitiesQuery();
  $query->max = CULTUREFEED_UI_BLOCK_USER_ACTIVITIES_COUNT;
  $query->type = array(
    CultureFeed_Activity::TYPE_LIKE,
    CultureFeed_Activity::TYPE_FACEBOOK,
    CultureFeed_Activity::TYPE_TWITTER,
    CultureFeed_Activity::TYPE_IK_GA,
  );
  $query->userId = $account->culturefeed_uid;

  try {
    $activities = DrupalCultureFeed::searchActivities($query);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    return;
  }

  if (empty($activities->objects)) {
    $content['activities'] = array(
      '#markup' => 'Deze gebruiker heeft nog geen activiteiten gedaan.',
    );
  }
  else {
    $items = culturefeed_ui_activities_list($activities);
    $content['activities'] = array(
      '#theme' => 'item_list',
      '#items' => $items,
      '#attributes' => array('class' => 'user-activity-list'),
    );
    if ($account->culturefeed_uid == DrupalCultureFeed::getLoggedInUserId()) {
      $content['edit'] = array(
        '#markup' => l('Beheer jouw acties', 'culturefeed/activities'),
      );
    }
  }

  $block['subject'] = 'Recente acties';
  $block['content'] = $content;

  return $block;
}

function culturefeed_ui_block_recent_activities_view() {
  $query = new CultureFeed_SearchActivitiesQuery();
  $query->max = CULTUREFEED_UI_BLOCK_RECENT_ACTIVITIES_COUNT;
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
  
  $content['activities'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
    '#attributes' => array('class' => 'user-activity-list'),
  );
  
  $content['more'] = array(
    '#markup' => l('Oudere acties', 'culturefeed/activities/recent'),
  );

  $block['subject'] = 'Recente acties';
  $block['content'] = $content;

  return $block;
}

function culturefeed_ui_block_related_events_cache_expires() {
  return $_SERVER['REQUEST_TIME'] + variable_get('culturefeed_ui_related_events_block_cache_expires', 86400);
}

function culturefeed_ui_block_related_events_cache_id() {
  $request = cnapi_ui_get_active_request();

  return array($request['event']);
}

function culturefeed_ui_block_related_events_is_visible() {
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

function culturefeed_ui_block_related_events_view() {
  // Get the event and CDBID.
  $request = cnapi_ui_get_active_request();
  
  $cdbid = $request['event'];

  $event = cnapi_get_event($cdbid);
  
  // Get the cityid for the event.
  $cityid = $event['location']['address']['cityid'];

  // Get the headings for the event.
  $headings = cnapi_get_headings();

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

  // Get recommendations based on the event.
  $query = new CultureFeed_RecommendationsQuery();
  $query->max = CULTUREFEED_UI_BLOCK_RELATED_EVENTS_COUNT;

  try {
    $recommendations = DrupalCultureFeed::getRecommendationsForEvent($cdbid, $query);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    return;
  }

  // If we got no recommendations, we don't bother showing the block at all.
  if (empty($recommendations)) {
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

function culturefeed_ui_block_users_likes_config() {
  $config = array(
    'node_types'          => array_keys(node_type_get_types()),
    'cnapi_types'         => array('actor', 'event', 'production'),
    'activity_type'       => CultureFeed_Activity::TYPE_LIKE,
    'block_subject'       => 'Vinden dit leuk',
    'undo_link_title'     => 'Vind ik niet meer leuk',
    'do_link_title'       => 'Vind ik leuk',
    'do_first_link_title' => 'Vind dit als eerste leuk',
    'all_link_title'      => 'Toon iedereen die dit leuk vindt.',
  );

  return $config;
}

function culturefeed_ui_block_users_likes_is_visible() {
  $config = culturefeed_ui_block_users_likes_config();
  
  return _culturefeed_ui_block_users_activity_is_visible($config);
}

function culturefeed_ui_block_users_likes_view() {
  $config = culturefeed_ui_block_users_likes_config();

  return _culturefeed_ui_block_users_activity_view($config);
}

function culturefeed_ui_block_users_attends_config() {
  $config = array(
    'node_types'          => array(),
    'cnapi_types'         => array('event', 'production'),
    'activity_type'       => CultureFeed_Activity::TYPE_IK_GA,
    'block_subject'       => 'Gaan hier naartoe',
    'undo_link_title'     => 'Ik ga hier niet meer naartoe',
    'do_link_title'       => 'Ga ik naartoe',
    'do_first_link_title' => 'Ga hier als eerste naartoe',
    'all_link_title'      => 'Toon iedereen die hier naartoe gaat.',
  );
  
  return $config;
}

function culturefeed_ui_block_users_attends_is_visible() {
  $config = culturefeed_ui_block_users_attends_config();
  
  return _culturefeed_ui_block_users_activity_is_visible($config);
}

function culturefeed_ui_block_users_attends_view() {
  $config = culturefeed_ui_block_users_attends_config();

  return _culturefeed_ui_block_users_activity_view($config);
}

function _culturefeed_ui_block_users_activity_is_visible($config) {
  $context = culturefeed_ui_activity_context($config['cnapi_types'], $config['node_types']);

  return $context !== FALSE;
}

// @todo clean up code
function _culturefeed_ui_block_users_activity_view($config) {
  $max = CULTUREFEED_UI_BLOCK_USERS_ACTIVITY_COUNT;

  $context = culturefeed_ui_activity_context($config['cnapi_types'], $config['node_types']);
  extract($context);
  
  $activity_type = $config['activity_type'];

  try {
    $users = DrupalCultureFeed::searchActivityUsers($node_id, $activity_type, $activity_content_type, 0, $max);
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
      '#markup' => l($config['all_link_title'], 'culturefeed/users/activity/' . $activity_type . '/' . $activity_content_type . '/' . $node_id, array('attributes' => array('class' => 'all-link'))),
    );
  }

  $user_likes_this = FALSE;

  if (DrupalCultureFeed::isCultureFeedUser()) {
    try {
      $user_likes_this = DrupalCultureFeed::userDidActivity($activity_type, $node_id, $type);
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_ui', $e);
    }
  }

  $items = array();

  if (!$user_likes_this) {
    $url = 'culturefeed/do/' . $activity_type . '/' . $activity_content_type . '/' . $node_id;
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
    $url = 'culturefeed/undo/' . $activity_type . '/' . $activity_content_type . '/' . $node_id;
    $title = $config['undo_link_title'];
    $content['link'] = array(
      '#markup' => l($title, $url, array('attributes' => array('class' => 'undo-link')))
    );
  }

  $block['subject'] = $config['block_subject'];
  $block['content'] = $content;

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

  $block['subject'] = 'UiTslover van de week';
  $block['content'] = array(
    '#theme' => 'culturefeed_ui_most_active_user',
    '#account' => $cf_account,
    '#sort' => $sort,
  );

  return $block;
}

function culturefeed_ui_block_search_users_view() {
  $block['subject'] = 'Zoek andere gebruikers';
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
  $block['subject'] = 'Maak zelf een UiTiD';
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

function culturefeed_ui_block_profile_menu_is_visible() {
  return DrupalCultureFeed::isCultureFeedUser();
}

function culturefeed_ui_block_profile_menu_view() {
	global $user;

  $menu = array(
    array(
      'title' => 'Mijn UiTiD',
      'url' => 'user/' . $user->uid,
      'description' => 'Zo zien anderen jouw profiel.',
    ),
    array(
      'title' => 'Profiel bewerken',
      'url' => 'culturefeed/profile/edit',
      'description' => 'Wijzig naam, adres, favoriete locaties, ...',
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
      'description' => 'Stel in welke acties (Leuk, Deel, Tweet, ...) voor anderen zichtbaar zijn in je UiTiD-profiel.',
    ),
  );

  $items = array();

  foreach ($menu as $menu_item) {
    $vars = array(
      'title' => $menu_item['title'], 
      'url' => $menu_item['url'], 
      'description' => $menu_item['description']
    );
    
    $data = theme('culturefeed_ui_profile_menu_item', $vars);
    
    $class = $_GET['q'] == $menu_item['url'] ? 'active' : '';
    
    $items[] = array(
      'data' => $data,
      'class' => $class,
    );
  }

  $block['subject'] = 'Beheer je profiel';
  $block['content'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
  );

  return $block;
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

  $block['subject'] = 'Bevestig jouw e-mailadres';
  $block['content'] = theme('culturefeed_ui_mbox_confirmation_reminder', array('account' => $cf_account));

  return $block;
}