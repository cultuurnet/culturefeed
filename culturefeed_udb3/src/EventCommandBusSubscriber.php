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
use Broadway\Domain\Metadata;
use Symfony\Component\HttpKernel\KernelEvents;
use Symfony\Component\HttpKernel\Event\FilterControllerEvent;

/**
 * Class EventCommandBusSubscriber.
 *
 * @package Drupal\culturefeed_udb3
 */
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
   * @param FilterControllerEvent $event
   *   The event to process.
   */
  public function onKernelControllerRequest(FilterControllerEvent $event) {

    $request = $event->getRequest();

    /* @var \Symfony\Component\Routing\Route $route */
    $route = $request->attributes->get('_route_object');
    $culturefeed_user_requirement = $route->getRequirement('_culturefeed_current_user');

    if ($culturefeed_user_requirement) {

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

  }

  /**
   * {@inheritdoc}
   */
  public static function getSubscribedEvents() {
    $events[KernelEvents::CONTROLLER][] = array('onKernelControllerRequest');
    return $events;
  }

}
