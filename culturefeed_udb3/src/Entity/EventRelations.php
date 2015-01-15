<?php
/**
 * @file
 */

namespace Drupal\culturefeed_udb3\Entity;


use Drupal\Core\Entity\ContentEntityBase;
use Drupal\Core\Entity\ContentEntityInterface;
use Drupal\Core\Entity\EntityTypeInterface;
use Drupal\Core\Field\BaseFieldDefinition;
use Drupal\Core\Entity\Annotation\ContentEntityType;
use Drupal\Core\Annotation\Translation;

/**
 * @ContentEntityType(
 *  id = "event_relations",
 *  label = @Translation("Culturefeed udb3 event relations"),
 *  base_table = "culturefeed_udb3_event_relations",
 *  entity_keys = {
 *    "id" = "event",
 *  },
 *  fieldable = FALSE,
 * )
 * @package Drupal\culturefeed_udb3\Entity
 */
class EventRelations extends ContentEntityBase implements ContentEntityInterface {

  /**
   * {@inheritdoc}
   */
  public static function baseFieldDefinitions(EntityTypeInterface $entity_type) {

    $fields['event'] = BaseFieldDefinition::create('string')
      ->setLabel(t('Event UUID'))
      ->setDescription(t('Event UUID.'));

    $fields['organizer'] = BaseFieldDefinition::create('string')
      ->setLabel(t('Organizer UUID'))
      ->setDescription(t('Organizer UUID.'));

    $fields['place'] = BaseFieldDefinition::create('string')
      ->setLabel(t('Place UUID'))
      ->setDescription(t('Place UUID.'));

    return $fields;
  }
}
