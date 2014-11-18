<?php

/**
 * @file
 * Contains \Drupal\culturefeed\UserCredentialsFactoryInterface.
 */

namespace Drupal\culturefeed;

/**
 * The interface for creating user credentials.
 */
interface UserCredentialsFactoryInterface {

  /**
   * Returns user credentials.
   *
   * @return UserCredentials
   *   The user credentials.
   */
  public function get();

}
