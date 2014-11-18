<?php

/**
 * @file
 * Contains \Drupal\culturefeed_udb3\LoggerCommandBusFactoryInterface.
 */

namespace Drupal\culturefeed_udb3;

use Monolog\Logger;

/**
 * The interface for creating an entry api factory.
 */
interface LoggerCommandBusFactoryInterface {

  /**
   * Returns command bus logger.
   *
   * @return Logger
   *   The logger.
   */
  public function get();

}
