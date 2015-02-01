<?php
/**
 * @file
 */

namespace Drupal\culturefeed_udb3;


use CultuurNet\UDB3\SimpleEventBus;
use Broadway\EventHandling\EventBusInterface;

class EventBusFactory{

  public function get($subscribers) {
    $bus = new SimpleEventBus();

    $bus->beforeFirstPublication(function (EventBusInterface $eventBus) use ($subscribers) {
      foreach ($subscribers as $subscriber) {
        $eventBus->subscribe(\Drupal::service($subscriber));
      }
    });

    return $bus;
  }

}
