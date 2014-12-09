<?php

/**
 * @file
 * Contains Drupal\culturefeed\OAuthClientFactory.
 */

namespace Drupal\culturefeed;

use Drupal\Core\Config\ConfigFactory;
use CultureFeed_DefaultOAuthClient;

/**
 * Class OAuthClientFactory.
 *
 * @package Drupal\culturefeed
 */
class OAuthClientFactory implements OAuthClientFactoryInterface {

  /**
   * The config factory.
   *
   * @var \Drupal\Core\Config\ConfigFactory;
   */
  protected $config;

  /**
   * The api location.
   *
   * @var string;
   */
  protected $apiLocation;

  /**
   * The application key.
   *
   * @var string;
   */
  protected $applicationKey;

  /**
   * The shared secret.
   *
   * @var string;
   */
  protected $sharedSecret;

  /**
   * Constructs a new OAuthClient.
   *
   * @param ConfigFactory $config_factory
   *   The config factory.
   */
  public function __construct(ConfigFactory $config_factory) {

    $this->config = $config_factory->get('culturefeed.api');
    $this->apiLocation = $this->config->get('api_location');
    $this->applicationKey = $this->config->get('application_key');
    $this->sharedSecret = $this->config->get('shared_secret');

  }

  /**
   * {@inheritdoc}
   */
  public function create($token = NULL, $secret = NULL) {

    $oauth_client = new CultureFeed_DefaultOAuthClient($this->applicationKey, $this->sharedSecret, $token, $secret);
    $oauth_client->setEndpoint($this->apiLocation);
    return $oauth_client;

  }

}
