<?php

/**
 * @file
 * Contains Drupal\culturefeed_udb3\Controller\RestController.
 */

namespace Drupal\culturefeed_udb3\Controller;

use Drupal\Core\Controller\ControllerBase;
use Symfony\Component\DependencyInjection\ContainerInterface;
use CultuurNet\UDB3\Search\PullParsingSearchService;
use CultuurNet\UDB3\EventServiceInterface;
use Symfony\Component\HttpFoundation\Request;
use CultuurNet\UDB3\Symfony\JsonLdResponse;
use Symfony\Component\HttpFoundation\BinaryFileResponse;

class RestController extends ControllerBase {

  /**
   * The search service.
   *
   * @var PullParsingSearchService;
   */
  protected $searchService;

  /**
   * The event service.
   *
   * @var EventServiceInterface
   */
  protected $eventService;

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('culturefeed_udb3.search'),
      $container->get('culturefeed_udb3.event')
    );
  }

  /**
   * Constructs a RestController.
   *
   * @param PullParsingSearchService $search_service
   *   The search service.
   * @param EventServiceInterface $event_service
   *   The event service.
   */
  public function __construct(PullParsingSearchService $search_service, EventServiceInterface $event_service) {
    $this->searchService = $search_service;
    $this->eventService = $event_service;
  }

  /**
   * Executes a search and returns the results.
   *
   * @param Request $request
   *   The request.
   *
   * @return JsonLdResponse
   *   A response.
   */
  public function search(Request $request) {

    $q = $request->query->get('query', '*.*');
    $limit = $request->query->get('limit', 30);
    $start = $request->query->get('start', 0);

    $response = $this->searchService->search($q, $limit, $start);

    $response = JsonLdResponse::create()
      ->setData($response)
      ->setPublic()
      ->setClientTtl(60 * 1)
      ->setTtl(60 * 5);

    return $response;

  }

  /**
   * Creates a json-ld response.
   *
   * @return BinaryFileResponse
   *   The response.
   */
  public function eventContext() {
    $response = new BinaryFileResponse('/udb3/api/1.0/event.jsonld');
    $response->headers->set('Content-Type', 'application/ld+json');
    return $response;
  }

  /**
   * Returns an event.
   *
   * @param string $cdbid
   *   The event id.
   *
   * @return JsonLdResponse
   *   The response.
   */
  public function event($cdbid) {
    $event = $this->eventService->getEvent($cdbid);

    /** @var \Symfony\Component\HttpFoundation\JsonResponse $response */
    $response = JsonLdResponse::create()
      ->setData($event)
      ->setPublic()
      ->setClientTtl(60 * 30)
      ->setTtl(60 * 5);

    return $response;

  }

}
