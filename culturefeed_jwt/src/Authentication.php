<?php

namespace Drupal\culturefeed_jwt;

use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\Entity\Query\QueryFactory;
use Drupal\culturefeed\UserMapInterface;
use Exception;
use Symfony\Component\HttpFoundation\Request;

/**
 * Class Authentication.
 *
 * @package Drupal\culturefeed_jwt
 */
class Authentication implements AuthenticationInterface {

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
   * The token provider.
   *
   * @var \Drupal\culturefeed_jwt\JwtTokenProvider
   */
  protected $tokenProvider;

  /**
   * The user map.
   *
   * @var \Drupal\culturefeed\UserMapInterface
   */
  protected $userMap;

  /**
   * Authentication constructor.
   *
   * @param \Drupal\culturefeed_jwt\JwtTokenProvider $token_provider
   *   The token provider.
   * @param \Drupal\culturefeed\UserMapInterface $user_map
   *   The user map.
   * @param EntityTypeManagerInterface $entity_type_manager
   *   The entity type manger.
   * @param QueryFactory $entity_query
   *   The query factory.
   */
  public function __construct(
      JwtTokenProvider $token_provider,
      UserMapInterface $user_map,
      EntityTypeManagerInterface $entity_type_manager,
      QueryFactory $entity_query
  ) {

    $this->tokenProvider = $token_provider;
    $this->userMap = $user_map;
    $this->entityTypeManager = $entity_type_manager;
    $this->entityQuery = $entity_query;

  }

  /**
   * {@inheritdoc}
   */
  public function authorize(Request $request) {

    try {

      $jwt = $this->tokenProvider->getFromRequest($request);
      $credentials = $jwt->getCredentials();

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

    $storage = $this->entityTypeManager->getStorage('culturefeed_jwt_token');

    $query = $this->entityQuery->get('culturefeed_jwt_token')->condition('uitid', $cf_user->id);
    $result = $query->execute();
    $entities = $storage->loadMultiple(array_keys($result));
    $storage->delete($entities);

    $storage->create(array(
      'uitid' => $cf_user->id,
      'token' => (string) $credentials,
    ))->save();

    if ($account) {
      user_login_finalize($account);
    }

  }

}
