<?php

namespace Drupal\culturefeed\Service;

use Drupal\Core\Config\ConfigFactoryInterface;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\Extension\ModuleHandlerInterface;
use Drupal\Core\Routing\UrlGeneratorInterface;
use Drupal\user\Entity\User;

class UserService {

  /**
   * Constructs a UserService object.
   */
  public function __construct(ConfigFactoryInterface $config, AccountInterface $account, ModuleHandlerInterface $module_handler, UrlGeneratorInterface $urlGenerator) {

    $this->config = $config->get('culturefeed.api');
    $this->account = $account;
    $this->moduleHandler = $module_handler;
    $this->urlGenerator = $urlGenerator;

  }

  public function setUser($uitid_account, $token) {

    // Check if the user is already known in our system.
    $query = \Drupal::entityQuery('user')
      ->condition('name', $token['userId']);
    $result = $query->execute();
    $uid = key($result);

    if (!$uid) {
      $account = $this->createUser($uitid_account);
    }
    else {
      $account = entity_load('user', $uid);
    }

    return $account;

  }

  public function createUser($uitid_account) {

    try {

      // If no CultureFeed user was passed, we can't create the user.
      if (!$uitid_account || empty($uitid_account->nick)) {
        return FALSE;
      }

      if (isset($this->account->uid) && $this->account->uid) {
        $account = $this->account;
      }
      else {
        $account = entity_create('user', array(
          'name' => $uitid_account->id,
          'status' => 1,
        ));
        $account->save();
      }

      // Save the mapping between CultureFeed User ID and Drupal user id.
      db_insert('culturefeed_user')
        ->fields(array(
          'uid' => $account->id(),
          'cf_uid' => $uitid_account->id,
        ))
        ->execute();

    }
    catch (Exception $e) {
      throw $e;
    }

    return $account;

  }

}
