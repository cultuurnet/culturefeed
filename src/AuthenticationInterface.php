<?php

/**
 * @file
 * Contains \Drupal\culturefeed\AuthenticationInterface.
 */

namespace Drupal\culturefeed;

/**
 * The interface for authenticating a culturefeed user.
 */
interface AuthenticationInterface {

  /**
   * Returns the authentication connect url.
   *
   * @return string $url
   *   A url.
   */
  public function connect();

  /**
   * Authenticates the user.
   */
  public function authorize();

}
