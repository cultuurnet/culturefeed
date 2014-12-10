<?php

/**
 * @file
 * Contains \Drupal\culturefeed_udb3\Entity\DomainMessageEntity.
 */

namespace Drupal\culturefeed_udb3\Entity;

use Drupal\culturefeed_udb3\DomainMessageEntity;

/**
 * Defines the culturefeed udb3 event store.
 *
 * @ContentEntityType(
 *   id = "event_store",
 *   label = @Translation("Culturefeed udb3 event store"),
 *   base_table = "culturefeed_udb3_event_store",
 *   handlers = {
 *     "storage_schema" = "Drupal\culturefeed_udb3\DomainMessageStorageSchema",
 *   },
 *   entity_keys = {
 *     "id" = "dmid",
 *   },
 *   fieldable = FALSE,
 * )
 */
class EventStoreEntity extends DomainMessageEntity {
}
