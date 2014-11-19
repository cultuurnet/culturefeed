<?php

/**
 * @file
 * Contains \Drupal\culturefeed_udb3\EventCommandBusSubscriber.
 */

namespace Drupal\culturefeed_udb3;

use CultuurNet\Auth\TokenCredentials;
use Symfony\Component\EventDispatcher\EventSubscriberInterface;
use CultuurNet\UDB3\CommandHandling\ResqueCommandBus;
use CultureFeed_User;
use Drupal\culturefeed\UserCredentials;
use Symfony\Component\HttpKernel\Event\GetResponseEvent;
use Broadway\Domain\Metadata;
use Symfony\Component\HttpKernel\KernelEvents;

class EventCommandBusSubscriber implements EventSubscriberInterface {

  /**
   * The event command bus.
   *
   * @var \CultuurNet\UDB3\CommandHandling\ResqueCommandBus
   */
  protected $eventCommandBus;

  /**
   * The current culturefeed user.
   *
   * @var \CultureFeed_User
   */
  protected $user;

  /**
   * The culturefeed user credentials.
   *
   * @var \Drupal\culturefeed\UserCredentials
   */
  protected $userCredentials;

  /**
   * Constructs the event command bus listener.
   *
   * @param ResqueCommandBus $resque_command_bus
   *   The event command bus.
   * @param CultureFeed_User $user
   *   The culturefeed user.
   * @param UserCredentials $user_credentials
   *   The culturefeed user credentials.
   */
  public function __construct(ResqueCommandBus $resque_command_bus, CultureFeed_User $user, UserCredentials $user_credentials) {
    $this->eventCommandBus = $resque_command_bus;
    $this->user = $user;
    $this->userCredentials = $user_credentials;
  }

  /**
   * Registers JSON-LD formats with the Request class.
   *
   * @param GetResponseEvent $event
   *   The event to process.
   */
  public function onKernelRequest(GetResponseEvent $event) {

    $request = $event->getRequest();
    $context_values = array();

    if ($this->user) {

      $context_values['user_id'] = $this->user->id;
      $context_values['user_nick'] = $this->user->nick;
      $credentials = new TokenCredentials($this->userCredentials->getToken(), $this->userCredentials->getSecret());
      $context_values['uitid_token_credentials'] = $credentials;

    }

    $context_values['client_ip'] = $request->getClientIp();
    $context_values['request_time'] = $_SERVER['REQUEST_TIME'];
    $context = new Metadata($context_values);
    $this->eventCommandBus->setContext($context);

  }

  /**
   * {@inheritdoc}
   */
  public static function getSubscribedEvents() {
    $events[KernelEvents::REQUEST][] = array('onKernelRequest', 40);
    return $events;
  }

}
