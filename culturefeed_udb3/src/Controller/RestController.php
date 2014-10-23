<?php

/**
 * @file
 * Contains Drupal\culturefeed_udb3\Controller\RestController.
 */

namespace Drupal\culturefeed_udb3\Controller;

use CultuurNet\UDB3\EventServiceInterface;
use CultuurNet\UDB3\SearchServiceInterface;
use Drupal\Core\Controller\ControllerBase;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\HttpFoundation\JsonResponse;

use Symfony\Component\HttpFoundation\BinaryFileResponse;

class RestController extends ControllerBase {

  /**
   * The search service.
   *
   * @var SearchServiceInterface;
   */
  protected $searchService;

  /**
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
   * @param SearchServiceInterface $searchService
   */
  public function __construct(SearchServiceInterface $searchService, EventServiceInterface $eventService) {
    $this->searchService = $searchService;
    $this->eventService = $eventService;
  }

  /**
   * Executes a search and returns the results.
   *
   * @return Response
   *   A response.
   */
  public function search(Request $request) {

    $q = $request->query->get('query', '*.*');
    $limit = $request->query->get('limit', 30);
    $start = $request->query->get('start', 0);

    $response = $this->searchService->search($q, $limit, $start);

    $response = JsonResponse::create()
      ->setData($response)
      ->setPublic()
      ->setClientTtl(60 * 1)
      ->setTtl(60 * 5);

    $response->headers->set('Content-Type', 'application/ld+json');

    return $response;

  }

  function eventContext() {
    $response = new BinaryFileResponse('/udb3/api/1.0/event.jsonld');
    $response->headers->set('Content-Type', 'application/ld+json');
    return $response;
  }

  function event($cdbid) {
    $event = $this->eventService->getEvent($cdbid);

    /** @var \Symfony\Component\HttpFoundation\JsonResponse $response */
    $response = JsonResponse::create()
      ->setData($event)
      ->setPublic()
      ->setClientTtl(60 * 30)
      ->setTtl(60 * 5);

    $response->headers->set('Content-Type', 'application/ld+json');

    return $response;

  }

}
