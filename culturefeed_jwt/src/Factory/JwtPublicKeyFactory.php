<?php

namespace Drupal\culturefeed_jwt\Factory;

use Drupal\Core\Config\ConfigFactoryInterface;
use Lcobucci\JWT\Signer\Key;

/**
 * Class JwtPublicKeyFactory.
 *
 * @package Drupal\culturefeed_jwt\Factory
 */
class JwtPublicKeyFactory {

  /**
   * The config factory.
   *
   * @var \Drupal\Core\Config\ConfigFactoryInterface
   */
  protected $config;

  /**
   * JwtPublicKeyFactory constructor.
   *
   * @param \Drupal\Core\Config\ConfigFactoryInterface $config
   *   The config factory.
   */
  public function __construct(ConfigFactoryInterface $config) {
    $this->config = $config->get('culturefeed_jwt.settings');
  }

  /**
   * Get the validation data.
   *
   * @return \Lcobucci\JWT\ValidationData
   *   The validation data.
   */
  public function get() {

    $key = $this->config->get('keys.public.file');
    return new Key($key);

  }

}
