<?php

/**
 * @file
 * Contains Drupal\culturefeed_udb3\Controller\EventRestController.
 */

namespace Drupal\culturefeed_udb3\Controller;

use CultuurNet\UDB3\Event\EventEditingServiceInterface;
use Drupal\Core\Controller\ControllerBase;
use Symfony\Component\DependencyInjection\ContainerInterface;
use CultuurNet\UDB3\Search\PullParsingSearchService;
use CultuurNet\UDB3\EventServiceInterface;
use CultuurNet\UDB3\Language;
use CultuurNet\UDB3\Keyword;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\JsonResponse;
use CultuurNet\UDB3\Symfony\JsonLdResponse;
use Symfony\Component\HttpFoundation\BinaryFileResponse;

class EventRestController extends ControllerBase {

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
      $container->get('culturefeed_udb3.event.service'),
      $container->get('culturefeed_udb3.event.editor')
    );
  }

  /**
   * Constructs a RestController.
   *
   * @param EventServiceInterface $event_service
   *   The event service.
   * @param EventEditingServiceInterface $event_editor
   *   The event editor.
   */
  public function __construct(EventServiceInterface $event_service, EventEditingServiceInterface $event_editor) {
    $this->eventService = $event_service;
    $this->eventEditor = $event_editor;
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
  public function details($cdbid) {
    $event = $this->eventService->getEvent($cdbid);

    $response = JsonResponse::create()
      ->setContent($event)
      ->setPublic()
      ->setClientTtl(60 * 30)
      ->setTtl(60 * 5);

    return $response;

  }

  /**
   * Modifies the event tile.
   *
   * @param Request $request
   *   The request.
   * @param string $cdbid
   *   The event id.
   * @param string $language
   *   The event language.
   *
   * @return JsonResponse
   *   The response.
   */
  public function title(Request $request, $cdbid, $language) {

    $response = new JsonResponse();
    $body_content = json_decode($request->getContent());

    if (!$body_content->title) {
      return new JsonResponse(['error' => "title required"], 400);
    }

    try {
      $command_id = $this->eventEditor->translateTitle(
        $cdbid,
        new Language($language),
        $body_content->title
      );

      $response->setData(['commandId' => $command_id]);
    }
    catch (\Exception $e) {
      $response->setStatusCode(400);
      $response->setData(['error' => $e->getMessage()]);
    }

    return $response;

  }

  /**
   * Modifies the event description.
   *
   * @param Request $request
   *   The request.
   * @param string $cdbid
   *   The event id.
   * @param string $language
   *   The event language.
   *
   * @return JsonResponse
   *   The response.
   */
  public function description(Request $request, $cdbid, $language) {

    $response = new JsonResponse();
    $body_content = json_decode($request->getContent());

    if (!$body_content->description) {
      return new JsonResponse(['error' => "description required"], 400);
    }

    try {
      $command_id = $this->eventEditor->translateDescription(
        $cdbid,
        new Language($language),
        $body_content->description
      );

      $response->setData(['commandId' => $command_id]);
    }
    catch (\Exception $e) {
      $response->setStatusCode(400);
      $response->setData(['error' => $e->getMessage()]);
    }

    return $response;

  }

  /**
   * Adds a keyword.
   *
   * @param Request $request
   *   The request.
   * @param string $cdbid
   *   The event id.
   *
   * @return JsonResponse
   *   The response.
   */
  public function addKeyword(Request $request, $cdbid) {

    $response = new JsonResponse();
    $body_content = json_decode($request->getContent());

    try {
      $command_id = $this->eventEditor->tag(
        $cdbid,
        new Keyword($body_content->keyword)
      );

      $response->setData(['commandId' => $command_id]);
    }
    catch (\Exception $e) {
      $response->setStatusCode(400);
      $response->setData(['error' => $e->getMessage()]);
    }

    return $response;

  }

  /**
   * Deletes a keyword.
   *
   * @param Request $request
   *   The request.
   * @param string $cdbid
   *   The event id.
   * @param string $keyword
   *   The keyword.
   *
   * @return JsonResponse
   *   The response.
   */
  public function deleteKeyword(Request $request, $cdbid, $keyword) {

    $response = new JsonResponse();

    try {
      $command_id = $this->eventEditor->eraseTag(
        $cdbid,
        new Keyword($keyword)
      );

      $response->setData(['commandId' => $command_id]);
    }
    catch (\Exception $e) {
      $response->setStatusCode(400);
      $response->setData(['error' => $e->getMessage()]);
    }

    return $response;

  }

}
