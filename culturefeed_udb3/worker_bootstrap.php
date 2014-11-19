<?php

/**
 * @file
 * Worker boostrap.
 */

use Drupal\Core\DrupalKernel;
use Symfony\Component\HttpFoundation\Request;
use CultuurNet\UDB3\CommandHandling\QueueJob;

chdir('../../../../..');
$autoloader = require_once 'core/vendor/autoload.php';
require_once 'sites/all/vendor/autoload.php';

try {
  $request = Request::createFromGlobals();
  $kernel = DrupalKernel::createFromRequest($request, $autoloader, 'prod');
  $kernel->prepareLegacyRequest($request);

  // We have to enable the user and system modules, even to check access and
  // display errors via the maintenance theme.
  \Drupal::moduleHandler()->addModule('system', 'core/modules/system');
  \Drupal::moduleHandler()->addModule('user', 'core/modules/user');
  \Drupal::moduleHandler()->load('system');
  \Drupal::moduleHandler()->load('user');

  $command_bus = $kernel->getContainer()->get('culturefeed_udb3.event_command_bus');
  QueueJob::setCommandBus($command_bus);
}
catch (Exception $e) {
  $message = 'Error';
  print $message;
  throw $e;
}
