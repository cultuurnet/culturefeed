<?php

namespace Drupal\culturefeed_jwt\Factory;

use CultuurNet\Auth\ConsumerCredentials;
use CultuurNet\UitidCredentials\UitidCredentialsFetcher;
use Drupal\Core\Config\ConfigFactoryInterface;

class UitidCredentialsFetcherFactory {

  /**
   * The config factory.
   *
   * @var \Drupal\Core\Config\ConfigFactoryInterface
   */
  protected $config;

  /**
   * The consumer credentials.
   *
   * @var \CultuurNet\Auth\ConsumerCredentials
   */
  protected $consumerCredentials;

  /**
   * UitidCredentialsFetcherFactory constructor.
   *
   * @param \Drupal\Core\Config\ConfigFactoryInterface $config
   *   The config factory.
   * @param \CultuurNet\Auth\ConsumerCredentials
   *   The consumer credentials.
   */
  public function __construct(ConfigFactoryInterface $config, ConsumerCredentials $consumer_credentials) {
    $this->config = $config->get('culturefeed.api');
    $this->consumerCredentials = $consumer_credentials;
  }

  /**
   * Get an uitid credentials fetcher.
   *
   * @return \CultuurNet\UitidCredentials\UitidCredentialsFetcher
   *   The uitid credentials fetcher.
   */
  public function get() {

    $url = parse_url($this->config->get('api_location'));
    $base_url = $url['scheme'] . '://' . $url['host'];
    return new UitidCredentialsFetcher($base_url, $this->consumerCredentials);

  }

}
