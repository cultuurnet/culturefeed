<?php

namespace Drupal\culturefeed\Service;

use Drupal\Core\Config\ConfigFactoryInterface;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\Entity\Query\QueryFactory;
use Drupal\culturefeed\Service\UserName;

class UserService {

  /**
   * Constructs a UserService object.
   */
  public function __construct(ConfigFactoryInterface $config, AccountInterface $account, QueryFactory $entity_query, UserName $user_name) {

    $this->config = $config->get('culturefeed.api');
    $this->account = $account;
    $this->entityQuery = $entity_query;
    $this->userName = $user_name;

  }

  public function setUser($uitid_account, $token) {

    // Check if the user is already known in our system.
    $query = \Drupal::entityQuery('culturefeed_user')
      ->condition('uitid', $token['userId']);
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
          'name' => $this->userName->uniqueUsername($uitid_account->nick),
          'status' => 1,
        ));
        $account->save();
      }

      // Save the mapping between CultureFeed User ID and Drupal user id.
      entity_create('culturefeed_user', array(
        'uid' => $account->id(),
        'uitid' => $uitid_account->id,
      ))->save();

    }
    catch (Exception $e) {
      throw $e;
    }

    return $account;

  }

}
