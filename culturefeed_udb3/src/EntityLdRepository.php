<?php

/**
 * @file
 * Contains \Drupal\culturefeed_udb3\EntityLdRepository.
 */

namespace Drupal\culturefeed_udb3;

use CultuurNet\UDB3\Event\ReadModel\DocumentRepositoryInterface;
use Drupal\Core\Entity\EntityManagerInterface;
use CultuurNet\UDB3\Event\ReadModel\JsonDocument;

/**
 * Class EntityLdRepository.
 *
 * @package Drupal\culturefeed_udb3
 */
class EntityLdRepository implements DocumentRepositoryInterface {

  /**
   * The entity type.
   *
   * @var string
   */
  protected $entityType;

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
   * Constructs the entity ld repository.
   *
   * @param string $entity_type
   *   The entity type.
   * @param EntityManagerInterface $entity_manager
   *   The entity manager.
   */
  public function __construct($entity_type, EntityManagerInterface $entity_manager) {

    $this->entityType = $entity_type;
    $this->entityManager = $entity_manager->getStorage($entity_type);

  }

  /**
   * {@inheritdoc}
   */
  public function get($id) {

    $entity = $this->entityManager->load($id);
    if (isset($entity->body->value) && $entity->body->value) {
      return new JsonDocument($id, $entity->body->value);
    }

  }

  /**
   * {@inheritdoc}
   */
  public function save(JsonDocument $document) {

    $id = $document->getId();
    $entity = $this->entityManager->load($id);
    if ($entity) {
      $entity->body->value = $document->getRawBody();
      $entity->save();
    }
    else {
      $entity = $this->entityManager->create(array(
        'id' => $id,
        'body' => $document->getRawBody(),
      ));
    }
    $entity->save();

  }

}
