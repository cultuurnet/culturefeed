<?php

/**
 * @file
 * Contains \Drupal\culturefeed_udb3\DomainMessageStorageSchema.
 */

namespace Drupal\culturefeed_udb3;

use Drupal\Core\Entity\Sql\SqlContentEntityStorageSchema;
use Drupal\Core\Entity\ContentEntityTypeInterface;

/**
 * Defines the domain message schema handler.
 */
class DomainMessageStorageSchema extends SqlContentEntityStorageSchema {

  /**
   * {@inheritdoc}
   */
  protected function getEntitySchema(ContentEntityTypeInterface $entity_type, $reset = FALSE) {

    $schema = parent::getEntitySchema($entity_type, $reset);

    $schema['culturefeed_domain_message']['unique keys'] += array(
      'id' => array('uuid', 'playhead'),
    );

    return $schema;
  }

}
