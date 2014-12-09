<?php

/**
 * @file
 * Contains \Drupal\culturefeed_udb3\Entity\OrganizerStoreEntity.
 */

namespace Drupal\culturefeed_udb3\Entity;

use Drupal\culturefeed_udb3\DomainMessageEntity;

/**
 * Defines the culturefeed user entity.
 *
 * @ContentEntityType(
 *   id = "organizer_store",
 *   label = @Translation("Culturefeed udb3 organizer store"),
 *   base_table = "culturefeed_udb3_organizer_store",
 *   handlers = {
 *     "storage_schema" = "Drupal\culturefeed_udb3\DomainMessageStorageSchema",
 *   },
 *   entity_keys = {
 *     "id" = "dmid",
 *   },
 *   fieldable = FALSE,
 * )
 */
class OrganizerStoreEntity extends DomainMessageEntity {
}
