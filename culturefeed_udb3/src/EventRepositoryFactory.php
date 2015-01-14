<?php

/**
 * @file
 * Contains Drupal\culturefeed_udb3\EventRepositoryFactory.
 */

namespace Drupal\culturefeed_udb3;

use CultuurNet\UDB3\Event\EventRepository;
use CultuurNet\UDB3\OrganizerService;
use CultuurNet\UDB3\PlaceService;
use CultuurNet\UDB3\SearchAPI2\DefaultSearchService;
use CultuurNet\UDB3\UDB2\EntryAPIFactory;
use CultuurNet\UDB3\UDB2\EntryAPIImprovedFactory;
use Broadway\EventSourcing\MetadataEnrichment\MetadataEnrichingEventStreamDecorator;
use Drupal\Core\Config\ConfigFactory;

/**
 * Class EventRepositoryFactory.
 *
 * @package Drupal\culturefeed_udb3
 */
class EventRepositoryFactory implements EventRepositoryFactoryInterface {

  /**
   * The local event repository.
   *
   * @var \CultuurNet\UDB3\Event\EventRepository
   */
  protected $localEventRepository;

  /**
   * The search api.
   *
   * @var \CultuurNet\UDB3\SearchAPI2\DefaultSearchService
   */
  protected $searchApi;

  /**
   * The entry api.
   *
   * @var \CultuurNet\UDB3\UDB2\EntryAPIFactory
   */
  protected $entryApi;

  /**
   * The event stream metadata enricher.
   *
   * @var \Broadway\EventSourcing\MetadataEnrichment\MetadataEnrichingEventStreamDecorator
   */
  protected $eventStreamMetadataEnricher;

  /**
   * The config factory.
   *
   * @var \Drupal\Core\Config\ConfigFactory
   */
  protected $config;

  /**
   * @var OrganizerService
   */
  protected $organizerService;

  /**
   * @var PlaceService
   */
  protected $placeService;

  /**
   * Constructs an event repository factory.
   *
   * @param EventRepository $local_event_repository
   *   The local event repository.
   * @param DefaultSearchService $search_api
   *   The search api.
   * @param EntryAPIImprovedFactory $improved_entry_api
   *   The improved entry api.
   * @param PlaceService $place_service
   * @param OrganizerService $organizer_service
   * @param MetadataEnrichingEventStreamDecorator $event_stream_metadata_enricher
   *   The event stream metadata enricher.
   * @param ConfigFactory $config
   *   The config factory.
   */
  public function __construct(
    EventRepository $local_event_repository,
    DefaultSearchService $search_api,
    EntryAPIImprovedFactory $improved_entry_api,
    PlaceService $place_service,
    OrganizerService $organizer_service,
    MetadataEnrichingEventStreamDecorator $event_stream_metadata_enricher,
    ConfigFactory $config
  ) {
    $this->localEventRepository = $local_event_repository;
    $this->searchApi = $search_api;
    $this->improvedEntryApi = $improved_entry_api;
    $this->organizerService = $organizer_service;
    $this->placeService = $place_service;
    $this->eventStreamMetadataEnricher = $event_stream_metadata_enricher;
    $this->config = $config->get('culturefeed_udb3.settings');
  }

  /**
   * {@inheritdoc}
   */
  public function get() {

    $udb2_repository_decorator = new \CultuurNet\UDB3\UDB2\EventRepository(
      $this->localEventRepository,
      $this->searchApi,
      $this->improvedEntryApi,
      $this->placeService,
      $this->organizerService,
      array($this->eventStreamMetadataEnricher)
    );

    if ($this->config->get('sync_with_udb2')) {
      $udb2_repository_decorator->syncBackOn();
    }

    return $udb2_repository_decorator;

  }

}
