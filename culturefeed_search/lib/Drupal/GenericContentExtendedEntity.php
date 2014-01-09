<?php
/**
 * @file
 * GenericContentExtendedEntity extends the normal activityEntityStats.
 */
use \CultuurNet\Search\ActivityStatsExtendedEntity;

/**
 * GenericContentExtendedEntity
 */
class GenericContentExtendedEntity extends ActivityStatsExtendedEntity {

  /**
   * Constructor to load a wrapper around a drupal entity.
   * @param stdClass $entity
   */
  public function __construct($entity) {
    $this->entity = $entity;
  }

  /**
   * Get the unique identifier of element.
   */
  public function getId() {
    return $this->getEntity()->id;
  }
  
  /**
   * Get the type of element.
   */
  public function getType() {
    return 'content';
  }
  
  /**
   * Overrides ActivityStatsExtendedEntity::getTitle().
   *
   * @param String $langcode
   */
  public function getTitle($langcode = NULL) {
    return $this->getEntity()->title;
  }

  /**
   * @param string $activityType
   */
  public function getActivityCount($activityType) {
    return parent::getActivityCount($activityType);
  }

  /**
   * @param SimpleXMLElement $xmlElement
   */
  public static function fromXml(SimpleXMLElement $xmlElement) {
    return NULL;
  }

  /**
   * @param stdClass $entity
   */
  public static function createFrom($entity) {

    $extendedEntity = new static($entity);
    $extendedEntity->type = 'content';
    $extendedEntity->id = $entity->id;
    
    return $extendedEntity;
  }

  /**
   * Load the activity counts for drupal nodes.
   */
  public function loadActivityCounts() {

  }

}
