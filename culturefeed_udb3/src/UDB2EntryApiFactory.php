<?php

/**
 * @file
 * Contains Drupal\culturefeed_udb3\UDB2EntryApiFactory.
 */

namespace Drupal\culturefeed_udb3;

use Drupal\Core\Config\ConfigFactory;
use CultuurNet\Auth\ConsumerCredentials;
use CultuurNet\UDB3\UDB2\Consumer;
use CultuurNet\UDB3\UDB2\EntryAPIFactory;

class UDB2EntryApiFactory implements UDB2EntryApiFactoryInterface {

  /**
   * The config factory.
   *
   * @var \Drupal\Core\Config\ConfigFactory
   */
  protected $config;

  /**
   * The credentials.
   *
   * @var \CultuurNet\Auth\ConsumerCredentials
   */
  protected $credentials;

  /**
   * Constructs an entry api factory.
   *
   * @param ConfigFactory $config_factory
   *   The config factory.
   * @param ConsumerCredentials $credentials
   *   The credentials.
   */
  public function __construct(ConfigFactory $config_factory, ConsumerCredentials $credentials) {
    $this->config = $config_factory->get('culturefeed.api');
    $this->credentials = $credentials;
  }

  /**
   * {@inheritdoc}
   */
  public function get() {

    $config = $this->config;
    $api_location = $config->get('api_location');
    $entry_api_path = $config->get('entry_api_path');
    $base_url = $api_location . $entry_api_path;
    $consumer = new Consumer($base_url, $this->credentials);
    return new EntryAPIFactory($consumer);
  }


}
