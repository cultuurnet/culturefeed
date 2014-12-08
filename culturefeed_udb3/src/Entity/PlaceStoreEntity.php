<?php

/**
 * @file
 * Contains \Drupal\culturefeed_udb3\Entity\PlaceStoreEntity.
 */

namespace Drupal\culturefeed_udb3\Entity;

use Drupal\culturefeed_udb3\DomainMessageEntity;

/**
 * Defines the culturefeed user entity.
 *
 * @ContentEntityType(
 *   id = "place_store",
 *   label = @Translation("Culturefeed udb3 place store"),
 *   base_table = "culturefeed_udb3_place_store",
 *   handlers = {
 *     "storage_schema" = "Drupal\culturefeed_udb3\DomainMessageStorageSchema",
 *   },
 *   entity_keys = {
 *     "id" = "dmid",
 *   },
 *   fieldable = FALSE,
 * )
 */
class PlaceStoreEntity extends DomainMessageEntity {
}
