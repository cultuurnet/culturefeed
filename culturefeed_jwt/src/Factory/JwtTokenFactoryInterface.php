<?php

namespace Drupal\culturefeed_jwt\Factory;

/**
 * Interface JwtTokenFactoryInterface.
 *
 * @package Drupal\culturefeed_jwt\Factory
 */
interface JwtTokenFactoryInterface {

  /**
   * Get the token.
   *
   * @return string
   *   The token.
   */
  public function get();

}
