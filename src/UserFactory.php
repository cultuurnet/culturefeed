<?php

/**
 * @file
 * Contains Drupal\culturefeed\UserFactory.
 */

namespace Drupal\culturefeed;

use Drupal\Core\Session\AccountInterface;
use CultureFeed;
use CultureFeed_User;

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
   * The user map.
   *
   * @var string;
   */
  protected $userMap;

  /**
   * Constructs a CultureFeed user object.
   *
   * @param CultureFeed $instance
   *   The authenticated culturefeed instance.
   * @param AccountInterface $account
   *   The account interface.
   * @param UserMapInterface $user_map
   *   The user map.
   */
  public function __construct(CultureFeed $instance, AccountInterface $account, UserMapInterface $user_map) {

    $this->instance = $instance;
    $this->account = $account;
    $this->userMap = $user_map;

  }

  /**
   * {@inheritdoc}
   */
  public function get() {

    try {
      $uitid = $this->userMap->getCulturefeedId($this->account->id);
      return $this->instance->getUser($uitid);
    }
    catch (\Exception $e) {
      return new CultureFeed_User();
    }

  }

}
