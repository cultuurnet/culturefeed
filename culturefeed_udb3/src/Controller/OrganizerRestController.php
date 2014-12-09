<?php

/**
 * @file
 * Contains Drupal\culturefeed_udb3\Controller\OrganizerRestController.
 */

namespace Drupal\culturefeed_udb3\Controller;

use Drupal\Core\Controller\ControllerBase;
use Symfony\Component\DependencyInjection\ContainerInterface;
use CultuurNet\UDB3\EntityServiceInterface;
use CultureFeed_User;
use Symfony\Component\HttpFoundation\JsonResponse;
use CultuurNet\UDB3\Symfony\JsonLdResponse;
use Symfony\Component\HttpFoundation\BinaryFileResponse;

/**
 * Class OrganizerRestController.
 *
 * @package Drupal\culturefeed_udb3\Controller
 */
class OrganizerRestController extends ControllerBase {

  /**
   * The entity service.
   *
   * @var EntityServiceInterface
   */
  protected $entityService;

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
      $container->get('culturefeed_udb3.organizer.service'),
      $container->get('culturefeed.current_user')
    );
  }

  /**
   * Constructs a RestController.
   *
   * @param EntityServiceInterface $entity_service
   *   The entity service.
   * @param CultureFeed_User $user
   *   The culturefeed user.
   */
  public function __construct(
    EntityServiceInterface $entity_service,
    CultureFeed_User $user
  ) {
    $this->entityService = $entity_service;
    $this->user = $user;
  }

  /**
   * Creates a json-ld response.
   *
   * @return BinaryFileResponse
   *   The response.
   */
  public function organizerContext() {
    $response = new BinaryFileResponse('/udb3/api/1.0/organizer.jsonld');
    $response->headers->set('Content-Type', 'application/ld+json');
    return $response;
  }

  /**
   * Returns an organizer.
   *
   * @param string $cdbid
   *   The place id.
   *
   * @return JsonLdResponse
   *   The response.
   */
  public function details($cdbid) {

    $organizer = $this->entityService->getEntity($cdbid);

    $response = JsonResponse::create()
      ->setContent($organizer)
      ->setPublic()
      ->setClientTtl(60 * 30)
      ->setTtl(60 * 5);

    return $response;

  }

}
