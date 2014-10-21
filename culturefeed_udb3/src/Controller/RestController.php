<?php

/**
 * @file
 * Contains Drupal\culturefeed_udb3\Controller\RestController.
 */

namespace Drupal\culturefeed_udb3\Controller;

use Drupal\culturefeed_udb3\SearchApi;
use Drupal\Core\Controller\ControllerBase;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\HttpFoundation\JsonResponse;

use CultuurNet\Search\Parameter\Query;
use CultuurNet\Search\SearchResult;
use Symfony\Component\HttpFoundation\BinaryFileResponse;
use SimpleXMLElement;

class RestController extends ControllerBase {

  /**
   * The culturefeed search api.
   *
   * @var \Drupal\culturefeed_udb3\SearchApi;
   */
  protected $searchApi;

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('culturefeed_udb3.search_api')
    );
  }

  /**
   * Constructs a RestController object.
   *
   * @param \Drupal\culturefeed_udb3\SearchApi $search_api
   *   The search api.
   */
  public function __construct(SearchApi $search_api) {
    $this->searchApi = $search_api;
  }


  /**
   * Returns the search results.
   *
   * @return Response
   *   A response.
   */
  public function search() {

    $q = \Drupal::request()->query->get('q');
    $q = new Query($q);
    $response = $this->searchApi->service->search(array($q));

    $results = SearchResult::fromXml(new SimpleXMLElement($response->getBody(true), 0, false, \CultureFeed_Cdb_Default::CDB_SCHEME_URL));

    $response = Response::create()
      ->setContent($results->getXml())
      ->setPublic()
      ->setClientTtl(60 * 1)
      ->setTtl(60 * 5);

    $response->headers->set('Content-Type', 'text/xml');

    return $response;

  }

  function event() {
    $response = new BinaryFileResponse('/udb3/api/1.0/event.jsonld');
    $response->headers->set('Content-Type', 'application/ld+json');
    return $response;
  }

  /**
   * Returns the search results.
   *
   * @return Response
   *   A response.
   */
  function api_search() {

    $query = \Drupal::request()->query->get('query', '*.*');
    $limit = \Drupal::request()->query->get('limit', 30);
    $start = \Drupal::request()->query->get('start', 0);

    $results = $this->searchApi->service->search($query, $limit, $start);

    $response = Response::create()
      ->setContent(json_encode($results))
      ->setPublic()
      ->setClientTtl(60 * 1)
      ->setTtl(60 * 5);

    $response->headers->set('Content-Type', 'application/ld+json');

    return $response;

  }

  function eventDetail($cdbid) {

    $service = $this->searchApi->service;

    /** @var \Symfony\Component\HttpFoundation\JsonResponse $response */
    $response = JsonResponse::create()
      ->setPublic()
      ->setClientTtl(60 * 1)
      ->setTtl(60 * 5);

    $event = $service->getEvent($cdbid);
    $response
      ->setData($event)
      ->setPublic()
      ->setClientTtl(60 * 30)
      ->setTtl(60 * 5);

    $response->headers->set('Content-Type', 'application/ld+json');

    return $response;

  }

}
