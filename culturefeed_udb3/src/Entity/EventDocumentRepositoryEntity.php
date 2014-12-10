<?php

/**
 * @file
 * Contains \Drupal\culturefeed_udb3\Entity\EventDocumentRepositoryEntity.
 */

namespace Drupal\culturefeed_udb3\Entity;

use Drupal\culturefeed_udb3\DocumentRepositoryEntity;

/**
 * Defines the culturefeed udb3 event document repository entity.
 *
 * @ContentEntityType(
 *   id = "event_document_repository",
 *   label = @Translation("Culturefeed udb3 event document repository"),
 *   base_table = "culturefeed_udb3_event_document_repository",
 *   entity_keys = {
 *     "id" = "id",
 *   },
 *   fieldable = FALSE,
 * )
 */
class EventDocumentRepositoryEntity extends DocumentRepositoryEntity {
}
