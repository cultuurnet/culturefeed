<?php

/**
 * @file
 * Contains Drupal\culturefeed\CultureFeedFactory.
 */

namespace Drupal\culturefeed;

use CultureFeed;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\Entity\EntityManagerInterface;
use Drupal\Core\Entity\Query\QueryFactory;

class CultureFeedFactory implements CultureFeedFactoryInterface {

  /**
   * The OAuth client factory.
   *
   * @var \Drupal\culturefeed\OAuthClientFactory;
   */
  protected $oauthClient;

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
   * The entity manager.
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
   * Constructs a culturefeed instance.
   *
   * @param OAuthClientFactoryInterface $oauth_client
   *   The oauth client.
   * @param AccountInterface $account
   *   The account interface.
   * @param UserMapInterface $user_map
   *   The user map.
   * @param EntityManagerInterface $entity_manager
   *   The entity manger.
   * @param QueryFactory $entity_query
   *   The query factory.
   */
  public function __construct(OAuthClientFactoryInterface $oauth_client, AccountInterface $account, UserMapInterface $user_map, EntityManagerInterface $entity_manager, QueryFactory $entity_query) {

    $this->oauthClient = $oauth_client;
    $this->account = $account;
    $this->userMap = $user_map;
    $this->entityManager = $entity_manager;
    $this->entityQuery = $entity_query;

  }

  /**
   * {@inheritdoc}
   */
  public function create($token = NULL, $secret = NULL) {
    $oauth_client = $this->oauthClient->create($token, $secret);
    return new CultureFeed($oauth_client);
  }

  /**
   * {@inheritdoc}
   */
  public function createAuthenticated() {

    $uitid = $this->userMap->getCulturefeedId($this->account->id());
    $result = $this->entityQuery->get('culturefeed_token')
      ->condition('uitid', $uitid)
      ->execute();

    if (!empty($result)) {
      $data = $this->entityManager->getStorage('culturefeed_token')
        ->load(reset($result));
      return $this->create($data->token->value, $data->secret->value);
    }
    return $this->create();

  }

}
