<?php

/**
 * @file
 * Contains \Drupal\culturefeed\UserMapInterface.
 */

namespace Drupal\culturefeed;

/**
 * The interface for a culturefeed user.
 */
interface UserInterface {

  /**
   * Maps a culturefeed user to a user object.
   *
   * @return mixed $user
   *   A user object.
   */
  public function get();

}
