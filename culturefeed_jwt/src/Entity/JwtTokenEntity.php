<?php

namespace Drupal\culturefeed_jwt\Entity;

use Drupal\Core\Entity\ContentEntityBase;
use Drupal\Core\Entity\ContentEntityInterface;
use Drupal\Core\Entity\EntityTypeInterface;
use Drupal\Core\Field\BaseFieldDefinition;

/**
 * Defines the culturefeed jwt token entity.
 *
 * @ContentEntityType(
 *   id = "culturefeed_jwt_token",
 *   label = @Translation("Culturefeed jwt token"),
 *   base_table = "culturefeed_jwt_token",
 *   fieldable = FALSE,
 *   entity_keys = {
 *     "id" = "uitid",
 *   },
 * )
 */
class JwtTokenEntity extends ContentEntityBase implements ContentEntityInterface {

  /**
   * {@inheritdoc}
   */
  public static function baseFieldDefinitions(EntityTypeInterface $entity_type) {

    $fields['uitid'] = BaseFieldDefinition::create('uuid')
      ->setLabel(t('UiT ID'))
      ->setDescription(t('UiT ID.'));

    $fields['token'] = BaseFieldDefinition::create('string_long')
      ->setLabel(t('CultureFeed jwt token'))
      ->setDescription(t('CultureFeed jwt token.'));

    return $fields;

  }

}
