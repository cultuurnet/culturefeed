<?php

namespace Drupal\culturefeed_jwt\Factory;

use CultuurNet\SymfonySecurityJwt\Authentication\JwtUserToken;
use CultuurNet\UDB3\Jwt\JwtDecoderServiceInterface;
use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\Entity\Query\QueryFactory;
use Drupal\Core\Session\AccountInterface;
use Drupal\culturefeed\UserFactoryInterface;
use Drupal\culturefeed\UserMapInterface;
use Drupal\culturefeed_jwt\JwtTokenProvider;
use Symfony\Component\HttpFoundation\RequestStack;
use ValueObjects\String\String as StringLiteral;

/**
 * Class UserFactory.
 *
 * @package Drupal\culturefeed_jwt\Factory
 */
class UserFactory implements UserFactoryInterface {

  /**
   * The account.
   *
   * @var \Drupal\Core\Session\AccountInterface;
   */
  protected $account;

  /**
   * The entity storage.
   *
   * @var \Drupal\Core\Entity\EntityStorageInterface
   */
  protected $entityStorage;

  /**
   * The jwt decoder service.
   *
   * @var \CultuurNet\UDB3\Jwt\JwtDecoderServiceInterface
   */
  protected $jwtDecoderService;

  /**
   * The jwt token provider.
   *
   * @var \Drupal\culturefeed_jwt\JwtTokenProvider
   */
  protected $jwtTokenProvider;

  /**
   * The query factory.
   *
   * @var \Drupal\Core\Entity\Query\QueryFactory
   */
  protected $queryFactory;

  /**
   * The request stack.
   *
   * @var \Symfony\Component\HttpFoundation\RequestStack
   */
  protected $requestStack;

  /**
   * The user map.
   *
   * @var \Drupal\culturefeed\UserMapInterface;
   */
  protected $userMap;

  /**
   * Constructs a CultureFeed user object.
   *
   * @param \Drupal\Core\Session\AccountInterface $account
   *   The account interface.
   * @param \Drupal\culturefeed\UserMapInterface $user_map
   *   The user map.
   * @param \Drupal\Core\Entity\EntityTypeManagerInterface $entity_type_manager
   *   The entity type manager.
   * @param \Drupal\Core\Entity\Query\QueryFactory $query_factory
   *   The query factory.
   * @param \CultuurNet\UDB3\Jwt\JwtDecoderServiceInterface $jwt_decoder_service
   *   The jwt decoder service.
   * @param \Drupal\culturefeed_jwt\JwtTokenProvider $jwt_token_provider
   *   The jwt token provider.
   * @param \Symfony\Component\HttpFoundation\RequestStack $request_stack
   *   The request stack.
   */
  public function __construct(
      AccountInterface $account,
      UserMapInterface $user_map,
      EntityTypeManagerInterface $entity_type_manager,
      QueryFactory $query_factory,
      JwtDecoderServiceInterface $jwt_decoder_service,
      JwtTokenProvider $jwt_token_provider,
      RequestStack $request_stack
  ) {

    $this->account = $account;
    $this->userMap = $user_map;
    $this->entityStorage = $entity_type_manager->getStorage('culturefeed_jwt_token');
    $this->queryFactory = $query_factory;
    $this->jwtDecoderService = $jwt_decoder_service;
    $this->jwtTokenProvider = $jwt_token_provider;
    $this->requestStack = $request_stack;

  }

  /**
   * {@inheritdoc}
   */
  public function get() {

    $cf_user = new \CultureFeed_User();

    // First check for a token in the request (authorization header).
    $request = $this->requestStack->getCurrentRequest();
    $token = $this->jwtTokenProvider->getFromRequest($request);
    if ($token) {
      $token_string = $token->getCredentials();
    }

    // Check if we have a stored token.
    if (empty($token_string)) {

      $uitid = $this->userMap->getCultureFeedId($this->account->id());
      $query = $this->queryFactory->get('culturefeed_jwt_token')
        ->condition('uitid', $uitid);
      $result = $query->execute();

      if (count($result)) {
        /* @var \Drupal\culturefeed_jwt\Entity\JwtTokenEntity $entity */
        $entity = $this->entityStorage->load(key($result));
        $token_string = $entity->get('token')->value;
      }

    }

    if (!empty($token_string)) {

      try {

        $jwt = $this->jwtDecoderService->parse(new StringLiteral($token_string));
        $token = new JwtUserToken($jwt);
        $credentials = $token->getCredentials();

        $cf_user->id = $credentials->getClaim('uid');
        $cf_user->nick = $credentials->getClaim('nick');
        $cf_user->mbox = $credentials->getClaim('email');

      }
      catch (\Exception $e) {
        watchdog_exception('culturefeed_jwt', $e);
      }

    }

    return $cf_user;

  }

}
