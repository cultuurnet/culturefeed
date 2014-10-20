<?php

/**
 * @file
 * Contains Drupal\culturefeed\Instance.
 */

namespace Drupal\culturefeed;

use CultureFeed;
use CultureFeed_DefaultOAuthClient;

class Instance {

  /**
   * The OAuth Client.
   *
   * @var \CultureFeed_DefaultOAuthClient;
   */
  protected $oauthClient;

  /**
   * The application key.
   *
   * @var string;
   */
  public $applicationKey;

  /**
   * Constructs a culturefeed instance.
   *
   * @param OAuthClient $oauth_client
   *   The oauth client.
   */
  public function __construct(OAuthClient $oauth_client) {
    $this->oauthClient = $oauth_client;
    $this->applicationKey = $this->oauthClient->applicationKey;
  }

  /**
   * Returns a new Culturefeed Instance.
   *
   * @param string $token
   *   The token.
   * @param string $secret
   *   The secret.
   *
   * @return CultureFeed
   */
  public function get($token = NULL, $secret = NULL) {
    $oauthClient = $this->oauthClient->get($token, $secret);
    return new CultureFeed($oauthClient);
  }

}
