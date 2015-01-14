<?php
/**
 * @file
 */

namespace Drupal\culturefeed_udb3;


use CultuurNet\UDB3\SimpleEventBus;

class EventBusFactory{

  private $subscribers;

  public function get($subscribers) {
    $bus = new SimpleEventBus();

    $bus->beforeFirstPublication(function (\Broadway\EventHandling\EventBusInterface $eventBus) use ($subscribers) {
      foreach ($subscribers as $subscriber) {
        $eventBus->subscribe(\Drupal::service($subscriber));
      }
    });

    return $bus;
  }

}
