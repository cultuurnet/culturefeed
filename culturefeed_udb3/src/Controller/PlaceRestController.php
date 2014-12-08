<?php

/**
 * @file
 * Contains Drupal\culturefeed_udb3\Controller\EventRestController.
 */

namespace Drupal\culturefeed_udb3\Controller;

use Drupal\Core\Controller\ControllerBase;
use Symfony\Component\DependencyInjection\ContainerInterface;
use CultuurNet\UDB3\PlaceServiceInterface;
use CultureFeed_User;
use Symfony\Component\HttpFoundation\JsonResponse;
use CultuurNet\UDB3\Symfony\JsonLdResponse;
use Symfony\Component\HttpFoundation\BinaryFileResponse;

class PlaceRestController extends ControllerBase {

  /**
   * The place service.
   *
   * @var PlaceServiceInterface
   */
  protected $placeService;

  /**
   * The culturefeed user.
   *
   * @var Culturefeed_User
   */
  protected $user;

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('culturefeed_udb3.place.service'),
      $container->get('culturefeed.current_user')
    );
  }

  /**
   * Constructs a RestController.
   *
   * @param PlaceServiceInterface $place_service
   *   The place service.
   * @param CultureFeed_User $user
   *   The culturefeed user.
   */
  public function __construct(
    PlaceServiceInterface $place_service,
    CultureFeed_User $user
  ) {
    $this->placeService = $place_service;
    $this->user = $user;
  }

  /**
   * Creates a json-ld response.
   *
   * @return BinaryFileResponse
   *   The response.
   */
  public function placeContext() {
    $response = new BinaryFileResponse('/udb3/api/1.0/place.jsonld');
    $response->headers->set('Content-Type', 'application/ld+json');
    return $response;
  }

  /**
   * Returns a place.
   *
   * @param string $cdbid
   *   The place id.
   *
   * @return JsonLdResponse
   *   The response.
   */
  public function details($cdbid) {

    $place = $this->placeService->getPlace($cdbid);

    $response = JsonResponse::create()
      ->setContent($place)
      ->setPublic()
      ->setClientTtl(60 * 30)
      ->setTtl(60 * 5);

    return $response;

  }

}
