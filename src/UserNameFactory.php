<?php

/**
 * @file
 * Contains Drupal\culturefeed\UserNameFactory.
 */

namespace Drupal\culturefeed;

use Drupal\Core\Session\AccountInterface;
use Drupal\Core\Entity\Query\QueryFactory;

/**
 * Class UserNameFactory.
 *
 * @package Drupal\culturefeed
 */
class UserNameFactory implements UserNameFactoryInterface {

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
   * Constructs a UserNameFactory object.
   *
   * @param QueryFactory $entity_query
   *   The query factory.
   * @param AccountInterface $account
   *   The account interface.
   */
  public function __construct(QueryFactory $entity_query, AccountInterface $account) {

    $this->entityQuery = $entity_query;
    $this->account = $account;

  }

  /**
   * {@inheritdoc}
   */
  public function create($nick) {

    $count = 0;
    $name = $nick;
    while ($this->check($name)) {
      $count++;
      $name = $nick . '_' . $count;
    }
    return $name;

  }

  /**
   * Checks if a name exists.
   *
   * @param string $name
   *   The name.
   *
   * @return bool
   *   True if the name is found, false if not.
   */
  private function check($name) {

    $result = $this->entityQuery->get('user')->condition('name', $name)->execute();
    return (count($result)) ? TRUE : FALSE;

  }

}
