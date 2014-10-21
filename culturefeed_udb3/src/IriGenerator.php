<?php
/**
 * @file
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
   * @param UrlGeneratorInterface $urlGenerator
   */
  public function __construct(
    UrlGeneratorInterface $urlGenerator,
    $eventRouteName = 'culturefeed_udb3.event'
  )
  {
    $this->urlGenerator = $urlGenerator;
    $this->eventRouteName = $eventRouteName;
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
