<?php

/**
 * @file
 * Contains \Drupal\culturefeed\Entity\CulturefeedUser.
 */

namespace Drupal\culturefeed\Entity;

use Drupal\Core\Entity\ContentEntityBase;
use Drupal\Core\Entity\ContentEntityInterface;
use Drupal\Core\Entity\EntityTypeInterface;
use Drupal\Core\Field\BaseFieldDefinition;

/**
 *
 * Defines the culturefeed user entity.
 *
 * @ContentEntityType(
 *   id = "culturefeed_user",
 *   label = @Translation("Culturefeed user"),
 *   base_table = "culturefeed_user",
 *   fieldable = FALSE,
 *   entity_keys = {
 *     "id" = "uid",
 *   },
 * )
 */
class CulturefeedUser extends ContentEntityBase implements ContentEntityInterface {

  /**
   * {@inheritdoc}
   */
  public static function baseFieldDefinitions(EntityTypeInterface $entity_type) {

    $fields['uid'] = BaseFieldDefinition::create('entity_reference')
      ->setLabel(t('Drupal user id'))
      ->setDescription(t('Drupal user id.'))
      ->setSetting('target_type', 'user');

    $fields['uitid'] = BaseFieldDefinition::create('uuid')
      ->setLabel(t('UiT ID'))
      ->setDescription(t('UiT ID.'));

    return $fields;

  }

}
