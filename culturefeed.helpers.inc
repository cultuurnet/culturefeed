<?php

/**
 * Fetch an unique username for the given Culture Feed user id.
 */
function culturefeed_unique_username($cf_uid, $nick) {
  // if previously registered, reuse local account
  $name = db_query("SELECT DISTINCT name FROM {culturefeed_user} cfu INNER JOIN {users} u ON u.uid = cfu.uid WHERE cfu.cf_uid = :cf_uid", array(':cf_uid' => $cf_uid))->fetchField();

  if ($name) {
    return $name;
  }

  $name = $nick;

  $count = 0;
  while ($name_exists = db_query("SELECT name FROM {users} WHERE LOWER(name) = :name", array(':name' => strtolower($nick)))->fetchField()) {
    $count++;
    $name = $name . '_' . $count;
  }

  return $name;
}

function culturefeed_user_create($cf_uid, $nick) {
  $url = parse_url($GLOBALS['base_url']);
  $account = user_save(
    drupal_anonymous_user(),
    array(
      'name' => $nick,
      'pass' => user_password(),
      'init' => $nick,
      'roles' => array(),
      'status' => 1,
      'timezone' => variable_get('date_default_timezone', date_default_timezone_get()),
      'access' => REQUEST_TIME,
    )
  );

  return $account;
}

function culturefeed_user_get($cf_uid, $nick = NULL, $token = NULL, $secret = NULL, $application_key = NULL) {
  $account = NULL;

  if (!$application_key) {
    $application_key = variable_get('culturefeed_api_application_key', '');
  }

  $user_exists = db_query("SELECT * FROM {culturefeed_user} cfu WHERE cfu.cf_uid = :cf_uid", array(':cf_uid' => $cf_uid))->fetchObject();

  if (!$user_exists) {
    if (!$nick) {
      return FALSE;
    }

    $unique_nick = culturefeed_unique_username($cf_uid, $nick);

    $account = culturefeed_user_create($cf_uid, $unique_nick);
    db_insert('culturefeed_user')
      ->fields(array(
        'uid' => $account->uid,
        'cf_uid' => $cf_uid,
        ))
      ->execute();

    user_set_authmaps($account, array('authname_culturefeed' => $account->name));

  }
  else {
    $account = user_load($user_exists->uid);
  }

  if ($token) {
    $token_exists = db_query("SELECT * FROM {culturefeed_token} cft WHERE cft.cf_uid = :cf_uid AND cft.application_key = :application_key", array(':cf_uid' => $cf_uid, ':application_key' => $application_key))->fetchObject();

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

  return $account;
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
    $cdbids = array_unique(array_keys($type_nodes));

    if (!empty($cdbids)) {
      $tmp = culturefeed_get_nodes_for_cdbids($type, $cdbids);

      $result = array_merge($tmp, $result);

    }
  }

  return $result;
}

function culturefeed_get_nodes_for_cdbids($type, $cdbids) {
  $query = array('cdbid' => implode(';', $cdbids), 'pagelength' => count($cdbids));
  $request = array('action' => 'list_summary', 'type' => $type, 'query' => $query);
  
  $results = cnapi_get($request);
  
  $nodes = array();

  if (!empty($results['data'])) {
    foreach ($results['data'] as $result) {
      $nodes[$result['cdbid']] = $result;
    }
  }

  return $nodes;
}

function culturefeed_log($type, $exception) {
  watchdog($type, '[%error]: !message in %file on line %line.', array('%error' => $exception->getCode(), '!message' => $exception->getMessage(), '%file' => $exception->getFile(), '%line' => $exception->getLine()), WATCHDOG_ERROR);
}