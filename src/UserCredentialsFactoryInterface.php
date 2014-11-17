<?php

/**
 * @file
 * Contains \Drupal\culturefeed\UserCredentialsFactoryInterface.
 */

namespace Drupal\culturefeed;

use CultuurNet\Auth\ConsumerCredentials;

/**
 * The interface for creating user credentials.
 */
interface UserCredentialsFactoryInterface {

  /**
   * Returns user credentials.
   *
   * @return ConsumerCredentials
   *   The user credentials.
   */
  public function create();

}
