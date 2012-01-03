<?php

/**
 * Fetch a unique username for the given Culture Feed user id.
 */
function culturefeed_unique_username($cf_uid, $nick) {
  $name = db_query("SELECT DISTINCT name FROM {culturefeed_user} cfu INNER JOIN {users} u ON u.uid = cfu.uid WHERE cfu.cf_uid = :cf_uid", array(':cf_uid' => $cf_uid))->fetchField();

  if ($name) {
    return $name;
  }

  $name = $nick;

  $count = 0;
  while ($name_exists = db_query("SELECT name FROM {users} WHERE LOWER(name) = :name", array(':name' => strtolower($name)))->fetchField()) {
    $count++;
    $name = $name . '_' . $count;
  }

  return $name;
}

/**
 * Create a Drupal user account with a given username.
 *
 * @param string $name
 *   The username for the account.
 * @return stdClass
 *   The user account.
 */
function culturefeed_create_user($name) {
  $url = parse_url($GLOBALS['base_url']);
  $account = user_save(
    drupal_anonymous_user(),
    array(
      'name' => $name,
      'pass' => '', // We save an empty password (don't generate a random one) because this is much more performant.
      'init' => $name,
      'roles' => array(),
      'status' => 1,
      'timezone' => variable_get('date_default_timezone', date_default_timezone_get()),
      'access' => REQUEST_TIME,
      'mail' => '',
    )
  );

  return $account;
}

/**
 * Try to load a user account for a certain Culture Feed User ID. If it doesn't exist, create it and return it.
 *
 * @param integer $cf_uid
 *   To CultureFeed User ID of the user to load.
 * @param stdClass $cf_account
 *   (optional) The user on CultureFeed.
 *   If this is not passed and the user can not be found, a new account will not be created and FALSE will be returned.
 * @param string $token
 *   (optional) If a token is known at the moment of creation, pass it to save it.
 *   This can be used for users that log in for the first time.
 * @param string $secret
 *   (optional) If a token is known at the moment of creation, pass the secret for the token to save it.
 *   This can be used for users that log in for the first time.
 * @param string $application_key
 *   (optional) If a token is known at the moment of creation, pass the wanted application key the token is for to save it.
 *   If token and secret are passed, but no application key, then the default application key will be used.
 *   This can be used for users that log in for the first time.
 * @return stdClass|bool
 *   The Drupal user account will be returned in case of success.
 *   In case of failure FALSE will be returned.
 */
function culturefeed_get_or_create_user($cf_uid, $cf_account = NULL, $token = NULL, $secret = NULL, $application_key = NULL) {
  $account = NULL;
  global $user;

  // If no application key is passed, use the default one.
  if (!$application_key) {
    $application_key = variable_get('culturefeed_api_application_key', '');
  }

  // Use a database transaction, as we need to handle what follows as a unit of work
  $txn = db_transaction();

  try {
    // Check if the user is already known in our system.
    $uid = db_query("SELECT uid FROM {culturefeed_user} cfu WHERE cfu.cf_uid = :cf_uid", array(':cf_uid' => $cf_uid))->fetchField();

    // If the user is not known, create it.
    if (!$uid) {
      // If no CultureFeed user was passed, we can't create the user.
      if (!$cf_account) {
        return FALSE;
      }

      // If the current user in Drupal is logged in and is not connected with a CultureFeed user yet, then we connect both
      if ($user->uid && empty($user->culturefeed_uid)) {
        $account = $user;
      }
      else {
        // Find a unique nick based on the given nick. In case the nick is not used yet, the passed nick will be used. Else the nick will be suffixed with an underscore and counter (example nick_12).
        $unique_nick = culturefeed_unique_username($cf_uid, $cf_account->nick);

        // Create the user account.
        $account = culturefeed_create_user($unique_nick);
      }

      // Save the mapping between Culture Feed User ID and Drupal user id.
      db_insert('culturefeed_user')
        ->fields(array(
          'uid' => $account->uid,
          'cf_uid' => $cf_uid,
          ))
        ->execute();

      // Save the fact that this is an external account for culturefeed.
      user_set_authmaps($account, array('authname_culturefeed' => $account->name));
    }
    else {
      // In case the user exists, load its account.
      $account = user_load($uid);
    }

    // If a token was passed, save it after deleting a possible previous entry.
    if ($token) {
      db_delete('culturefeed_token')
        ->condition('cf_uid', $cf_uid)
        ->condition('application_key', $application_key)
        ->execute();

      db_insert('culturefeed_token')
        ->fields(array(
          'cf_uid' => $cf_uid,
          'token' => $token,
          'secret' => $secret,
          'application_key' => $application_key,
          ))
        ->execute();
    }
  }
  catch (Exception $e) {
    $txn->rollback();
    throw $e;
  }

  return $account;
}

function culturefeed_get_uid_for_cf_uid($cf_uid, $nick) {
  $user = new stdClass();
  $user->cf_uid = $cf_uid;
  $user->nick = $nick;

  return reset(culturefeed_get_uids_for_cf_uids(array($cf_uid => $user)));
}

/**
 * Get the Drupal user ids for a list of CultureFeed user ids.
 *
 * If a user account does not exist locally for a given user id, a new account is created for it.
 *
 * @param array $users
 * @return array
 *   An associative array where the keys are the passed CultureFeed user ids.
 *   The values are the matching Drupal user id for each CultureFeed user id.
 */
function culturefeed_get_uids_for_cf_uids($users) {
  $uids = array();

  if (empty($users)) {
    return array();
  }

  $result = db_query('SELECT cfu.cf_uid, u.uid FROM {culturefeed_user} cfu INNER JOIN {users} u ON u.uid = cfu.uid WHERE cfu.cf_uid IN (:cf_uids)', array(':cf_uids' => array_keys($users)));

  foreach ($result as $record) {
    $uids[$record->cf_uid] = $record->uid;
  }

  foreach ($users as $cf_uid => $user) {
    if (!isset($uids[$cf_uid])) {
      $account = culturefeed_get_or_create_user($cf_uid, $user);

      $uids[$cf_uid] = $account->uid;
    }
  }

  return $uids;
}

function culturefeed_get_uids_for_activities($activities) {
  $ids = array();

  foreach ($activities as $activity) {
    $ids[$activity->userId] = new stdClass();
    $ids[$activity->userId]->nick = $activity->nick;
    $ids[$activity->userId]->cf_uid = $activity->userId;
  }

  return culturefeed_get_uids_for_cf_uids($ids);
}

function culturefeed_get_uids_for_users($users) {
  $ids = array();

  foreach ($users as $user) {
    $ids[$user->id] = new stdClass();
    $ids[$user->id]->nick = $user->nick;
    $ids[$user->id]->cf_uid = $user->id;
  }

  return culturefeed_get_uids_for_cf_uids($ids);
}

function culturefeed_get_nodes_for_activities($activities) {
  $result = array();

  $nodes = array();

  foreach ($activities as $activity) {
    switch ($activity->contentType) {
      case CultureFeed_Activity::CONTENT_TYPE_ACTOR:
        $nodes['actor'][$activity->nodeId] = NULL;
        break;
      case CultureFeed_Activity::CONTENT_TYPE_EVENT:
        $nodes['event'][$activity->nodeId] = NULL;
        break;
      case CultureFeed_Activity::CONTENT_TYPE_PRODUCTION:
        $nodes['production'][$activity->nodeId] = NULL;
        break;
    }
  }

  foreach ($nodes as $type => $type_nodes) {
    if (empty($type_nodes)) {
      continue;
    }

    $cdbids = array_unique(array_keys($type_nodes));

    $tmp = culturefeed_get_nodes_for_cdbids($type, $cdbids);

    $result = array_merge($tmp, $result);
  }

  return $result;
}

function culturefeed_get_nodes_for_cdbids($type, $cdbids) {
  $nodes = array();

  if (empty($cdbids)) {
    return $nodes;
  }

  $query = array('cdbid' => implode(';', $cdbids), 'pagelength' => count($cdbids));
  $request = array('action' => 'list_summary', 'type' => $type, 'query' => $query);

  $results = cnapi_get($request);

  if (isset($results['data']) && !empty($results['data'])) {
    foreach ($results['data'] as $result) {
      $nodes[$result['cdbid']] = $result;
    }
  }

  return $nodes;
}

function culturefeed_activity_content_type_from_cnapi_type($type) {
  $mapping = array(
    'actor' => CultureFeed_Activity::CONTENT_TYPE_ACTOR,
    'event' => CultureFeed_Activity::CONTENT_TYPE_EVENT,
    'production' => CultureFeed_Activity::CONTENT_TYPE_PRODUCTION,
  );

  if (isset($mapping[$type])) {
    return $mapping[$type];
  }

  return CultureFeed_Activity::CONTENT_TYPE_PAGE;
}

function culturefeed_get_consumer_shared_secret($application_key) {
  foreach (module_implements('culturefeed_consumer_shared_secret') as $module) {
    $shared_secret = module_invoke($module, 'culturefeed_consumer_shared_secret', $application_key);

    if (isset($shared_secret)) {
      return $shared_secret;
    }
  }

  watchdog('culturefeed', 'The shared secret for Application key (@app_key) cannot be found!', array('@app_key' => $application_key), WATCHDOG_ERROR);

  return NULL;
}

function culturefeed_is_connected($application_key = NULL) {
  if (!isset($application_key)) {
    $application_key = variable_get('culturefeed_api_application_key', '');
  }

  $account = DrupalCultureFeed::getLoggedInAccount();

  return $account && isset($account->tokens) && !empty($account->tokens[$application_key]);
}