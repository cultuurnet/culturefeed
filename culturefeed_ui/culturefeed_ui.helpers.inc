<?php

/**
 * Put noindex and canonical metatags for SEO on relevant culturefeed pages.
 */
function culturefeed_ui_set_canonical_and_noindex() {
  $canonical = FALSE;
  $noindex = FALSE;
  
  if (arg(0) == 'culturefeed') {
    if (arg(1) == 'activities' && arg(2) == 'recent') {
      $noindex = TRUE;
    }
    if (arg(1) == 'users' && arg(2) == 'search') {
      $noindex = TRUE;
    }
  }

  // Add the "noindex, follow" meta tag if needed.
  if ($noindex) {
    $element = array(
      '#tag' => 'meta',
      '#attributes' => array(
        'name' => 'robots',
        'content' => 'noindex, follow',
      ),
    );

    drupal_add_html_head($element, 'nofollow');
  }

  // Add the rel="canonical" tag if needed.
  if ($canonical) {
    $canonical['options']['absolute'] = TRUE;
    drupal_add_html_head_link(array('rel' => 'canonical', 'href' => url($canonical['path'], $canonical['options'])), TRUE);
  }
}

function culturefeed_ui_block_ajaxload($config) {
  $block['subject'] = $config['title'];
  $block['content'] = array(
    '#markup' => '<div id="' . $config['id'] . '">Bezig met laden...</div>',
    '#id' => $config['id'],
    '#ajax' => array(
      'event' => 'ajaxload',
      'path' => $config['path'],
    ),
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'culturefeed_ui') . '/js/culturefeed_ui.js',
        array('data' => array(), 'type' => 'setting'),
      ),
    ),
  );
  
  $block['content'] = ajax_pre_render_element($block['content']);

  return $block;
}

function culturefeed_ui_is_culturefeed_profile_page() {
  $account = menu_get_object('user');

  if (arg(0) == 'user' && is_numeric(arg(1)) && !arg(2) && DrupalCultureFeed::isCultureFeedUser($account->uid)) {
    return TRUE;
  }

  return FALSE;
}

function culturefeed_ui_online_account($online_account_type, $online_account = NULL) {
  $delete_link = '';
  
  if ($online_account) {
    $delete_link = array(
      '#type' => 'link',
      '#title' => 'Verwijderen',
      '#href' => 'culturefeed/onlineaccounts/delete/' . $online_account_type . '/' . $online_account->accountName . '/nojs',
      '#attributes' => array('class' => 'delete-link'),
      '#options' => array('query' => drupal_get_destination()),
      '#ajax' => array(),
    );
    
    $delete_link = drupal_render($delete_link);
  }
  
  $statement = 'Ik geef toestemming aan UiTiD om mijn acties op ' . variable_get('site_name', 'Drupal') .  ' automatisch te publiceren op ' . $online_account_type . '. ' . l('Gebruiksvoorwaarden', 'node/2512');
  
  $publish_link = '';
  if ($online_account) {
    $publish_link = array(
      '#type' => 'link',
      '#title' => $online_account->publishActivities ? 'Publiek' : 'Privé',
      '#href' => 'culturefeed/onlineaccounts/privacy/' . $online_account_type . '/' . $online_account->accountName . '/' . ($online_account->publishActivities ? '0' : '1')  . '/nojs',
      '#attributes' => array('id' => 'onlineaccount-privacy-' . $online_account->accountName, 'class' => 'privacy-link'),
      '#options' => array('query' => drupal_get_destination()),
      '#ajax' => array(),
    );
    
    $publish_link = drupal_render($publish_link) . $statement;
  }
    
  $redirect = url('culturefeed/account/edit', array('absolute' => TRUE, 'query' => array('closepopup' => 'true')));
  $connect_link = l('Account toevoegen', DrupalCultureFeed::getUrlAddSocialNetwork($online_account_type, $redirect), array('attributes' => array('class' => 'culturefeedconnect')));
    
  $vars = array(
    'account_type'     => $online_account_type,
    'account'          => $online_account,
    'publish_link'     => $publish_link,
    'delete_link'      => $delete_link,
    'connect_link'     => $connect_link,
  );
  
  return theme('culturefeed_ui_online_account', $vars);
}

function culturefeed_ui_activity_types() {
  return array(
    CultureFeed_Activity::TYPE_LIKE     => 'Vind ik leuk',
    CultureFeed_Activity::TYPE_IK_GA    => 'Ga hier naartoe',
    CultureFeed_Activity::TYPE_FACEBOOK => 'Gedeeld op Facebook',
    CultureFeed_Activity::TYPE_TWITTER  => 'Gedeeld op Twitter',
  );
}

function culturefeed_ui_activity_context($cnapi_types = array(), $node_types = array()) {
  $node = menu_get_object();
  $request = cnapi_ui_get_active_request();

  if ($request) {
    $type = reset(array_intersect($cnapi_types, array_keys($request)));
    if ($type) {
      return array(
        'node_id' => $request[$type],
        'activity_content_type' => culturefeed_activity_content_type_from_cnapi_type($type),
      );
    }
  }
  elseif ($node) {
    if (in_array($node->type, $node_types)) {
      return array(
        'node_id' => $node->nid,
        'activity_content_type' => culturefeed_activity_content_type_from_cnapi_type($node->type),
      );
    }
  }
  
  return FALSE;
}

function culturefeed_ui_recommendations_list($total, $zip = NULL) {
  $query = new CultureFeed_RecommendationsQuery();
  $query->max = $total;

  if ($zip) {
    $query->zipcode = $zip;
  }

  try {
    global $conf;
    $conf['culturefeed_cache_status'] = 0;
    $recommendations = DrupalCultureFeed::getRecommendationsForUser(DrupalCultureFeed::getLoggedInUserId(), $query);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    return FALSE;
  }

  $items = array();

  foreach ($recommendations as $recommendation) {
    $recommendation_id = $recommendation->id;
    $node_id = $recommendation->itemid;

    $positive = array(
      '#type' => 'link',
      '#title' => 'Vind ik leuk',
      '#href' => 'culturefeed/recommendations/positive/' . $recommendation_id . '/' . $node_id . '/nojs',
      '#attributes' => array('class' => 'like-link'),
      '#options' => array('query' => drupal_get_destination()),
      '#ajax' => array(
        'wrapper' => 'recommendation-' . $recommendation_id,
      ),
    );

    $negative = array(
      '#type' => 'link',
      '#title' => 'Verwijderen',
      '#href' => 'culturefeed/recommendations/negative/' . $recommendation_id . '/' . $node_id . '/nojs',
      '#attributes' => array('class' => 'remove-link'),
      '#options' => array('query' => drupal_get_destination()),
      '#ajax' => array(
        'wrapper' => 'recommendation-' . $recommendation_id,
      ),
    );

    $links = array('like' => drupal_render($positive), 'remove' => drupal_render($negative));
    $item = theme('culturefeed_ui_event_recommendation', array('event' => $recommendation, 'links' => $links, 'heading_level' => 3));

    $items[] = array(
      'data' => $item,
      'id' => 'recommendation-' . $recommendation_id,
    );
  }

  return $items;
}

function _culturefeed_ui_undo($activity_type, $content_type, $node_id) {
  $user_id = DrupalCultureFeed::getLoggedInUserId();
  $content_type = culturefeed_activity_content_type_from_cnapi_type($content_type);

  try {
    DrupalCultureFeed::deleteActivities($user_id, $node_id, $content_type, $activity_type);
    
    return TRUE;
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed_ui', $e);
    
    return FALSE;
  }
}

function _culturefeed_ui_do($activity_type, $type, $id) {
  $activity = new CultureFeed_Activity();
  $activity->contentType = culturefeed_activity_content_type_from_cnapi_type($type);
  $activity->type = $activity_type;
  $activity->nodeId = $id;
  $activity->userId = DrupalCultureFeed::getLoggedInUserId();

  if ($type == 'page') {
    $node = node_load($id);
    $activity->nodeTitle = $node->title;
  }
  else {
    $object = cnapi_get(array('action' => 'detail', 'type' => $type, 'query' => array('cdbid' => $id)));
    $activity->nodeTitle = $object['detail']['nl']['title'];
  }

  try {
    DrupalCultureFeed::createActivity($activity);
    
    return TRUE;
  }
  catch (Exception $e) {
    if ($e->getCode() != 421) {
      watchdog_exception('culturefeed_ui', $e);
      return FALSE;
    }
    return TRUE;
  }
}

function culturefeed_ui_activity_based_goto($content_type, $node_id) {
  if ($content_type == 'page') {
    drupal_goto('node/' . $node_id);
  }
  else {
    $object = cnapi_get(array('action' => 'detail', 'type' => $content_type, 'query' => array('cdbid' => $node_id)));

    $request = array($content_type => $node_id, 'title' => $object['detail']['nl']['title']);

    cnapi_ui_goto($request);
  }
}

function culturefeed_ui_activities_list($activities) {
  $items = array();

  $accounts = culturefeed_get_uids_for_activities($activities->objects);

  foreach ($activities->objects as $activity) {
    if (!isset($accounts[$activity->userId])) {
      continue;
    }

    $items[] = theme('culturefeed_ui_activity_summary', array('activity' => $activity, 'uid' => $accounts[$activity->userId]));
  }

  return $items;
}

function culturefeed_ui_users_list($users) {
  $items = array();

  $accounts = culturefeed_get_uids_for_users($users->objects);

  foreach ($users->objects as $user) {
    if (!isset($accounts[$user->id])) {
      continue;
    }

    $items[] = theme('culturefeed_ui_user_summary', array('user' => $user, 'uid' => $accounts[$user->id]));
  }

  return $items;
}