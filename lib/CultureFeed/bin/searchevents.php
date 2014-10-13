#!/usr/bin/env php
<?php
/**
 * @file
 *
 * CLI script to register an event
 *
 * Expected CLI arguments:
 * - endpoint URL
 * - consumer key
 * - consumer secret
 * - organizer cdbid
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
 
  $endpoint = $_SERVER['argv'][1]; //http://uitpas-acc.lodgon.com:8080/uitid/rest/
  $consumer_key = $_SERVER['argv'][2]; // 62510a93c4754ed7306113e0f2391c82
  $consumer_secret = $_SERVER['argv'][3]; // beea3e641e184c4a4bf3272ef4dcedf7
  $searchparms = $_SERVER['argv'][4]; // "startDate=2012-09-1&endDate=2012-09-31&max=5"
  
  $path = "/uitpas/cultureevent/search";
    
  $oc = new CultureFeed_DefaultOAuthClient($consumer_key, $consumer_secret);
  $oc->setEndpoint($endpoint);
  $c = new CultureFeed($oc);

  $query = new CultureFeed_Uitpas_Event_Query_SearchEventsOptions();
  $query->readQueryString( $searchparms );
  
  $data = $c->uitpas()->searchEvents($query);
  
  print_r($data);
  
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