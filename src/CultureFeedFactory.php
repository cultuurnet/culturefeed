<?php

/**
 * @file
 * Contains Drupal\culturefeed\CultureFeedFactory.
 */

namespace Drupal\culturefeed;

use CultureFeed;
use Drupal\Core\Config\ConfigFactory;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\Entity\EntityManagerInterface;
use Drupal\Core\Entity\Query\QueryFactory;
use Psr\Log\LoggerInterface;

class CultureFeedFactory implements CultureFeedFactoryInterface {

  /**
   * The OAuth client factory.
   *
   * @var \Drupal\culturefeed\OAuthClientFactory;
   */
  protected $oauthClient;

  /**
   * The config factory.
   *
   * @var \Drupal\Core\Config\ConfigFactory;
   */
  protected $config;

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
   * A logger instance.
   *
   * @var \Psr\Log\LoggerInterface
   */
  protected $logger;

  /**
   * Constructs a culturefeed instance.
   *
   * @param OAuthClientFactoryInterface $oauth_client
   *   The oauth client.
   * @param ConfigFactory $config_factory
   *   The config factory.
   * @param AccountInterface $account
   *   The account interface.
   * @param UserMapInterface $user_map
   *   The user map.
   * @param EntityManagerInterface $entity_manager
   *   The entity manger.
   * @param QueryFactory $entity_query
   *   The query factory.
   * @param \Psr\Log\LoggerInterface $logger
   *   A logger instance.
   */
  public function __construct(
    OAuthClientFactoryInterface $oauth_client,
    ConfigFactory $config_factory,
    AccountInterface $account,
    UserMapInterface $user_map,
    EntityManagerInterface $entity_manager,
    QueryFactory $entity_query,
    LoggerInterface $logger
  ) {

    $this->oauthClient = $oauth_client;
    $this->config = $config_factory->get('culturefeed.api');
    $this->account = $account;
    $this->userMap = $user_map;
    $this->entityManager = $entity_manager;
    $this->entityQuery = $entity_query;
    $this->logger = $logger;

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
      ->condition('application_key', $this->config->get('application_key'))
      ->execute();

    if (!empty($result)) {
      $data = $this->entityManager->getStorage('culturefeed_token')
        ->load(reset($result));

      try {
        return $this->create($data->token->value, $data->secret->value);
      }
      catch (\Exception $e) {
        $this->logger->error('No authenticated instance could be created.');
      }

    }

    return $this->create();

  }

}
