<?php

namespace Drupal\culturefeed\Service;

use Drupal\Core\Config\ConfigFactoryInterface;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\Entity\Query\QueryFactory;

class UserName {

  /**
   * Constructs a UserService object.
   */
  public function __construct(ConfigFactoryInterface $config, AccountInterface $account, QueryFactory $entity_query) {

    $this->config = $config->get('culturefeed.api');
    $this->account = $account;
    $this->entityQuery = $entity_query;

  }

  function uniqueUsername($nick) {

    $count = 0;
    $name = $nick;
    while ($this->checkName($name)) {
      $count++;
      $name = $name . '_' . $count;
    }
    return $name;

  }

  function checkName($name) {
    $result = $this->entityQuery->get('user')->condition('name', $name)->execute();
    return (count($result)) ? TRUE : FALSE;
  }

}
