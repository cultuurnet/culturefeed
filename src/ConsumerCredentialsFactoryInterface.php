<?php

/**
 * @file
 * Contains \Drupal\culturefeed\ConsumerCredentialsFactoryInterface.
 */

namespace Drupal\culturefeed;

use CultuurNet\Auth\ConsumerCredentials;

/**
 * The interface for creating user credentials.
 */
interface ConsumerCredentialsFactoryInterface {

  /**
   * Returns consumer credentials.
   *
   * @return ConsumerCredentials
   *   The consumer credentials.
   */
  public function get();

}
