<?php

/**
 * @file
 * Contains \Drupal\culturefeed_udb3\PlaceRepositoryFactoryInterface.
 */

namespace Drupal\culturefeed_udb3;

use CultuurNet\UDB3\UDB2\PlaceRepository;

/**
 * The interface for creating a place repository factory.
 */
interface PlaceRepositoryFactoryInterface {

  /**
   * Returns place repository factory.
   *
   * @return PlaceRepository
   *   The place repository.
   */
  public function get();

}
