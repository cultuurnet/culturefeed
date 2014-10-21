<?php

/**
 * @file
 * Contains \Drupal\culturefeed\OAuthClientFactoryInterface.
 */

namespace Drupal\culturefeed;

use CultureFeed_DefaultOAuthClient;

/**
 * The interface for creating a Oauth client.
 */
interface OAuthClientFactoryInterface {

  /**
   * Returns a new OAuthClient.
   *
   * @param string $token
   *   The token.
   * @param string $secret
   *   The secret.
   *
   * @return CultureFeed_DefaultOAuthClient
   *   The OAuthClient.
   */
  public function create($token = NULL, $secret = NULL);

}
