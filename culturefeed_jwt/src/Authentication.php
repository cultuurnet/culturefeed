<?php

namespace Drupal\culturefeed_jwt;

use CultuurNet\Auth\ConsumerCredentials;
use CultuurNet\SymfonySecurityJwt\Authentication\JwtAuthenticationProvider;
use CultuurNet\SymfonySecurityJwt\Authentication\JwtUserToken;
use CultuurNet\UDB3\Jwt\JwtDecoderServiceInterface;
use CultuurNet\UitidCredentials\UitidCredentialsFetcher;
use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\Entity\Query\QueryFactory;
use Drupal\culturefeed\UserMapInterface;
use Exception;
use Symfony\Component\HttpFoundation\Request;
use ValueObjects\String\String as StringLiteral;

/**
 * Class Authentication.
 *
 * @package Drupal\culturefeed_jwt
 */
class Authentication implements AuthenticationInterface {

  /**
   * The authentication provider.
   *
   * @var \CultuurNet\SymfonySecurityJwt\Authentication\JwtAuthenticationProvider
   */
  protected $authenticationProvider;

  /**
   * The consumer credentials.
   *
   * @var \CultuurNet\Auth\ConsumerCredentials
   */
  protected $consumerCredentials;

  /**
   * The decoder service.
   *
   * @var \CultuurNet\UDB3\Jwt\JwtDecoderServiceInterface
   */
  protected $decoderService;

  /**
   * The entity type manager.
   *
   * @var \Drupal\Core\Entity\EntityTypeManagerInterface;
   */
  protected $entityTypeManager;

  /**
   * The query factory.
   *
   * @var \Drupal\Core\Entity\Query\QueryFactory;
   */
  protected $entityQuery;

  /**
   * The uitid credentials fetcher.
   *
   * @var \CultuurNet\UitidCredentials\UitidCredentialsFetcher
   */
  protected $uitidCredentialsFetcher;

  /**
   * The user map.
   *
   * @var \Drupal\culturefeed\UserMapInterface
   */
  protected $userMap;

  /**
   * Authentication constructor.
   *
   * @param \CultuurNet\UDB3\Jwt\JwtDecoderServiceInterface $decoder_service
   *   The decoder service.
   * @param \CultuurNet\SymfonySecurityJwt\Authentication\JwtAuthenticationProvider $authentication_provider
   *   The authentication provider.
   * @param \Drupal\culturefeed\UserMapInterface $user_map
   *   The user map.
   * @param EntityTypeManagerInterface $entity_type_manager
   *   The entity type manger.
   * @param QueryFactory $entity_query
   *   The query factory.
   * @param \CultuurNet\Auth\ConsumerCredentials $consumer_credentials
   *   The consumer credentials.
   * @param \CultuurNet\UitidCredentials\UitidCredentialsFetcher $uitid_credentials_fetcher
   *   The uitid credentials fetcher.
   */
  public function __construct(
      JwtDecoderServiceInterface $decoder_service,
      JwtAuthenticationProvider $authentication_provider,
      UserMapInterface $user_map,
      EntityTypeManagerInterface $entity_type_manager,
      QueryFactory $entity_query,
      ConsumerCredentials $consumer_credentials,
      UitidCredentialsFetcher $uitid_credentials_fetcher
  ) {

    $this->decoderService = $decoder_service;
    $this->authenticationProvider = $authentication_provider;
    $this->userMap = $user_map;
    $this->entityTypeManager = $entity_type_manager;
    $this->entityQuery = $entity_query;
    $this->consumerCredentials = $consumer_credentials;
    $this->uitidCredentialsFetcher = $uitid_credentials_fetcher;

  }

  /**
   * {@inheritdoc}
   */
  public function authorize(Request $request) {

    try {

      $query = $request->query;
      $token_string = $query->get('jwt');
      $jwt = $this->decoderService->parse(new StringLiteral($token_string));
      $token = new JwtUserToken($jwt);
      $this->authenticationProvider->authenticate($token);
      $credentials = $token->getCredentials();
      setcookie('token', json_encode($token_string), NULL, '/');

      $cf_user = new \CultureFeed_User();
      $cf_user->id = $credentials->getClaim('uid');
      $cf_user->nick = $credentials->getClaim('nick');
      $cf_user->mbox = $credentials->getClaim('email');

    }
    catch (Exception $e) {

      drupal_set_message(t('An error occurred while logging in. Please try again later.'), 'error');
      watchdog_exception('culturefeed', $e);
      return;

    }

    $account = $this->userMap->get($cf_user);

    // Store the jwt token.
    $storage = $this->entityTypeManager->getStorage('culturefeed_jwt_token');

    $query = $this->entityQuery->get('culturefeed_jwt_token')->condition('uitid', $cf_user->id);
    $result = $query->execute();
    $entities = $storage->loadMultiple(array_keys($result));
    $storage->delete($entities);

    $storage->create(array(
      'uitid' => $cf_user->id,
      'token' => $token_string,
    ))->save();

    // Store the oauth token.
    $storage = $this->entityTypeManager->getStorage('culturefeed_token');

    $query = $this->entityQuery->get('culturefeed_token')->condition('uitid', $cf_user->id);
    $result = $query->execute();
    $entities = $storage->loadMultiple(array_keys($result));
    $storage->delete($entities);

    $uitid_credentials = $this->uitidCredentialsFetcher->getAccessTokenFromJwt($jwt);

    if ($uitid_credentials) {

      $storage->create(array(
        'uitid' => $cf_user->id,
        'token' => $uitid_credentials->getToken(),
        'secret' => $uitid_credentials->getTokenSecret(),
        'application_key' => $this->consumerCredentials->getKey(),
      ))->save();

    }

    if ($account) {
      user_login_finalize($account);
    }

  }

}
