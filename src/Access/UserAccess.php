<?php

/**
 * @file
 * Contains Drupal\culturefeed\Access\User.
 */

namespace Drupal\culturefeed\Access;

use Drupal\Core\Routing\Access\AccessInterface;
use CultureFeed_User;
use Drupal\Core\Access\AccessResult;

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
  public function access() {
    return AccessResult::allowedIf($this->user->id)->setCacheable(FALSE);
  }

}
