<?php
/**
 * @file
 */

namespace Drupal\culturefeed_udb3;


use CultuurNet\UDB3\Event\ReadModel\Relations\RepositoryInterface;
use Drupal\Core\Database\Connection;
use Drupal\Core\Entity\Query\QueryFactory;

class EventRelationsRepository implements RepositoryInterface{

  /**
   * The query factory.
   *
   * @var QueryFactory;
   */
  protected $queryFactory;

  /**
   * The database connection.
   *
   * @var Connection
   */
  protected $database;

  /**
   * @param QueryFactory $query_factory
   * @param Connection $database
   */
  public function __construct(
    QueryFactory $query_factory,
    Connection $database
  ) {
    $this->database = $database;
    $this->queryFactory = $query_factory;
  }

  public function storeRelations($eventId, $placeId, $organizerId) {
    // For optimal performance we use a merge query here
    // instead of the entity API.
    $query = $this->database->merge('culturefeed_udb3_event_relations')
      ->key(array('event' => $eventId))
      ->fields(
        array(
          'place' => $placeId,
          'organizer' => $organizerId
        )
      );

    $query->execute();
  }

  public function getEventsLocatedAtPlace($placeId) {
    $query = $this->queryFactory->get('event_relations');
    $query->condition('place', $placeId);

    return $query->execute();
  }

  public function getEventsOrganizedByOrganizer($organizerId) {
    $query = $this->queryFactory->get('event_relations');
    $query->condition('organizer', $organizerId);

    return $query->execute();
  }
}
