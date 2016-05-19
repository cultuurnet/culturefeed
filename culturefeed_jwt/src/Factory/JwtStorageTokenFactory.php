<?php

namespace Drupal\culturefeed_jwt\Factory;

use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\Entity\Query\QueryFactory;
use Drupal\Core\Session\AccountInterface;
use Drupal\culturefeed\UserMapInterface;

/**
 * Class JwtStorageTokenFactory.
 *
 * @package Drupal\culturefeed_jwt\Factory
 */
class JwtStorageTokenFactory implements JwtTokenFactoryInterface {

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
   * The user map service.
   *
   * @var \Drupal\culturefeed\UserMapInterface.
   */
  protected $userMap;

  /**
   * JwtStorageTokenFactory constructor.
   *
   * @param \Drupal\Core\Session\AccountInterface $account
   *   The account.
   * @param \Drupal\culturefeed\UserMapInterface $user_map
   *   The user map service.
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
   * @return string
   *   The token.
   */
  public function get() {

    $token_string = '';
    $uit_id = $this->userMap->getCultureFeedId($this->account->id());
    $query = $this->queryFactory->get('culturefeed_jwt_token')->condition('uitid', $uit_id);
    $result = $query->execute();

    if (count($result)) {
      /* @var \Drupal\culturefeed_jwt\Entity\JwtTokenEntity $entity */
      $entity = $this->entityStorage->load(key($result));
      $token_string = $entity->get('token')->value;
    }

    return $token_string;

  }

}
