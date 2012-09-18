#!/usr/bin/env php
<?php
/**
 * @file
 * CLI script to register an event
 */

date_default_timezone_set('Europe/Brussels');
// Require the third-party oauth library which is not properly structured
// to be autoloaded.
require_once dirname(__FILE__) . '/../../OAuth/OAuth.php';

/**
 * Auto load the cFeed libs.
 */
function culturefeed_autoload($class) {
  $file = str_replace('_', '/', $class) . '.php';
  require_once $file;
}

spl_autoload_register('culturefeed_autoload');

try {

  // http://uitpas-acc.lodgon.com:8080/uitid/rest/
  // 62510a93c4754ed7306113e0f2391c82
  // beea3e641e184c4a4bf3272ef4dcedf7
  $endpoint = $_SERVER['argv'][1];
  $consumer_key = $_SERVER['argv'][2];
  $consumer_secret = $_SERVER['argv'][3];

  $oc = new CultureFeed_DefaultOAuthClient($consumer_key, $consumer_secret);
  $oc->setEndpoint($endpoint);
  $c = new CultureFeed($oc);

  $token = $c->getRequestToken();
  $event_xml_str = file_get_contents(dirname(__FILE__) . "/../test/uitpas/data/events/event.xml");
  $event_xml_obj = new CultureFeed_SimpleXMLElement($event_xml_str);

  $event = CultureFeed_Uitpas_Event_CultureEvent::createFromXML($event_xml_obj);

  $data = $c->uitpas()->registerEvent($event);

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
