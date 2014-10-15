<?php

/**
 * @file
 * Contains \Drupal\culturefeed\Entity\CulturefeedToken.
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
 *   id = "culturefeed_token",
 *   label = @Translation("Culturefeed token"),
 *   base_table = "culturefeed_token",
 *   fieldable = FALSE,
 *   entity_keys = {
 *     "id" = "uitid",
 *   },
 * )
 */
class CulturefeedToken extends ContentEntityBase implements ContentEntityInterface {

  /**
   * {@inheritdoc}
   */
  public static function baseFieldDefinitions(EntityTypeInterface $entity_type) {

    $fields['uitid'] = BaseFieldDefinition::create('uuid')
      ->setLabel(t('UiT ID'))
      ->setDescription(t('UiT ID.'));

    $fields['application_key'] = BaseFieldDefinition::create('string')
      ->setLabel(t('Application key'))
      ->setDescription(t('Application key.'));

    $fields['token'] = BaseFieldDefinition::create('string')
      ->setLabel(t('CultureFeed OAuth token'))
      ->setDescription(t('CultureFeed OAuth token.'));

    $fields['secret'] = BaseFieldDefinition::create('string')
      ->setLabel(t('CultureFeed OAuth secret'))
      ->setDescription(t('CultureFeed OAuth secret.'));

    return $fields;

  }

}
