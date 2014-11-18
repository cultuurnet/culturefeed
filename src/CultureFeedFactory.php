<?php

/**
 * @file
 * Contains Drupal\culturefeed\CultureFeedFactory.
 */

namespace Drupal\culturefeed;

use CultureFeed;
use Psr\Log\LoggerInterface;

class CultureFeedFactory implements CultureFeedFactoryInterface {

  /**
   * The OAuth client factory.
   *
   * @var \Drupal\culturefeed\OAuthClientFactory;
   */
  protected $oauthClient;

  /**
   * The consumer credentials.
   *
   * @var \CultuurNet\Auth\ConsumerCredentials;
   */
  protected $consumerCredentials;

  /**
   * A logger instance.
   *
   * @var \Psr\Log\LoggerInterface
   */
  protected $logger;

  /**
   * Constructs a culturefeed instance.
   *
   * @param OAuthClientFactoryInterface $oauth_client
   *   The oauth client.
   * @param UserCredentials $user_credentials
   *   The user credentials.
   * @param \Psr\Log\LoggerInterface $logger
   *   A logger instance.
   */
  public function __construct(
    OAuthClientFactoryInterface $oauth_client,
    UserCredentials $user_credentials,
    LoggerInterface $logger
  ) {

    $this->oauthClient = $oauth_client;
    $this->userCredentials = $user_credentials;
    $this->logger = $logger;

  }

  /**
   * {@inheritdoc}
   */
  public function create($token = NULL, $secret = NULL) {
    $oauth_client = $this->oauthClient->create($token, $secret);
    return new CultureFeed($oauth_client);
  }

  /**
   * {@inheritdoc}
   */
  public function createAuthenticated() {

    $token = $this->userCredentials->getToken();
    $secret = $this->userCredentials->getSecret();

    try {
      return $this->create($token, $secret);
    }
    catch (\Exception $e) {
      $this->logger->error('No authenticated instance could be created.');
    }

    return $this->create();

  }

}
