<?php

/**
 * @file
 * Contains Drupal\culturefeed_udb3\SearchApi2Factory.
 */

namespace Drupal\culturefeed_udb3;

use Drupal\Core\Config\ConfigFactory;
use CultuurNet\Auth\ConsumerCredentials;
use CultuurNet\UDB3\SearchAPI2\DefaultSearchService;

class SearchApi2Factory {

  /**
   * The search api 2 config.
   *
   * @var \Drupal\Core\Config\Config;
   */
  protected $config;

  /**
   * Constructs the search api.
   *
   * @param ConfigFactory $config_factory
   *   The config factory.
   */
  public function __construct(ConfigFactory $config_factory) {

    $this->config = $config_factory->get('culturefeed_search.api');
  }

  public function create() {
    $consumerCredentials = new ConsumerCredentials(
      $this->config->get('application_key'),
      $this->config->get('shared_secret')
    );

    return new DefaultSearchService(
      $this->config->get('api_location'),
      $consumerCredentials
    );
  }

}
