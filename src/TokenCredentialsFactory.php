<?php

/**
 * @file
 * Contains Drupal\culturefeed\TokenCredentialsFactory.
 */

namespace Drupal\culturefeed;

use CultuurNet\Auth\TokenCredentials;

/**
 * Class ConsumerCredentialsFactory.
 *
 * @package Drupal\culturefeed
 */
class TokenCredentialsFactory implements TokenCredentialsFactoryInterface {

  /**
   * The user credentials.
   *
   * @var \Drupal\culturefeed\UserCredentials;
   */
  protected $userCredentials;

  /**
   * Constructs the token credentials.
   *
   * @param UserCredentials $user_credentials
   *   The user credentials.
   */
  public function __construct(UserCredentials $user_credentials) {
    $this->userCredentials = $user_credentials;
  }

  /**
   * {@inheritdoc}
   */
  public function get() {

    return new TokenCredentials(
      $this->userCredentials->getToken(),
      $this->userCredentials->getSecret()
    );

  }

}
