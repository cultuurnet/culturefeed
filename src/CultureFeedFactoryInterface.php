<?php

/**
 * @file
 * Contains \Drupal\culturefeed\CultureFeedFactoryInterface.
 */

namespace Drupal\culturefeed;

use CultureFeed;

/**
 * The interface for creating a CultureFeed instance.
 */
interface CultureFeedFactoryInterface {

  /**
   * Returns a new Culturefeed Instance.
   *
   * @param string $token
   *   The token.
   * @param string $secret
   *   The secret.
   *
   * @return CultureFeed
   *   The CultureFeed instance.
   */
  public function create($token = NULL, $secret = NULL);

  /**
   * Returns a new authenticated Culturefeed Instance.
   *
   * @return CultureFeed
   *   The CultureFeed instance.
   */
  public function createAuthenticated();

}
