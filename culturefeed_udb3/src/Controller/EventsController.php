<?php

/**
 * @file
 * Contains Drupal\culturefeed\Controller\EventsController.
 */

namespace Drupal\culturefeed_udb3\Controller;

use Drupal\Core\Controller\ControllerBase;
use Symfony\Component\DependencyInjection\ContainerInterface;
use CultuurNet\UDB3\Event\EventTaggerServiceInterface;
use Symfony\Component\HttpFoundation\Request;
use CultuurNet\UDB3\Symfony\JsonLdResponse;

class EventsController extends ControllerBase {

  /**
   * The culturefeed user service.
   *
   * @var CultureFeed_User;
   */
  protected $user;

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('culturefeed_udb3.event.tagger')
    );
  }

  /**
   * Constructs a RestController.
   *
   * @param EventTaggerServiceInterface $event_tagger
   *   The event tagger.
   */
  public function __construct(EventTaggerServiceInterface $event_tagger) {
    $this->eventTagger = $event_tagger;
  }

  /**
   * Tag culturefeed events.
   *
   * @param Request $request
   *   The request.
   *
   * @return JsonLdResponse
   *   A json response.
   */
  public function tag(Request $request) {

    $response = JsonLdResponse::create()
      ->setData('test')
      ->setPublic()
      ->setClientTtl(60 * 30)
      ->setTtl(60 * 5);

    return $response;

  }

}
