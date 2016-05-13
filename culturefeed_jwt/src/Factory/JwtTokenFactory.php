<?php

namespace Drupal\culturefeed_jwt\Factory;

use CultuurNet\SymfonySecurityJwt\Authentication\JwtUserToken;
use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\Entity\Query\QueryFactory;
use Drupal\Core\Session\AccountInterface;
use Drupal\culturefeed\UserMapInterface;

/**
 * Class JwtTokenFactory.
 *
 * @package Drupal\culturefeed_jwt\Factory
 */
class JwtTokenFactory {

  /**
   * The account.
   *
   * @var \Drupal\Core\Session\AccountInterface
   */
  protected $account;

  /**
   * The entity storage.
   *
   * @var \Drupal\Core\Entity\EntityStorageInterface
   */
  protected $entityStorage;

  /**
   * The query factory.
   *
   * @var \Drupal\Core\Entity\Query\QueryFactory
   */
  protected $queryFactory;

  /**
   * The user map.
   *
   * @var \Drupal\culturefeed\UserMapInterface
   */
  protected $userMap;

  /**
   * JwtTokenFactory constructor.
   *
   * @param \Drupal\Core\Session\AccountInterface $account
   *   The account.
   * @param \Drupal\culturefeed\UserMapInterface $user_map
   *   The user map.
   * @param \Drupal\Core\Entity\Query\QueryFactory $query_factory
   *   The query factory.
   * @param \Drupal\Core\Entity\EntityTypeManagerInterface $entity_type_manager
   *   The entity type manager.
   */
  public function __construct(
      AccountInterface $account,
      UserMapInterface $user_map,
      QueryFactory $query_factory,
      EntityTypeManagerInterface $entity_type_manager
  ) {

    $this->account = $account;
    $this->userMap = $user_map;
    $this->queryFactory = $query_factory;
    $this->entityStorage = $entity_type_manager->getStorage('culturefeed_jwt_token');

  }

  /**
   * Get the token.
   *
   * @return \CultuurNet\SymfonySecurityJwt\Authentication\JwtUserToken
   *   The token.
   */
  public function get() {

    $cf_user_id = $this->userMap->getCultureFeedId($this->account->id());
    $query = $this->queryFactory->get('culturefeed_jwt_token')->condition('uitid', $cf_user_id);
    $result = $query->execute();
    $entity = $this->entityStorage->load(key($result));
    return new JwtUserToken($entity->get('field_token')->value());

  }

}
