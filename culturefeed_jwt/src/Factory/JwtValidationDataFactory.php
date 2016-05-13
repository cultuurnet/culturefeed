<?php

namespace Drupal\culturefeed_jwt\Factory;

use Drupal\Core\Config\ConfigFactoryInterface;
use Lcobucci\JWT\ValidationData;

/**
 * Class JwtValidationDataFactory.
 *
 * @package Drupal\culturefeed_jwt\Factory
 */
class JwtValidationDataFactory {

  /**
   * The config factory.
   *
   * @var \Drupal\Core\Config\ConfigFactoryInterface
   */
  protected $config;

  /**
   * JwtValidationDataFactory constructor.
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

    $validation = $this->config->get('validation');
    $validation_data = new ValidationData();

    $claims = !empty($validation) ? $validation : [];

    foreach ($claims as $claim => $value) {

      switch ($claim) {
        case 'jti':
          $validation_data->setId($value);
          break;

        case 'iss':
          $validation_data->setIssuer($value);
          break;

        case 'aud':
          $validation_data->setAudience($value);
          break;

        case 'sub':
          $validation_data->setSubject($value);
          break;

        case 'current_time':
          $validation_data->setCurrentTime($value);
          break;
      }

    }

    return $validation_data;

  }

}
