<?php

/**
 * @file
 * Contains \Drupal\culturefeed\UserMapInterface.
 */

namespace Drupal\culturefeed;

use CultureFeed_User;

/**
 * The interface for mapping a culturefeed user.
 */
interface UserMapInterface {

  /**
   * Maps a culturefeed user to a user object.
   *
   * @param CultureFeed_User $user
   *   A Culturefeed user.
   * @param array $token
   *   An associative array containing the token, secret and callback confirmed
   *   status.
   *
   * @return mixed $user
   *   A user object.
   */
  public function get(CultureFeed_User $user, array $token);

  /**
   * Returns the culturefeed user id.
   *
   * @param int $id
   *   A drupal user id.
   *
   * @return string
   *   A culturefeed user id.
   */
  public function getCultureFeedId($id);

  /**
   * Returns the drupal user id.
   *
   * @param string $id
   *   A culturefeed user id.
   *
   * @return int
   *   A drupal user id.
   */
  public function getDrupalId($id);

}
