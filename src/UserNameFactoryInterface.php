<?php

/**
 * @file
 * Contains \Drupal\culturefeed\UserNameFactoryInterface.
 */

namespace Drupal\culturefeed;

/**
 * The interface for creating a unique username.
 */
interface UserNameFactoryInterface {

  /**
   * Creates a unique username.
   *
   * @param string $name
   *   The culturefeed user name.
   *
   * @return string
   *   A unique username.
   */
  public function create($name);

}
