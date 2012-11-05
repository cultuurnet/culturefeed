<?php

function culturefeed_ui_block_recommendations_is_visible() {
  if (DrupalCultureFeed::isCultureFeedUser()) {
    return TRUE;
  }

  return FALSE;
}

function culturefeed_ui_block_recommendations_cache_id() {
  return array(DrupalCultureFeed::isCultureFeedUser() ? 1 : 0);
}

function culturefeed_ui_block_recommendations_view() {
  $config = array(
    'title' => 'Aanbevolen voor jou',
    'id' => 'recommendations',
    'path' => 'culturefeed/recommendations/json',
  );

  return culturefeed_ui_block_ajaxload($config);
}

function culturefeed_ui_block_user_liked_locations_is_visible() {
  return culturefeed_ui_is_culturefeed_profile_page();
}

function culturefeed_ui_block_user_liked_locations_view() {
  $account = menu_get_object('user');

  if (!$account->culturefeed_uid) {
    return;
  }

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

  // Search what top events are liked.
  $user_likes = array();
  $user_goes = array();
  if (DrupalCultureFeed::isCultureFeedUser()) {
    try {

      $user_likes = DrupalCultureFeed::userDidActivity(CultureFeed_Activity::TYPE_LIKE, implode(',', $events), CultureFeed_Activity::CONTENT_TYPE_EVENT);

      $query = new CultureFeed_SearchActivitiesQuery();
      $query->type = array(CultureFeed_Activity::TYPE_LIKE, CultureFeed_Activity::TYPE_IK_GA,);
      $query->nodeId = implode(',', $events);
      $query->contentType = CultureFeed_Activity::CONTENT_TYPE_EVENT;
      $query->userId = DrupalCultureFeed::getLoggedInUserId();
      $query->private = TRUE;

      $activities = DrupalCultureFeed::searchActivities($query);

      foreach ($activities->objects as $activity) {
        if ($activity->type == CultureFeed_Activity::TYPE_LIKE) {
          $user_goes[$activity->nodeId] = TRUE;
        }
        else {
          $user_likes[$activity->nodeId] = TRUE;
        }
      }

    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_ui', $e);
    }
  }

  $items = array();

  foreach ($events as $cdbid) {

    if (!isset($events_info[$cdbid])) {
      continue;
    }

    $event = $events_info[$cdbid];

    $anonymous = '';
    $disable = array();
    if (user_is_anonymous()) {
      $url = 'culturefeed/do/' . CultureFeed_Activity::TYPE_LIKE . '/' . CultureFeed_Activity::CONTENT_TYPE_EVENT . '/' . $cdbid;
      $anonymous = theme('culturefeed_ui_connect_hover', array('url' => $url));
      $disable = array('disable');
    }

    if (!isset($user_likes[$cdbid])) {
      $url = 'culturefeed/do/' . CultureFeed_Activity::TYPE_LIKE . '/' . CultureFeed_Activity::CONTENT_TYPE_EVENT . '/' . $cdbid;
      $title = 'Vind ik leuk';
      $options = array('attributes' => array('class' => array_merge(array('like-link'), $disable)));
      $link = l($title, $url, $options) . $anonymous;
    }
    else {
      $url = 'culturefeed/undo/' . CultureFeed_Activity::TYPE_LIKE . '/' . CultureFeed_Activity::CONTENT_TYPE_EVENT . '/' . $cdbid;
      $title = 'Niet meer';
      $options = array('attributes' => array('class' => array('unlike-link'), 'title' => 'Vind ik niet meer leuk'));
      $link = l($title, $url, $options);
    }

    $items[] = theme('culturefeed_ui_top_event', array('event' => $event, 'rate_link' => $link));

  }

  if (empty($items)) {
    return;
  }

  $block['subject'] = 'Top events';
  $block['content'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
    '#type' => 'ol',
    '#attributes' => array('class' => 'event-teaser-list'),
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

  if (!$account->culturefeed_uid) {
    return;
  }

  $query = new CultureFeed_SearchActivitiesQuery();
  $query->max = CULTUREFEED_UI_BLOCK_USER_ACTIVITIES_COUNT;
  $query->type = array(
    CultureFeed_Activity::TYPE_LIKE,
    CultureFeed_Activity::TYPE_FACEBOOK,
    CultureFeed_Activity::TYPE_TWITTER,
    CultureFeed_Activity::TYPE_IK_GA,
  );
  $query->userId = $account->culturefeed_uid;
  $query->private = FALSE;

  try {
    $activities = DrupalCultureFeed::searchActivities($query);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    return;
  }

  if (empty($activities->objects)) {
    $content['activities'] = array(
      '#markup' => 'Deze gebruiker heeft nog geen acties gedaan.',
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
        '#markup' => l('Beheer jouw acties', 'culturefeed/activities', array('attributes' => array('class' => array('manage-activities-link')))),
      );
    }

    $facebook = _culturefeed_ui_facebook_privacy_toggle();
    if ($facebook) {
      $item_list = array(
        '#theme' => 'item_list',
        '#items' => array(
          array(
            'data' => $facebook,
            'id' => 'onlineaccount-facebook',
          ),
        ),
      );
      $content['facebook'] = array(
        '#markup' => drupal_render($item_list),
      );
    }

    $facebook = _culturefeed_ui_facebook_privacy_toggle();
    if ($facebook) {
      $item_list = array(
        '#theme' => 'item_list',
        '#items' => array(
          array(
            'data' => $facebook,
            'id' => 'onlineaccount-facebook',
          ),
        ),
      );
      $content['facebook'] = array(
        '#markup' => drupal_render($item_list),
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
    '#markup' => l('Meer acties', 'culturefeed/activities/recent'),
  );

  $block['subject'] = 'Recente acties';
  $block['content'] = $content;

  return $block;
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
  $request = cnapi_ui_get_active_request();

  $cdbid = $request['event'];

  $config = array(
    'title' => 'Ontdek ook',
    'id' => 'related-events',
    'path' => 'culturefeed/relatedevents/' . $cdbid . '/json',
  );

  return culturefeed_ui_block_ajaxload($config);
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
    'all_link_title'      => 'Toon iedereen die dit leuk vindt',
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
    'do_link_title'       => 'Ik ga hier naartoe',
    'do_first_link_title' => 'Ik ga hier naartoe',
    'all_link_title'      => 'Toon iedereen die hier naartoe gaat',
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
      '#markup' => '<div class="more-link">' . l($config['all_link_title'], 'culturefeed/users/activity/' . $activity_type . '/' . $activity_content_type . '/' . $node_id, array('attributes' => array('class' => 'all-link'))) . '</div>',
    );
  }

  $user_did_this = FALSE;

  if (DrupalCultureFeed::isCultureFeedUser()) {
    try {
      $user_did_this = DrupalCultureFeed::userDidActivity($activity_type, $node_id, $activity_content_type);
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_ui', $e);
    }
  }

  $items = array();

  if (!$user_did_this) {
    $url = 'culturefeed/do/' . $activity_type . '/' . $activity_content_type . '/' . $node_id;
    if ($users->total == 0) {
      $title = $config['do_first_link_title'];
    }
    else {
      $title = $config['do_link_title'];
    }
    $content['link'] = array(
      '#markup' => '<div class="do-link-wrapper">' . l($title, $url, array('attributes' => array('class' => 'do-link ' . 'do-' . $config['activity_type'] . '-link'))) . '</div>',
    );
  }
  else {
    $url = 'culturefeed/undo/' . $activity_type . '/' . $activity_content_type . '/' . $node_id;
    $title = $config['undo_link_title'];
    $content['link'] = array(
      '#markup' => '<div class="undo-link-wrapper">' . l($title, $url, array('attributes' => array('class' => 'undo-link ' . 'undo-' . $config['activity_type'] . '-link'))) . '</div>',
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
  $block['subject'] = 'Teveel te doen?';
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

  drupal_alter('culturefeed_ui_profile_menu', $menu);

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

    $block['subject'] = 'Mijn UiTiD';
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
  $block['subject'] = 'Echt iets voor jou?';
  $block['content'] = theme('culturefeed_ui_something_for_you');
  return $block;
}

function culturefeed_ui_block_something_for_you_is_visible() {
  return user_is_anonymous();
}
