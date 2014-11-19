<?php

/**
 * @file
 * Contains \Drupal\culturefeed_udb3\EventRepositoryFactoryInterface.
 */

namespace Drupal\culturefeed_udb3;

use CultuurNet\UDB3\UDB2\EventRepository;

/**
 * The interface for creating an event repository factory.
 */
interface EventRepositoryFactoryInterface {

  /**
   * Returns event repository factory.
   *
   * @return EventRepository
   *   The event repository.
   */
  public function get();

}
