<?php

namespace Drupal\culturefeed_jwt\Factory;

use CultuurNet\SymfonySecurityJwt\Authentication\JwtAuthenticationProvider;
use CultuurNet\SymfonySecurityJwt\Authentication\JwtUserToken;
use CultuurNet\UDB3\Jwt\JwtDecoderServiceInterface;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\RequestStack;
use Symfony\Component\Security\Core\Exception\AuthenticationException;
use ValueObjects\String\String as StringLiteral;

/**
 * Class JwtStatelessTokenFactory.
 *
 * @package Drupal\culturefeed_jwt\Factory
 */
class JwtStatelessTokenFactory {

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
   * The request stack.
   *
   * @var \Symfony\Component\HttpFoundation\RequestStack
   */
  protected $requestStack;

  /**
   * JwtStatelessTokenFactory constructor.
   *
   * @param \Symfony\Component\HttpFoundation\RequestStack $request_stack
   *   The request stack.
   * @param \CultuurNet\UDB3\Jwt\JwtDecoderServiceInterface $decoder_service
   *   The decoder service.
   * @param \CultuurNet\SymfonySecurityJwt\Authentication\JwtAuthenticationProvider $authentication_provider
   *   The authentication provider.
   */
  public function __construct(
      RequestStack $request_stack,
      JwtDecoderServiceInterface $decoder_service,
      JwtAuthenticationProvider $authentication_provider
  ) {
    $this->requestStack = $request_stack;
    $this->decoderService = $decoder_service;
    $this->authenticationProvider = $authentication_provider;
  }

  /**
   * Get the token.
   *
   * @return string
   *   The token.
   */
  public function get() {

    $request = $this->requestStack->getCurrentRequest();
    $token_string = $this->getTokenStringFromHeader($request);

    if (!empty($token_string)) {

      $jwt = $this->decoderService->parse(new StringLiteral($token_string));
      $token = new JwtUserToken($jwt);
      try {
        $this->authenticationProvider->authenticate($token);
        return $token_string;
      }
      catch (AuthenticationException $e) {
        watchdog_exception('culturefeed_jwt', $e);
      }

    }

    return '';

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
