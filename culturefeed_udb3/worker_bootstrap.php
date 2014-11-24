<?php

/**
 * @file
 * Worker bootstrap.
 */

use Drupal\Core\DrupalKernel;
use Symfony\Component\HttpFoundation\Request;
use CultuurNet\UDB3\CommandHandling\QueueJob;

$drupal_root_depth = getenv('DRUPAL_ROOT_DEPTH');
$depth = 0;
while ($depth < $drupal_root_depth) {
  chdir('..');
  $depth++;
}

// Bootstrap drupal after the parent forks its process and is ready to perform.
Resque_Event::listen('afterFork', function() {

    $autoloader = require_once 'core/vendor/autoload.php';
    require_once 'sites/all/vendor/autoload.php';

    try {
      $request = Request::createFromGlobals();
      $kernel = DrupalKernel::createFromRequest($request, $autoloader, 'prod');
      $kernel->prepareLegacyRequest($request);

      $command_bus = $kernel->getContainer()->get(
        'culturefeed_udb3.event_command_bus'
      );
      QueueJob::setCommandBus($command_bus);
    } catch (Exception $e) {
      $message = 'Error';
      print $message;
      throw $e;
    }
  });
