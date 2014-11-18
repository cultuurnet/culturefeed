<?php

/**
 * @file
 * Worker boostrap.
 */

use Drupal\Core\DrupalKernel;
use Symfony\Component\HttpFoundation\Request;

chdir('../../../../..');
$autoloader = require_once 'core/vendor/autoload.php';
require_once 'sites/all/vendor/autoload.php';

try {
  $request = Request::createFromGlobals();
  $kernel = DrupalKernel::createFromRequest($request, $autoloader, 'prod');
  $kernel->prepareLegacyRequest($request);
  $command_bus = $kernel->getContainer()->get('culturefeed_udb3.event_command_bus');
  \CultuurNet\UDB3\CommandHandling\QueueJob::setCommandBus($command_bus);
}
catch (Exception $e) {
  $message = 'Error';
  http_response_code(500);
  print $message;
  throw $e;
}
