#!/usr/bin/env php
<?php
/**
 * @file
 *
 * CLI script to get a welcome advantage
 *
 * Expected CLI arguments:
 * - endpoint URL
 * - consumer key
 * - consumer secret
 * - uitpas number
 * - callback URL
 * - balie consumer key
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
  $uitpas_number = $_SERVER['argv'][4];
  $callback_url = $_SERVER['argv'][5];
  $counter_consumer_key = isset($_SERVER['argv'][6]) ? $_SERVER['argv'][6] : NULL;

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
  $c = new CultureFeed($new_user_oauth_client);

  $passholder = $c->uitpas()->getPassholderByUitpasNumber($uitpas_number, $counter_consumer_key);

  print_r($passholder);
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