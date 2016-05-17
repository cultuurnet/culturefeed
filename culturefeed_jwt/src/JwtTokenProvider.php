<?php

namespace Drupal\culturefeed_jwt;

use CultuurNet\SymfonySecurityJwt\Authentication\JwtAuthenticationProvider;
use CultuurNet\SymfonySecurityJwt\Authentication\JwtUserToken;
use CultuurNet\UDB3\Jwt\JwtDecoderServiceInterface;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\Security\Core\Exception\AuthenticationException;
use ValueObjects\String\String as StringLiteral;

/**
 * Class JwtTokenProvider.
 *
 * @package Drupal\culturefeed_jwt
 */
class JwtTokenProvider {

  /**
   * The authentication provider.
   *
   * @var \CultuurNet\SymfonySecurityJwt\Authentication\JwtAuthenticationProvider
   */
  protected $authenticationProvider;

  /**
   * The decoder service.
   *
   * @var \CultuurNet\UDB3\Jwt\JwtDecoderServiceInterface
   */
  protected $decoderService;

  /**
   * JwtTokenProvider constructor.
   *
   * @param \CultuurNet\UDB3\Jwt\JwtDecoderServiceInterface $decoder_service
   *   The decoder service.
   * @param \CultuurNet\SymfonySecurityJwt\Authentication\JwtAuthenticationProvider $authentication_provider
   *   The authentication provider.
   */
  public function __construct(
      JwtDecoderServiceInterface $decoder_service,
      JwtAuthenticationProvider $authentication_provider
  ) {
    $this->decoderService = $decoder_service;
    $this->authenticationProvider = $authentication_provider;
  }

  /**
   * Get the token from the request.
   *
   * @param \Symfony\Component\HttpFoundation\Request $request
   *   The request.
   *
   * @return null|\CultuurNet\SymfonySecurityJwt\Authentication\JwtUserToken
   *   The user token.
   */
  public function getFromRequest(Request $request) {

    // First try to get the token string from an authorization header.
    // Second check query parameters.
    $token_string = $this->getTokenStringFromHeader($request);
    if (empty($token_string)) {
      $query = $request->query;
      $token_string = $query->get('jwt');
    }

    if (!empty($token_string)) {

      $jwt = $this->decoderService->parse(new StringLiteral($token_string));
      $token = new JwtUserToken($jwt);
      try {
        $authenticated_token = $this->authenticationProvider->authenticate($token);
        return $authenticated_token;
      }
      catch (AuthenticationException $e) {
        watchdog_exception('culturefeed_jwt', $e);
      }

    }

    return NULL;

  }

  /**
   * Get the token string from the authorization header.
   *
   * @param \Symfony\Component\HttpFoundation\Request $request
   *   The request.
   *
   * @return null|string
   *   The token string.
   */
  private function getTokenStringFromHeader(Request $request) {

    $authorization = $request->headers->get('authorization');
    $bearer_prefix = 'Bearer ';

    if (!$authorization || strpos($authorization, $bearer_prefix) !== 0) {
      return NULL;
    }

    return substr($authorization, strlen($bearer_prefix));

  }

}
