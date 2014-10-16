<?php

/**
 * @file
 * Contains Drupal\culturefeed\UserMap.
 */

namespace Drupal\culturefeed;

use Drupal\Core\Entity\EntityManagerInterface;
use Drupal\Core\Entity\Query\QueryFactory;
use Drupal\Core\Session\AccountInterface;
use CultureFeed_User;
use Exception;

class UserMap implements UserMapInterface {

  /**
   * The entity Manager.
   *
   * @var \Drupal\Core\Entity\EntityManagerInterface;
   */
  protected $entityManager;

  /**
   * The query factory.
   *
   * @var \Drupal\Core\Entity\Query\QueryFactory;
   */
  protected $entityQuery;

  /**
   * The account.
   *
   * @var \Drupal\Core\Session\AccountInterface;
   */
  protected $account;

  /**
   * The user name factory.
   *
   * @var \Drupal\culturefeed\UserNameFactoryInterface;
   */
  protected $userNameFactory;

  /**
   * Constructs a UserMap object.
   *
   * @param EntityManagerInterface $entity_manager
   *   The entity manger.
   * @param QueryFactory $entity_query
   *   The query factory.
   * @param AccountInterface $account
   *   The account interface.
   * @param UserNameFactoryInterface $user_name_factory
   *   The user name factory.
   */
  public function __construct(EntityManagerInterface $entity_manager, QueryFactory $entity_query, AccountInterface $account, UserNameFactoryInterface $user_name_factory) {

    $this->entityManager = $entity_manager;
    $this->entityQuery = $entity_query;
    $this->account = $account;
    $this->userNameFactory = $user_name_factory;

  }

  /**
   * {@inheritdoc}
   */
  public function get(CultureFeed_User $user, array $token) {

    // Check if the user is already known in our system.
    $query = $this->entityQuery->get('culturefeed_user')->condition('uitid', $token['userId']);
    $result = $query->execute();
    $uid = key($result);

    if (!$uid) {
      $account = $this->create($user);
    }
    else {
      $account = $this->entityManager->getStorage('user')->load($uid);
    }

    return $account;

  }

  /**
   * Creates a new user.
   *
   * @param CultureFeed_User $user
   *   The culturefeed user.
   *
   * @return \Drupal\Core\Entity\EntityInterface
   *   A user object.
   *
   * @throws \Exception
   *   An exception.
   */
  public function create(CultureFeed_User $user) {

    try {

      // If no CultureFeed user was passed, we can't create the user.
      if (!$user || empty($user->nick)) {
        return FALSE;
      }

      $storage = $this->entityManager->getStorage('user');
      $account = $storage->create(array(
        'name' => $this->userNameFactory->create($user->nick),
        'status' => 1,
      ));
      $account->save();

      // Save the mapping between CultureFeed User ID and Drupal user id.
      $storage = $this->entityManager->getStorage('culturefeed_user');
      $storage->create(array(
        'uid' => $account->id(),
        'uitid' => $user->id,
      ))->save();

    }
    catch (Exception $e) {
      throw $e;
    }

    return $account;

  }

}
