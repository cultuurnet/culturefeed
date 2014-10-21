<?php

/**
 * @file
 * Contains Drupal\culturefeed\CultureFeedFactory.
 */

namespace Drupal\culturefeed;

use CultureFeed;

class CultureFeedFactory implements CultureFeedFactoryInterface {

  /**
   * The OAuth client factory.
   *
   * @var \Drupal\culturefeed\OAuthClientFactory;
   */
  protected $oauthClient;

  /**
   * Constructs a culturefeed instance.
   *
   * @param OAuthClientFactoryInterface $oauth_client
   *   The oauth client.
   */
  public function __construct(OAuthClientFactoryInterface $oauth_client) {
    $this->oauthClient = $oauth_client;
  }

  /**
   * {@inheritdoc}
   */
  public function create($token = NULL, $secret = NULL) {
    $oauth_client = $this->oauthClient->create($token, $secret);
    return new CultureFeed($oauth_client);
  }

}
