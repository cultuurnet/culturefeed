<?php

/**
 * @file
 * Contains \Drupal\culturefeed\TokenCredentialsFactoryInterface.
 */

namespace Drupal\culturefeed;

/**
 * The interface for creating token credentials.
 */
interface TokenCredentialsFactoryInterface {

  /**
   * Returns token credentials.
   *
   * @return \CultuurNet\Auth\TokenCredentials
   *   The token credentials.
   */
  public function get();

}
