<?php

/**
 * Load callback for a culturefeed user.
 */
function cf_user_load($uid) {
  try {
    return DrupalCultureFeed::getUser($uid);
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed', $e);
  }

}

/**
 * Load the logged in user
 */
function culturefeed_load_logged_in_user() {
  try {
    return DrupalCultureFeed::getLoggedInUser();
  }
  catch (Exception $e) {
    watchdog_exception('culturefeed', $e);
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

  if (!$nick) {
    $nick = t('Anonymous');
  }

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
 * @param CultureFeed_User $cf_account
 * @return boolean|Ambigous <A, boolean, (optional), stdClass>
 */
function culturefeed_create_user($cf_account, $user = NULL) {

  // Use a database transaction, as we need to handle what follows as a unit of work.
  $txn = db_transaction();

  try {

    // If no CultureFeed user was passed, we can't create the user.
    if (!$cf_account) {
      return FALSE;
    }

    if (isset($user->uid) && $user->uid) {
      // User is already Drupaluser, so map his Drupal and CultureFeed account
      $uid = db_query("SELECT uid FROM {culturefeed_user} cfu WHERE cfu.uid = :uid", array(':uid' => $user->uid))->fetchField();
      if ($user->uid != $uid) {
        $account = $user;
      }
    }
    else {

      // Find a unique nick based on the given nick. In case the nick is not used yet, the passed nick will be used. Else the nick will be suffixed with an underscore and counter (example nick_12).
      $unique_nick = culturefeed_unique_username($cf_account->id, $cf_account->nick);

      // Create the user account.
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
    }

    // Save the mapping between CultureFeed User ID and Drupal user id.
    db_insert('culturefeed_user')
      ->fields(array(
      'uid' => $account->uid,
      'cf_uid' => $cf_account->id,
      ))
      ->execute();

    // Save the fact that this is an external account for culturefeed.
    user_set_authmaps($account, array('authname_culturefeed' => $account->name));

    module_invoke_all('culturefeed_user_create', $account, $cf_account);
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

/**
 * Returns a human readable string for the activity type.
 *
 * @param int $type
 *   The activity type.
 *
 * @return string
 *   The readable string
 */
function culturefeed_get_human_readable_activity_type($type) {

  $types = array(
    CultureFeed_Activity::TYPE_VIEW => t('view'),
    CultureFeed_Activity::TYPE_DETAIL => t('detail'),
    CultureFeed_Activity::TYPE_LIKE => t('like'),
    CultureFeed_Activity::TYPE_MAIL => t('mail'),
    CultureFeed_Activity::TYPE_PRINT => t('print'),
    CultureFeed_Activity::TYPE_FACEBOOK => t('Facebook'),
    CultureFeed_Activity::TYPE_TWITTER => t('Twitter'),
    CultureFeed_Activity::TYPE_IK_GA => t('attend'),
    CultureFeed_Activity::TYPE_TICKET => t('ticket'),
    CultureFeed_Activity::TYPE_ROUTE => t('route'),
    CultureFeed_Activity::TYPE_MORE_INFO => t('more info'),
    CultureFeed_Activity::TYPE_UITPAS => t('uitpas checkin'),
    CultureFeed_Activity::TYPE_REGULAR_CHECKIN => t('regular checkin'),
    CultureFeed_Activity::TYPE_COMMENT => t('comment'),
    CultureFeed_Activity::TYPE_RECOMMEND => t('recommend'),
    CultureFeed_Activity::TYPE_FOLLOW => t('follow'),
    CultureFeed_Activity::TYPE_PAGE_MEMBER => t('page member'),
    CultureFeed_Activity::TYPE_PAGE_ADMIN => t('page admin'),
    CultureFeed_Activity::TYPE_NEW_EVENT => t('new event'),
    CultureFeed_Activity::TYPE_REVIEW => t('review'),
    CultureFeed_Activity::TYPE_MEDIA_PHOTO => t('media photo'),
    CultureFeed_Activity::TYPE_MEDIA_VIDEO => t('media video'),
    CultureFeed_Activity::TYPE_PAGE_CREATED => t('page created'),
    CultureFeed_Activity::TYPE_NEWS => t('news'),
    CultureFeed_Activity::TYPE_CASHIN => t('cashin'),
  );

  return isset($types[$type]) ? $types[$type] : '';

}

/**
 * Get the preferred language of current cf user.
 */
function culturefeed_get_preferred_language() {
  $preferredLanguage = &drupal_static(__FUNCTION__, FALSE);
  global $language;

  if (!$preferredLanguage) {

    // Defaults to site language.
    $preferredLanguage = $language->language;

    // Try catching the preferred language from the user.
    try {
      $cf_account = DrupalCultureFeed::getLoggedInUser();
      if (!empty($cf_account->preferredLanguage)) {
        $preferredLanguage = $cf_account->preferredLanguage;
      }
    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_search', $e);
      return $language->language;
    }
  }

  return $preferredLanguage;
}

/**
 * Check if we are connected to acceptance API.
 */
function culturefeed_acceptance_mode() {

  // Get culturefeed_api_location
  $location = variable_get('culturefeed_api_location');
  if (strpos($location,'www') === FALSE) {
    return TRUE;
  }

  // Check for acc or dev environments that are connected with production
  else {
    global $base_root;
    $dev_ids = array('culturefeed', 'dev.', 'acc.');
    foreach($dev_ids as $id) {
      if (stripos($base_root, $id) !== false) {
        return TRUE;
      }
    }
    return FALSE;
  }

}

/**
 * Set the "noindex, follow" meta tag when needed.
 */
function culturefeed_set_noindex_metatag() {

  $element = array(
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'robots',
      'content' => 'noindex, follow',
    ),
  );

  drupal_add_html_head($element, 'robots');

}

/**
 * Store the page image in the drupal files temporary.
 * @return file object or false if failed.
 */
function culturefeed_create_temporary_image($image, $destination) {

  if (strpos($image, 'http') === false) {
    if (isset($_SERVER['HTTPS'])) {
      $image = 'https:' . $image;
    }
    else {
      $image = 'http:' . $image;
    }
  }

  $image_data = file_get_contents($image);
  file_prepare_directory($destination, FILE_CREATE_DIRECTORY);
  $uri = file_unmanaged_save_data($image_data, $destination . '/' . drupal_basename($image));

  if (empty($uri)) {
    return FALSE;
  }

  global $user;
  $file = new stdClass();
  $file->uri = $uri;
  $file->filename = drupal_basename($uri);
  $file->filemime = file_get_mimetype($file->uri);
  $file->uid = $user->uid;
  $file->status = FALSE;

  return file_save($file);
}

/**
 * Load a block through ajax.
 */
function culturefeed_ui_block_ajaxload($config) {
  $block['subject'] = $config['title'];
  $block['content'] = array(
    '#markup' => '<div id="' . $config['id'] . '">' . t('Loading') . '</div>',
    '#id' => $config['id'],
    '#ajax' => array(
      'event' => 'ajaxload',
      'path' => $config['path'],
    ),
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'culturefeed') . '/js/culturefeed_ajaxload.js',
        array('data' => array(), 'type' => 'setting'),
      ),
    ),
  );

  $block['content'] = ajax_pre_render_element($block['content']);

  return $block;
}

/**
 * Generate an image style url for an external image.
 */
function culturefeed_external_image_style($image_url, $image_style) {

  global $base_url;
  $curr_url = str_replace('http://', '', $base_url);
  $image_url_parts = explode('/', str_replace('http://', '', $image_url));
  $curr_url_parts = explode('/', $curr_url);

  $file_directory = variable_get('file_public_path', 'sites/default/files');

  // If this file is original of current host, just use the original path.
  if ($image_url_parts[0] == $curr_url_parts[0]) {
    $local_uri = trim(str_replace($base_url . '/', '', $image_url), '/');
    $local_uri = str_replace($file_directory . '/', 'public://', $local_uri);
  }
  // If the file is of another site, download it locally. This is needed to use imagecache.
  else {

    $file = new stdClass();
    $file->filename = $image_url;
    $validation_errors = file_validate_extensions($file, 'jpg jpeg png gif');
    if ($validation_errors) {
      return;
    }

    $name_parts = explode('/', $image_url);
    $file_name = $name_parts[count($name_parts) - 1];

    $local_uri = 'public://external/' . $file_name;
    $directory = $file_directory . '/external';
    // Not needed if the file already exists.
    if (!file_exists($local_uri)) {
      file_prepare_directory($directory, FILE_CREATE_DIRECTORY);
      $image_content = @file_get_contents($image_url);
      if ($image_content) {
        file_put_contents($directory . '/' . $file_name, $image_content);
      }
      else {
        return;
      }
    }

  }

  return image_style_url($image_style, $local_uri);

}

/**
 * Ajax command for a modal.
 * This will check if culturefeed_bootstrap's modal command is available.
 * If not, it will do a html insert.
 */
function culturefeed_ajax_command_modal($selector, $html) {
  return array(
    'command' => 'culturefeedModal',
    'selector' => $selector,
    'data' => $html,
  );
}

/**
 * Ajax command to reload current page.
 */
function culturefeed_ajax_command_goto($url) {
  return array(
    'command' => 'culturefeedGoto',
    'url' => $url,
  );
}


/*
f
*/
function culturefeed_share_link($share_url, $platform, $title, $device) {

  // Set prefix for share URL's.
  $facebook['prefix'] = 'https://www.facebook.com/sharer/sharer.php?u=';
  $googleplus['prefix'] = 'https://plus.google.com/share?url=';
  $twitter['prefix'] = 'https://twitter.com/intent/tweet?text=';
  $whatsapp['prefix'] = 'whatsapp://send?text=';

  // Set UTM parameters for share URL's.
  $utm['campaign'] = 'share';
  $utm['medium'] = 'social';

  $facebook['utm'] = array (
    'utm_campaign' => $utm['campaign'],
    'utm_medium' => $utm['medium'],
    'utm_source' => 'facebook.com',
  );

  $googleplus['utm'] = array (
    'utm_campaign' => $utm['campaign'],
    'utm_medium' => $utm['medium'],
    'utm_source' => 'plus.google.com',
  );

  $twitter['utm'] = array (
    'utm_campaign' => $utm['campaign'],
    'utm_medium' => $utm['medium'],
    'utm_source' => 'twitter.com',
  );

  $whatsapp['utm'] = array (
    'utm_campaign' => $utm['campaign'],
    'utm_medium' => $utm['medium'],
    'utm_source' => 'whatsapp',
  );

  $mail['utm'] = array (
    'utm_campaign' => $utm['campaign'],
    'utm_medium' => 'email',
    'utm_source' => 'forward-mail',
  );

  switch($platform)
  {
    case 'facebook' :
      return $facebook['prefix'] . urlencode(url($share_url, array('query' => $facebook['utm'])));
      break;
    case 'googleplus' :
      return $googleplus['prefix'] . urlencode(url($share_url, array('query' => $googleplus['utm'])));
      break;
    case 'twitter' :
      return $twitter['prefix'] . urlencode(url($share_url, array('query' => $twitter['utm'])));
      break;
    case 'whatsapp' :
      return $whatsapp['prefix'] . urlencode(url($share_url, array('query' => $whatsapp['utm'])));
      break;
    case 'mail' :
      if (module_exists('forward') && $device == 'computer') {
        // We can't forward external URL's
        $share_url = current_path();
        return url('forward', array('query' => array('path' => $share_url)));
      }
      else {
        return 'mailto:?subject=' . $title . '&body=' . t('I thought this might interest you:') . '%0D%0A' .  urlencode(url($share_url, array('query' => $mail['utm'])));
      }
      break;
  }

}

/**
 * Getter function to retrieve a Mobile_Detect() singleton.
 * @return object
 * Instance of Mobile_Detect(), NULL if failure.
 */
function culturefeed_mobile_detect_get_object() {
  $detect = &drupal_static(__FUNCTION__);

  if (!isset($detect)) {
    if (class_exists('Mobile_Detect')) {
      try {
        $detect = new Mobile_Detect();
      } catch (Exception $e) {
        $detect = NULL;
        watchdog('mobile_detect', 'Could not instantiate Mobile_Detect(): %message',
          array('%message' => $e->getMessage()), WATCHDOG_ALERT);
      }
    } else {
      $detect = NULL;
      watchdog('mobile_detect', 'Mobile_Detect() class does not exist', array(), WATCHDOG_ALERT);
    }
  }

  return $detect;
}

/**
 * Known versions of cdb xml.
 *
 * @return array
 */
function culturefeed_cdb_xml_versions() {
  $cdb_xml_versions = array(
    '3.0',
    '3.1',
    '3.2',
    '3.3',
  );

  return $cdb_xml_versions;
}
