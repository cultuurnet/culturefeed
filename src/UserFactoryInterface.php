<?php

/**
 * @file
 * Contains \Drupal\culturefeed\UserFactoryInterface.
 */

namespace Drupal\culturefeed;

use CultureFeed_User;

/**
 * The interface for a culturefeed user.
 */
interface UserFactoryInterface {

  /**
   * Returns a Culturefeed User object.
   *
   * @return CultureFeed_User $user
   *   A culturefeed user object.
   */
  public function get();

}
