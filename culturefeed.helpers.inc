<?php

/**
 * Load callback for a culturefeed user.
 */
function cf_user_load($uid) {

  try {
    return DrupalCultureFeed::getUser($uid);
  }
  catch (Exception $e) {
    watchdog_e('culturefeed', $e);
  }

}

/**
 * Check if a user is a culturefeed user.
 */
function culturefeed_is_culturefeed_user($uid = NULL) {
  return DrupalCultureFeed::isCultureFeedUser($uid);
}

/**
 * Fetch a unique username for the given CultureFeed user id.
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
 * Create a new Drupal user account based on a fake Culturefeed User object
 *   $object has only the properties [cf_uid, nick]
 * @param unknown $cf_account
 * @return boolean|Ambigous <A, boolean, (optional), stdClass>
 */
function culturefeed_create_user($cf_account) {

  // Use a database transaction, as we need to handle what follows as a unit of work.
  $txn = db_transaction();

  try {

    // If no CultureFeed user was passed, we can't create the user.
    if (!$cf_account || empty($cf_account->nick)) {
      return FALSE;
    }

    // Find a unique nick based on the given nick. In case the nick is not used yet, the passed nick will be used. Else the nick will be suffixed with an underscore and counter (example nick_12).
    $unique_nick = culturefeed_unique_username($cf_account->id, $cf_account->nick);

    // Create the user account.
    $url = parse_url($GLOBALS['base_url']);
    $account = user_save(
      drupal_anonymous_user(),
      array(
        'name' => $unique_nick,
        'pass' => '', // We save an empty password (don't generate a random one) because this is much more performant.
        'init' => $unique_nick,
        'roles' => array(),
        'status' => 1,
        'timezone' => variable_get('date_default_timezone', date_default_timezone_get()),
        'access' => REQUEST_TIME,
        'mail' => '',
      )
    );

    // Save the mapping between CultureFeed User ID and Drupal user id.
    db_insert('culturefeed_user')
      ->fields(array(
      'uid' => $account->uid,
      'cf_uid' => $cf_account->id,
      ))
      ->execute();

    // Save the fact that this is an external account for culturefeed.
    user_set_authmaps($account, array('authname_culturefeed' => $account->name));
  }
  catch (Exception $e) {
    $txn->rollback();
    throw $e;
  }

  return $account;

}

function culturefeed_get_uid_for_cf_uid($cf_uid, $nick) {
  $user = new stdClass();
  $user->id = $cf_uid;
  $user->nick = $nick;

  $uids = culturefeed_get_uids_for_cf_uids(array($cf_uid => $user));

  return reset($uids);
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

  foreach ($users as $cf_uid => $fakeCulturefeedAccount) {
    if (!isset($uids[$cf_uid])) {
      $account = culturefeed_create_user($fakeCulturefeedAccount);
      if ($account) {
        $uids[$cf_uid] = $account->uid;
      }
    }
  }

  return $uids;
}

function culturefeed_get_uids_for_activities($activities) {
  $ids = array();

  foreach ($activities as $activity) {
    $ids[$activity->userId] = new stdClass();
    $ids[$activity->userId]->nick = $activity->nick;
    $ids[$activity->userId]->id = $activity->userId;
  }

  return culturefeed_get_uids_for_cf_uids($ids);
}

/**
 * Get the drupal uids for all given memberships.
 */
function culturefeed_get_uids_for_memberships($memberships) {
  $ids = array();

  foreach ($memberships as $membership) {
    $ids[$membership->user->id] = new stdClass();
    $ids[$membership->user->id]->nick = $membership->user->nick;
    $ids[$membership->user->id]->id = $membership->user->id;
  }

  return culturefeed_get_uids_for_cf_uids($ids);
}

function culturefeed_get_uids_for_users($users) {
  $ids = array();

  foreach ($users as $user) {
    $ids[$user->id] = new stdClass();
    $ids[$user->id]->nick = $user->nick;
    $ids[$user->id]->id = $user->id;
  }

  return culturefeed_get_uids_for_cf_uids($ids);
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

/**
 *
 * @param String $type
 * @return Ambigous <string>|string
 */
function culturefeed_get_content_type($type) {

  $mapping = array(
    'content' => CultureFeed_Activity::CONTENT_TYPE_CONTENT,
    'bibnet_book' => CultureFeed_Activity::CONTENT_TYPE_BOOK,
    'actor' => CultureFeed_Activity::CONTENT_TYPE_ACTOR,
    'event' => CultureFeed_Activity::CONTENT_TYPE_EVENT,
    'production' => CultureFeed_Activity::CONTENT_TYPE_PRODUCTION,
    'CultureFeed_Activity' => CultureFeed_Activity::CONTENT_TYPE_ACTIVITY,
  );

  if (isset($mapping[$type])) {
    return $mapping[$type];
  }

  return CultureFeed_Activity::CONTENT_TYPE_NODE;
}

