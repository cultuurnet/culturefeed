<?php

/**
 * @file
 * Contains Drupal\culturefeed\UserCredentialsFactory.
 */

namespace Drupal\culturefeed;

use Drupal\Core\Config\ConfigFactory;
use CultuurNet\Auth\ConsumerCredentials;
use Psr\Log\LoggerInterface;

/**
 * Class ConsumerCredentialsFactory.
 *
 * @package Drupal\culturefeed
 */
class ConsumerCredentialsFactory implements ConsumerCredentialsFactoryInterface {

  /**
   * The config factory.
   *
   * @var \Drupal\Core\Config\ConfigFactory;
   */
  protected $config;

  /**
   * A logger instance.
   *
   * @var \Psr\Log\LoggerInterface
   */
  protected $logger;

  /**
   * Constructs the consumer credentials.
   *
   * @param ConfigFactory $config_factory
   *   The config factory.
   * @param \Psr\Log\LoggerInterface $logger
   *   A logger instance.
   */
  public function __construct(
    ConfigFactory $config_factory,
    LoggerInterface $logger
  ) {
    $this->config = $config_factory->get('culturefeed.api');
    $this->logger = $logger;
  }

  /**
   * {@inheritdoc}
   */
  public function get() {

    return new ConsumerCredentials(
      $this->config->get('application_key'),
      $this->config->get('shared_secret')
    );

  }

}
