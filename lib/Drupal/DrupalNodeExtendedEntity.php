<?php
/**
 * @file
 * DrupalNodeExtendedEntity extends the normal fake entity specific for drupal nodes.
 */
use \CultuurNet\Search\ActivityStatsExtendedEntity;

/**
 * DrupalNodeExtendedEntity
 */
class DrupalNodeExtendedEntity extends ActivityStatsExtendedEntity {

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
    return $this->getEntity()->nid;
  }
  
  /**
   * Get the type of element.
   */
  public function getType() {
    return $this->type;
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
  public static function fromDrupal($entity) {

    $extendedEntity = new static($entity);
    $extendedEntity->type = 'node';
    $extendedEntity->id = $entity->nid;
    
    return $extendedEntity;
  }

  /**
   * Load the activity counts for drupal nodes.
   */
  public function loadActivityCounts() {

    try {

      // Add the different activity counts.
      $query = new CultureFeed_SearchActivitiesQuery();
      $query->contentType = CultureFeed_Activity::CONTENT_TYPE_NODE;
      $query->type = array(CultureFeed_Activity::TYPE_RECOMMEND);
      $query->nodeId = url('node/' . $this->getEntity()->nid, array('absolute' => TRUE));
      $query->private = FALSE;
      $activitiesResult = DrupalCultureFeed::searchActivities($query);

      $stringType = CultureFeed_Activity::getNameById(CultureFeed_Activity::TYPE_RECOMMEND);
      $this->activityCounts[$stringType] = $activitiesResult->total;

      $query->type = array(CultureFeed_Activity::TYPE_COMMENT);
      $activitiesResult = DrupalCultureFeed::searchActivities($query);
      $stringType = CultureFeed_Activity::getNameById(CultureFeed_Activity::TYPE_COMMENT);
      $this->activityCounts[$stringType] = $activitiesResult->total;

    }
    catch (Exception $e) {
      watchdog_exception('culturefeed', $e);
    }

  }

}
