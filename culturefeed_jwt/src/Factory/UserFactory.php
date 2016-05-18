<?php

namespace Drupal\culturefeed_jwt\Factory;

use CultuurNet\SymfonySecurityJwt\Authentication\JwtAuthenticationProvider;
use CultuurNet\SymfonySecurityJwt\Authentication\JwtUserToken;
use CultuurNet\UDB3\Jwt\JwtDecoderServiceInterface;
use Drupal\culturefeed\UserFactoryInterface;
use ValueObjects\String\String as StringLiteral;

/**
 * Class UserFactory.
 *
 * @package Drupal\culturefeed_jwt\Factory
 */
class UserFactory implements UserFactoryInterface {

  /**
   * The jwt authentication provider.
   *
   * @var \CultuurNet\SymfonySecurityJwt\Authentication\JwtAuthenticationProvider
   */
  protected $jwtAuthenticationProvider;

  /**
   * The jwt decoder service.
   *
   * @var \CultuurNet\UDB3\Jwt\JwtDecoderServiceInterface
   */
  protected $jwtDecoderService;

  /**
   * The jwt storage token factory.
   *
   * @var \Drupal\culturefeed_jwt\Factory\JwtStorageTokenFactory
   */
  protected $jwtStorageTokenFactory;

  /**
   * Constructs a CultureFeed user object.
   *
   * @param \Drupal\culturefeed_jwt\Factory\JwtStorageTokenFactory $jwt_storage_token_factory
   *   The jwt storage token factory.
   * @param \CultuurNet\UDB3\Jwt\JwtDecoderServiceInterface $jwt_decoder_service
   *   The jwt decoder service.
   * @param \CultuurNet\SymfonySecurityJwt\Authentication\JwtAuthenticationProvider $jwt_authentication_provider
   *   The jwt authentication provider.
   */
  public function __construct(
      JwtStorageTokenFactory $jwt_storage_token_factory,
      JwtDecoderServiceInterface $jwt_decoder_service,
      JwtAuthenticationProvider $jwt_authentication_provider
  ) {

    $this->jwtStorageTokenFactory = $jwt_storage_token_factory;
    $this->jwtDecoderService = $jwt_decoder_service;
    $this->jwtAuthenticationProvider = $jwt_authentication_provider;

  }

  /**
   * {@inheritdoc}
   */
  public function get() {

    $cf_user = new \CultureFeed_User();
    $token_string = $this->jwtStorageTokenFactory->get();

    if (!empty($token_string)) {

      try {

        $jwt = $this->jwtDecoderService->parse(new StringLiteral($token_string));
        $token = new JwtUserToken($jwt);
        $this->jwtAuthenticationProvider->authenticate($token);
        $credentials = $token->getCredentials();

        $cf_user->id = $credentials->getClaim('uid');
        $cf_user->nick = $credentials->getClaim('nick');
        $cf_user->mbox = $credentials->getClaim('email');

      }
      catch (\Exception $e) {
        watchdog_exception('culturefeed_jwt', $e);
        user_logout();
      }

    }

    return $cf_user;

  }

}
