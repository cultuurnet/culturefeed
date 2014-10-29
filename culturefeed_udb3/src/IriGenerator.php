<?php
/**
 * @file
 * Contains Drupal\culturefeed_udb3\IriGenerator.
 */

namespace Drupal\culturefeed_udb3;

use CultuurNet\UDB3\IriGeneratorInterface;
use Drupal\Core\Routing\UrlGeneratorInterface;

class IriGenerator implements IriGeneratorInterface {

  /**
   * Drupal's UrlGeneratorInterface.
   *
   * @var UrlGeneratorInterface
   */
  protected $urlGenerator;

  /**
   * Name of the route to show a single event.
   *
   * @var string
   */
  protected $eventRouteName;

  /**
   * Constructs a new IriGenerator for use with Drupal's URLGeneratorInterface.
   *
   * @param UrlGeneratorInterface $url_generator
   *   The url generator.
   * @param string $event_route_name
   *   The event route name.
   */
  public function __construct(
    UrlGeneratorInterface $url_generator,
    $event_route_name = 'culturefeed_udb3.event'
  ) {
    $this->urlGenerator = $url_generator;
    $this->eventRouteName = $event_route_name;
  }

  /**
   * {@inheritdoc}
   */
  public function iri($item) {
    return $this->urlGenerator->generateFromRoute(
      $this->eventRouteName,
      array(
        'cdbid' => $item,
      ),
      array(
        'absolute' => TRUE,
      )
    );
  }

}
