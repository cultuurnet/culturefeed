<?php

/**
 * @file
 * Contains \Drupal\culturefeed_udb3\Entity\PlaceDocumentRepositoryEntity.
 */

namespace Drupal\culturefeed_udb3\Entity;

use Drupal\culturefeed_udb3\DocumentRepositoryEntity;

/**
 * Defines the culturefeed udb3 event document repository entity.
 *
 * @ContentEntityType(
 *   id = "place_document_repository",
 *   label = @Translation("Culturefeed udb3 place document repository"),
 *   base_table = "culturefeed_udb3_place_document_repository",
 *   entity_keys = {
 *     "id" = "id",
 *   },
 *   fieldable = FALSE,
 * )
 */
class PlaceDocumentRepositoryEntity extends DocumentRepositoryEntity {
}
