<?php

namespace Drupal\culturefeed_jwt\Factory;

use CultuurNet\UDB3\Jwt\JwtDecoderService;
use Lcobucci\JWT\Parser;
use Lcobucci\JWT\Signer\Key;
use Lcobucci\JWT\Signer\Rsa\Sha256;
use Lcobucci\JWT\ValidationData;

/**
 * Class JwtDecoderServiceFactory.
 *
 * @package Drupal\culturefeed_jwt\Factory
 */
class JwtDecoderServiceFactory {

  /**
   * The key.
   *
   * @var \Lcobucci\JWT\Signer\Key
   */
  protected $key;

  /**
   * The validation data.
   *
   * @var \Lcobucci\JWT\ValidationData
   */
  protected $validationData;

  /**
   * JwtDecoderServiceFactory constructor.
   *
   * @param \Lcobucci\JWT\ValidationData $validation_data
   *   The validation data.
   * @param \Lcobucci\JWT\Signer\Key $key
   *   The key.
   */
  public function __construct(ValidationData $validation_data, Key $key) {
    $this->key = $key;
    $this->validationData = $validation_data;
  }

  /**
   * Get the decoder service.
   *
   * @return \CultuurNet\UDB3\Jwt\JwtDecoderService
   *   The decoder service.
   */
  public function get() {

    return new JwtDecoderService(
      new Parser(),
      $this->validationData,
      new Sha256(),
      $this->key,
      array(
        'uid',
        'nick',
        'email',
      )
    );

  }

}
