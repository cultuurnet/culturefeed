<?php

/**
 * @file
 * Contains Drupal\culturefeed_udb3\EventStore.
 */

namespace Drupal\culturefeed_udb3;

use Broadway\EventStore\EventStoreInterface;
use Drupal\Core\Entity\EntityManagerInterface;
use Drupal\Core\Entity\Query\QueryFactory;
use Broadway\Domain\DomainEventStreamInterface;

class EventStore implements EventStoreInterface {

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
   * Constructs the event store.
   *
   * @param EntityManagerInterface $entity_manager
   *   The entity manger.
   * @param QueryFactory $entity_query
   *   The query factory.
   */
  public function __construct(EntityManagerInterface $entity_manager, QueryFactory $entity_query) {

    $this->entityManager = $entity_manager;
    $this->entityQuery = $entity_query;

  }

  /**
   * {@inheritdoc}
   */
  public function load($id) {
  }

  /**
   * {@inheritdoc}
   */
  public function append($id, DomainEventStreamInterface $event_stream) {
  }

}
