<?php

/**
 * @file
 * Contains \Drupal\culturefeed_udb3\Entity\DomainMessageEntity.
 */

namespace Drupal\culturefeed_udb3\Entity;

use Drupal\culturefeed_udb3\DomainMessageEntity;

/**
 * Defines the culturefeed user entity.
 *
 * @ContentEntityType(
 *   id = "used_keywords_memory_event_store",
 *   label = @Translation("Culturefeed udb3 used keywords memory event store"),
 *   base_table = "culturefeed_udb3_used_keywords_memory_event_store",
 *   handlers = {
 *     "storage_schema" = "Drupal\culturefeed_udb3\DomainMessageStorageSchema",
 *   },
 *   entity_keys = {
 *     "id" = "dmid",
 *   },
 *   fieldable = FALSE,
 * )
 */
class UsedKeywordsMemoryEventStoreEntity extends DomainMessageEntity {
}
