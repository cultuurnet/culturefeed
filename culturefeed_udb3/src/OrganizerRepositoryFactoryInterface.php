<?php

/**
 * @file
 * Contains \Drupal\culturefeed_udb3\OrganizerRepositoryFactoryInterface.
 */

namespace Drupal\culturefeed_udb3;

use CultuurNet\UDB3\UDB2\OrganizerRepository;

/**
 * The interface for creating an organizer repository factory.
 */
interface OrganizerRepositoryFactoryInterface {

  /**
   * Returns organizer repository factory.
   *
   * @return OrganizerRepository
   *   The organizer repository.
   */
  public function get();

}
