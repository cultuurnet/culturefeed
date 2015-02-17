<?php

/**
 * @file
 * Contains Drupal\culturefeed\Access\User.
 */

namespace Drupal\culturefeed\Access;

use CultureFeed_User;
use Drupal\Core\Access\AccessResult;
use Drupal\Core\Routing\Access\AccessInterface;
use Drupal\Core\Session\AccountInterface;
use Symfony\Component\Routing\Route;

/**
 * Class UserAccess.
 *
 * @package Drupal\culturefeed\Access
 */
class UserAccess implements AccessInterface {

  /**
   * The culturefeed user.
   *
   * @var \CultureFeed_User;
   */

  /**
   * Constructs the user access object.
   *
   * @param CultureFeed_User $user
   *   The culturefeed user.
   */
  public function __construct(CultureFeed_User $user) {
    $this->user = $user;
  }

  /**
   * Checks access based on Culturefeed user.
   */
  public function access(AccountInterface $account, Route $route) {

    $required_status = filter_var($route->getRequirement('_culturefeed_current_user'), FILTER_VALIDATE_BOOLEAN);
    $actual_status = !empty($this->user->id);

    return AccessResult::allowedIf($required_status === $actual_status)->setCacheable(FALSE);

  }

}
