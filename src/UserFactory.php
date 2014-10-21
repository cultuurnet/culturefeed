<?php

/**
 * @file
 * Contains Drupal\culturefeed\UserFactory.
 */

namespace Drupal\culturefeed;

use Drupal\Core\Session\AccountInterface;
use Drupal\Core\Entity\EntityManagerInterface;
use Drupal\Core\Entity\Query\QueryFactory;

class UserFactory implements UserFactoryInterface {

  /**
   * The culturefeed instance.
   *
   * @var \Drupal\culturefeed\CultureFeedFactoryInterface;
   */
  protected $instance;

  /**
   * The account.
   *
   * @var \Drupal\Core\Session\AccountInterface;
   */
  protected $account;

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
   * Constructs a CultureFeed user object.
   *
   * @param CultureFeedFactoryInterface $instance
   *   The culturefeed instance.
   * @param AccountInterface $account
   *   The account interface.
   * @param EntityManagerInterface $entity_manager
   *   The entity manger.
   * @param QueryFactory $entity_query
   *   The query factory.
   */
  public function __construct(CultureFeedFactoryInterface $instance, AccountInterface $account, EntityManagerInterface $entity_manager, QueryFactory $entity_query) {

    $this->instance = $instance;
    $this->account = $account;
    $this->entityManager = $entity_manager;
    $this->entityQuery = $entity_query;

  }

  /**
   * {@inheritdoc}
   */
  public function get() {

    // Get the uitid.
    $result = $this->entityQuery->get('culturefeed_user')->condition('uid', $this->account->id())->execute();
    $uitid = $this->entityManager->getStorage('culturefeed_user')->load(reset($result))->uitid->value;

    if ($uitid) {

      // Get the uitid data.
      $result = $this->entityQuery->get('culturefeed_token')->condition('uitid', $uitid)->execute();
      $data = $this->entityManager->getStorage('culturefeed_token')->load(reset($result));
      $instance = $this->instance->create($data->token, $data->secret);
      $user = $instance->getUser($uitid);

      if ($user) {
        return $user;
      }

    }

  }

}
