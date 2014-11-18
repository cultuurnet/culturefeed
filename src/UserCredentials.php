<?php

/**
 * @file
 * Contains Drupal\culturefeed\UserCredentials.
 */

namespace Drupal\culturefeed;

class UserCredentials {

  /**
   * The token.
   *
   * @var string
   */
  protected $token;


  /**
   * The secret.
   *
   * @var string
   */
  protected $secret;

  /**
   * Constructs the user credentials.
   *
   * @param string $token
   *   The token.
   * @param string $secret
   *   The secret.
   */
  public function __construct($token = NULL, $secret = NULL) {

    if ($token) {
      $this->setToken($token);
    }

    if ($secret) {
      $this->setSecret($secret);
    }

  }

  /**
   * Set the token.
   *
   * @param string $token
   *   The token.
   */
  public function setToken($token) {
    $this->token = $token;
  }

  /**
   * Set the secret.
   *
   * @param string $secret
   *   The secret.
   */
  public function setSecret($secret) {
    $this->secret = $secret;
  }

  /**
   * Get the token.
   *
   * @return string
   *   The token.
   */
  public function getToken() {
    return $this->token;
  }

  /**
   * Get the secret.
   *
   * @return string
   *   The secret.
   */
  public function getSecret() {
    return $this->secret;
  }

}
