<?php

/**
 * @file
 * Contains \Drupal\culturefeed_udb3\Entity\DocumentRepositoryEntity.
 */

namespace Drupal\culturefeed_udb3;

use Drupal\Core\Entity\ContentEntityBase;
use Drupal\Core\Entity\ContentEntityInterface;
use Drupal\Core\Entity\EntityTypeInterface;
use Drupal\Core\Field\BaseFieldDefinition;

/**
 * Class DomainMessageEntity.
 *
 * @package Drupal\culturefeed_udb3
 */
class DocumentRepositoryEntity extends ContentEntityBase implements ContentEntityInterface {

  /**
   * {@inheritdoc}
   */
  public static function baseFieldDefinitions(EntityTypeInterface $entity_type) {

    $fields['id'] = BaseFieldDefinition::create('uuid')
      ->setLabel(t('ID'))
      ->setDescription(t('ID.'))
      ->setReadOnly(TRUE);

    $fields['body'] = BaseFieldDefinition::create('string_long')
      ->setLabel(t('UUID'))
      ->setDescription(t('UUID.'));

    return $fields;

  }

}
