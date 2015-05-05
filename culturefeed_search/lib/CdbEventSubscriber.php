<?php

use Symfony\Component\EventDispatcher\EventSubscriberInterface;
use Symfony\Component\EventDispatcher\Event;

class CdbEventSubscriber implements EventSubscriberInterface {

  /**
   * {@inheritdoc}
   */
  public static function getSubscribedEvents() {
    return array(
      'request.before_send' => array('onRequestBeforeSend', 255),
    );
  }

  /**
   * Called before a request is sent
   *
   * @param Event $event
   */
  public function onRequestBeforeSend(Event $event) {

    $request = $event['request'];
    $request->getQuery()->set('version', '3.3');

  }

}
