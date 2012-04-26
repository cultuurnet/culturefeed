#!/usr/bin/env php
<?php
/**
 * @file
 *
 * CLI script to get user preferences
 *
 * Expected CLI arguments:
 * - endpoint URL
 * - consumer key
 * - consumer secret
 * - base url of a RequestBin-like service (http://requestb.in/)
 */

date_default_timezone_set('Europe/Brussels');

// require the third-party oauth library which is not properly structured to be autoloaded
require_once dirname(__FILE__) . '/../../OAuth/OAuth.php';

function culturefeed_autoload($class) {
  $file = str_replace('_', '/', $class) . '.php';
  require_once $file;
}

spl_autoload_register('culturefeed_autoload');

try {
  $endpoint = $_SERVER['argv'][1];
  $consumer_key = $_SERVER['argv'][2];
  $consumer_secret = $_SERVER['argv'][3];
  $callback_url = $_SERVER['argv'][4];

  $oc = new CultureFeed_DefaultOAuthClient($consumer_key, $consumer_secret);
  $oc->setEndpoint($endpoint);
  $c = new CultureFeed($oc);

  $token = $c->getRequestToken($callback_url);

  print "Requested oauth_token: {$token['oauth_token']}" . PHP_EOL;

  // Save the token and secret in the session.
  //$_SESSION['oauth_token'] = $token['oauth_token'];
  //$_SESSION['oauth_token_secret'] = $token['oauth_token_secret'];

  // Fetch the authorisation url...
  $auth_url = $c->getUrlAuthorize($token, $callback_url, CultureFeed::AUTHORIZE_TYPE_REGULAR);

  print "Now open the following URL in your browser: {$auth_url}" . PHP_EOL;
  print "After logging in with your UiTID you should inspect your RequestBin and find there the last request with an oauth_token and oauth_verifier." . PHP_EOL;
  print "Make sure the you have identified the right request, containing the oauth token: {$token['oauth_token']}" . PHP_EOL;
  print "Give me the oauth_verifier value now: ";
  $oauth_verifier = fgets(STDIN);
  $oauth_verifier = trim($oauth_verifier);

  $user_oauth_client = new CultureFeed_DefaultOAuthClient($consumer_key, $consumer_secret, $token['oauth_token'], $token['oauth_token_secret']);
  $user_oauth_client->setEndpoint($endpoint);
  $user_c = new CultureFeed($user_oauth_client);
  $new_token = $user_c->getAccessToken($oauth_verifier);

  $new_user_oauth_client = new CultureFeed_DefaultOAuthClient($consumer_key, $consumer_secret, $new_token['oauth_token'], $new_token['oauth_token_secret']);
  $new_user_oauth_client->setEndpoint($endpoint);
  $new_user_c = new CultureFeed($new_user_oauth_client);
  $account = $new_user_c->getUser($new_token['userId']);

  print "You have succesfully logged in with the following account: {$account->nick} [{$account->id}]" . PHP_EOL;

  print "Setting user preferences" . PHP_EOL;

  $preferences = new CultureFeed_Preferences();
  $preferences->activityPrivacyPreferences[] = new CultureFeed_ActivityPrivacyPreference(CultureFeed_Activity::TYPE_UITPAS, FALSE);

  $preferences = $new_user_c->setUserPreferences($account->id, $preferences);

  if (count($preferences->activityPrivacyPreferences) == 0) {
    print "No user preferences found" . PHP_EOL;
  }
  else {
    print "User preferences:" . PHP_EOL;
    foreach ($preferences->activityPrivacyPreferences as $preference) {
      $private = $preference->private ? "private" : "public";
      print "{$preference->activityType}: {$private}" . PHP_EOL;
    }
  }

  print "Getting user preferences" . PHP_EOL;

  $preferences = $new_user_c->getUserPreferences($account->id);

  if (count($preferences->activityPrivacyPreferences) == 0) {
    print "No user preferences found" . PHP_EOL;
  }
  else {
    print "User preferences:" . PHP_EOL;
    foreach ($preferences->activityPrivacyPreferences as $preference) {
      $private = $preference->private ? "private" : "public";
      print "{$preference->activityType}: {$private}" . PHP_EOL;
    }
  }
}
catch (Exception $e) {
  $eol = PHP_EOL;
  $type = get_class($e);
  print "An exception of type {$type} was thrown." . PHP_EOL;
  print "Code: {$e->getCode()}" . PHP_EOL;
  if ($e instanceof CultureFeed_Exception) {
    print "CultureFeed error code: {$e->error_code}" . PHP_EOL;
  }
  print "Message: {$e->getMessage()}" . PHP_EOL;
  print "Stack trace: {$eol}{$e->getTraceAsString()}" . PHP_EOL;

  exit(1);
}