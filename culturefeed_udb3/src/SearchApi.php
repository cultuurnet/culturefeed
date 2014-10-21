<?php

/**
 * @file
 * Contains Drupal\culturefeed_udb3\SearchApi.
 */

namespace Drupal\culturefeed_udb3;

use Drupal\Core\Config\ConfigFactory;
use CultuurNet\Auth\ConsumerCredentials;
use CultuurNet\UDB3\SearchAPI2\DefaultSearchService;

class SearchApi {

  /**
   * The config factory.
   *
   * @var \Drupal\Core\Config\ConfigFactory;
   */
  protected $config;

  /**
   * The service.
   *
   * @var \CultuurNet\UDB3\SearchAPI2\DefaultSearchService;
   */
  public $service;

  /**
   * Constructs the search api.
   *
   * @param ConfigFactory $config_factory
   *   The config factory.
   */
  public function __construct(ConfigFactory $config_factory) {

    $this->config = $config_factory->get('culturefeed_search.api');
    $this->apiLocation = $this->config->get('api_location');
    $this->applicationKey = $this->config->get('application_key');
    $this->sharedSecret = $this->config->get('shared_secret');

    $consumerCredentials = new ConsumerCredentials();
    $consumerCredentials->setKey($this->applicationKey);
    $consumerCredentials->setSecret($this->sharedSecret);
    $this->service = new DefaultSearchService($this->apiLocation, $consumerCredentials);

  }

}
